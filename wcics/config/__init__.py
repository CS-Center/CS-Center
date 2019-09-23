# -*- coding: utf-8 -*-

from ..consts import set_constant
from wcics.config import debug, testing, default, prod

from flask_mail import Mail
from flask_misaka import Misaka
from flask_mobility import Mobility
from flask_sqlalchemy import SQLAlchemy
from flask_socketio import SocketIO

from flask import Flask, json

from werkzeug.middleware.proxy_fix import ProxyFix

import os

def configure_app():
  preset_config_name = os.environ.get("WCICS_PRESET", "")
  
  module = None
  
  if preset_config_name == 'debug':
    module = debug
    
  elif preset_config_name == 'testing':
    module = testing
    
  elif preset_config_name == "production":
    module = prod
    
  else:
    raise ValueError("Preset configuration '%s' not recognized." % preset_config_name)
    
  preset_config = module.configuration
    
  final_config = default.configuration
  
  for key in preset_config:
    final_config[key].update(preset_config[key])
    
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