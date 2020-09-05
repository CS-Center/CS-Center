# -*- coding: utf-8 -*-

# Contains the configuration for the default configuration

from ..utils.files import load_file
from ..utils.session_interface import WCICS_Session_Interface

from .keys import keys
from .consts import DOMAIN, PRESET, LOCAL

from flask import json

import os

app_properties = dict(
  secret_key = keys['JWT_KEY'],
  session_interface = WCICS_Session_Interface()
)

GRECAPTCHA = keys['GRECAPTCHA']

GOOGLE = {} if LOCAL else keys['OAUTH_GOOGLE']['web']

GITHUB = [0,0] if LOCAL else keys['OAUTH_GITHUB'][DOMAIN]

app_config = dict(
  MAIL_SERVER = keys['MAIL']['SERVER'],
  MAIL_DEBUG = False,
  MAIL_USE_TLS = True,
  MAIL_USERNAME = keys['MAIL'].get("USERNAME"),
  MAIL_PASSWORD = keys['MAIL'].get("PASSWORD"),
  MAIL_DEFAULT_SENDER = tuple(keys['MAIL']['DEFAULT_SENDER']),

  SQLALCHEMY_DATABASE_URI = keys['DATABASE_URI'],
  SQLALCHEMY_TRACK_MODIFICATIONS = False, # Will be deprecated eventually

  COOKIES_SECURE = True,
  COOKIES_HTTPONLY = True,
  COOKIES_SAMESITE = 'Lax',

  RECAPTCHA_PUBLIC_KEY = GRECAPTCHA[0],
  RECAPTCHA_PRIVATE_KEY = GRECAPTCHA[1],

  MISAKA_CONFIG = json.load(open("wcics/config/config-files/misaka.json")),

  PREFERRED_URL_SCHEME = 'https',

  TRAP_BAD_REQUEST_ERRORS = True,

  SERVER_NAME = DOMAIN,

  GOOGLE_OAUTH_CLIENT_ID = GOOGLE.get('client_id'),
  GOOGLE_OAUTH_CLIENT_SECRET = GOOGLE.get('client_secret'),

  GITHUB_OAUTH_CLIENT_ID = GITHUB[0],
  GITHUB_OAUTH_CLIENT_SECRET = GITHUB[1],
)

app_params = dict(
  subdomain_matching = True
)

misc_config = dict(
  CSP_HEADERS = json.load(open('wcics/config/config-files/csp-headers.json'))
)

configuration = dict(
  properties = app_properties,

  config = app_config,

  params = app_params,

  misc = misc_config
)
