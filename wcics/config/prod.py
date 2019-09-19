# -*- coding: utf-8 -*-

import os

# Contains the configuration for the production default configuration

app_properties = {}

app_config = dict(
  SERVER_NAME = "www." + os.environ['WCICS_DOMAIN'],
)

app_params = {}

misc_config = dict(
  DEBUG_LEVEL = "INFO"
)

configuration = dict(
  properties = app_properties,
  
  config = app_config,
  
  params = app_params,
  
  misc = misc_config
)

def setup_func(): pass