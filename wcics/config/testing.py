# -*- coding: utf-8 -*-

from flask import templating, before_render_template, template_rendered, get_flashed_messages, make_response, json
# Contains the configuration for the testing default configuration

app_properties = dict(
  testing = True
)

app_config = dict(
  SQLALCHEMY_DATABASE_URI = "sqlite:///:memory:", # In-memory sqlite database
  WTF_CSRF_ENABLED = False
)

app_params = {}

misc_config = {}

configuration = dict(
  properties = app_properties,
  
  config = app_config,
  
  params = app_params,
  
  misc = misc_config
)

# A function to be run at configuration time, after the app has been constructed
def setup_func():
  orig_render = templating._render
  
  def blank_render(template, context, app):    
    fm = get_flashed_messages(False, "ERROR")
    if fm:
      return make_response(json.dumps(fm), 400, {'content-type' : 'application/json'})
    
    return orig_render(template, context, app)
  
  templating._render = blank_render