# -*- coding: utf-8 -*-

# Contains the configuration for the default configuration

from ..utils.files import load_file
from ..utils.session_interface import WCICS_Session_Interface

from .paths import *

from flask import json

import os

app_properties = dict(
  secret_key = load_file(KEYS_FOLDER_PATH + "/jwt-key.pem"),
  session_interface = WCICS_Session_Interface()
)

MAIL_SERVER, MAIL_USERNAME, MAIL_PASSWORD, MAIL_DEFAULT_USERNAME, MAIL_DEFAULT_EMAIL = load_file(KEYS_FOLDER_PATH + "/new_mail_key.pem").splitlines()

google_secret = json.load(open(KEYS_FOLDER_PATH + "/google-secret.json"))['web']
github_client_id, github_client_secret = load_file(KEYS_FOLDER_PATH + "/github-{}-oauth.pem".format(os.environ["WCICS_DOMAIN"])).splitlines()

app_config = dict(
  MAIL_SERVER = MAIL_SERVER,
  MAIL_DEBUG = False,
  MAIL_USE_TLS = True,
  MAIL_USERNAME = MAIL_USERNAME or None,
  MAIL_PASSWORD = MAIL_PASSWORD or None,
  MAIL_DEFAULT_SENDER = (MAIL_DEFAULT_USERNAME, MAIL_DEFAULT_EMAIL),
  
  SQLALCHEMY_DATABASE_URI = load_file(KEYS_FOLDER_PATH + "/database_uri.pem"),
  SQLALCHEMY_TRACK_MODIFICATIONS = False, # Will be deprecated eventually
  
  COOKIES_SECURE = True,
  COOKIES_HTTPONLY = True,
  COOKIES_SAMESITE = 'Lax',
  
  RECAPTCHA_PUBLIC_KEY = load_file(KEYS_FOLDER_PATH + "/grecaptcha-public.pem"),
  RECAPTCHA_PRIVATE_KEY = load_file(KEYS_FOLDER_PATH + "/grecaptcha-secret.pem"),

  MISAKA_CONFIG = json.load(open(CONFIG_FOLDER_PATH + "/misaka.json")),
  
  PREFERRED_URL_SCHEME = 'https',
  
  TRAP_BAD_REQUEST_ERRORS = True,
  
  SERVER_NAME = os.environ['WCICS_DOMAIN'],
  
  GOOGLE_OAUTH_CLIENT_ID = google_secret['client_id'],
  GOOGLE_OAUTH_CLIENT_SECRET = google_secret['client_secret'],
  
  GITHUB_OAUTH_CLIENT_ID = github_client_id,
  GITHUB_OAUTH_CLIENT_SECRET = github_client_secret,
)

app_params = dict(
  subdomain_matching = True
)

misc_config = dict(
  STATIC_FOLDER_PATH = app_params.get('static_folder', "static"),
  MISAKA_CONFIG = app_config['MISAKA_CONFIG'],
  CSP_HEADERS = json.load(open(CONFIG_FOLDER_PATH + "/csp-headers.json")),
  CAPTCHA_PUBLIC = load_file(KEYS_FOLDER_PATH + "/grecaptcha-public.pem"),
  CAPTCHA_SECRET = load_file(KEYS_FOLDER_PATH + "/grecaptcha-secret.pem")
)

configuration = dict(
  properties = app_properties,
  
  config = app_config,
  
  params = app_params,
  
  misc = misc_config
)
