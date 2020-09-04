# -*- coding: utf-8 -*-

from .utils.files import load_file

from logging import getLevelName

# Import our environment variables
from os import environ

# Change a constant
# This is used to setup our config
def set_constant(key, val):
  globals()[key] = val

# Initialize constants with null values and then later we will load them from config
DOMAIN = None
USERNAME = None
BRANCH = None
MISAKA_CONFIG = None
STATIC_FOLDER_PATH = None
PORT = None
PRESET = None

# Keys
CAPTCHA_PUBLIC = None
CAPTCHA_SECRET = None
