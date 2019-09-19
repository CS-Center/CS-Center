# -*- coding: utf-8 -*-

# Contains the configuration for the debug default configuration

app_properties = dict(
  debug = True
)

app_config = dict(
  ENV = 'development'
)

app_params = {}

misc_config = {}

configuration = dict(
  properties = app_properties,
  
  config = app_config,
  
  params = app_params,
  
  misc = misc_config
)

def setup_func():
  pass