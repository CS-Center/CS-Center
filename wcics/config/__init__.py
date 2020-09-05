# -*- coding: utf-8 -*-
import sys

from .consts import set_constant
from wcics.config import debug, testing, default, prod
from wcics.auth._jwt import verify_jwt, make_jwt, InvalidJWT, ExpiredJWT
from wcics.auth._cookies import set_cookie

from flask_mail import Mail
from flask_misaka import Misaka
from flask_mobility import Mobility
from flask_sqlalchemy import SQLAlchemy
from flask_socketio import SocketIO

from flask import Flask, json, request, session, redirect

from werkzeug.middleware.proxy_fix import ProxyFix

import os, base64

def configure_app():
  preset_config_name = os.environ['CSCENTER_PRESET']

  module = None

  if preset_config_name == 'debug':
    module = debug

  elif preset_config_name == 'testing':
    module = testing

  elif preset_config_name == "production" or preset_config_name == 'prod-test':
    module = prod

  else:
    raise ValueError("Preset configuration '%s' not recognized." % preset_config_name)

  preset_config = module.configuration

  final_config = default.configuration

  for key in preset_config:
    final_config[key].update(preset_config[key])

  if module is prod:
    # Have a default subdomain for the static route, since flask is bad
    from werkzeug.routing import Map
    class CSCMap(Map):
      def __init__(self, *args, **kwargs):
        if kwargs.get("default_subdomain") is None:
          kwargs['default_subdomain'] = 'www'
        super().__init__(*args, **kwargs)

    Flask.url_map_class = CSCMap

  application = Flask('wcics', **final_config['params'])

  for attr, val in final_config['properties'].items():
    setattr(application, attr, val)

  for attr, val in final_config['misc'].items():
    set_constant(attr, val)

  for attr, val in os.environ.items():
    if attr.startswith("WCICS_"):
      set_constant(attr[6:], val)

  application.config.from_mapping(final_config['config'])

  # Run setup function for the configuration
  module.setup_func()

  if preset_config_name != 'production' and not application.testing:
    # If we aren't full prod, we make ppl enter password
    password = base64.b64encode(os.urandom(12)).decode("utf-8")

    nonce = 'i am a noncy boi'

    print("\nYOUR DEBUG PASSWORD IS: %s\n" % password)

    @application.before_request
    def force_password_entry():
      if request.path == '/favicon.ico':
        # Let them have a favicon lol
        return

      msg = ""

      if request.path == '/submit-password/':
        if request.form['password'] == password:
          rv = redirect(session.get("next", '/'))
          set_cookie(application.config, rv, "_dev_pwd", make_jwt(dict(
            nonce = nonce
          ), application.secret_key))

          return rv

        msg = "Invalid Password!"

      # Check for a cookie, if they have one, they can have in
      pwd = request.cookies.get('_dev_pwd', '')

      session['next'] = request.url

      class InvalidNonce(Exception): pass

      try:
        body = verify_jwt(pwd, application.secret_key)
        # CLI file for `wcics start`

        if body.get("nonce") != nonce:
          msg = "Invalid nonce provided!"
          raise InvalidNonce

      except (InvalidNonce, InvalidJWT, ExpiredJWT):
        return """
        {}<br>
        A password is required for entry. Check your terminal.
        <form action='/submit-password/', method='post'>
          <input type='password' name='password' />
          <input type='submit' />
        </form>
        """.format(msg)

  return application

def add_middlewares(application):
  # if debug, we can assume that its behind the nginx proxy
  if application.debug:
    application.wsgi_app = ProxyFix(application.wsgi_app, x_for = 1, x_proto = 1, x_host = 1)

def add_extensions(application):
  global db
  global mail_app
  global md
  global md_no_mj
  global logger
  global mb
  global socketio

  # App.testing indicates that the database is sqlite://:memory:
  if not app.testing:
    application.config['SQLALCHEMY_DATABASE_URI'] += "_debug" if application.debug else "_prod"

  db = SQLAlchemy(application)

  mail_app = Mail(application)

  md = Misaka(application, **application.config['MISAKA_CONFIG'])
  md_no_mj = Misaka(application, **{**application.config['MISAKA_CONFIG'], "math": False, "math_explicit": False})

  mb = Mobility(app)

  logger = application.logger

  socketio = SocketIO(app)

# Call configuration functions to setup our objects
app = configure_app()

# Add extensions first so that socketio middleware gets called _last_
# This way the logging IP is still correct, since socketio middleware copies the request environment
# Applying middlewares later ensures that they get called _first_
add_extensions(app)

add_middlewares(app)
