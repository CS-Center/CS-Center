# -*- coding: utf-8 -*-

from flask import templating, before_render_template, template_rendered
# Contains the configuration for the testing default configuration

app_properties = dict(
  testing = True
)

app_config = dict(
  SQLALCHEMY_DATABASE_URI = "sqlite://", # In-memory sqlite database
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
  def blank_render(template, context, app):
    # Fake the signals, just in case code depends on them
    before_render_template.send(app, template=template, context=context)
    
    template_rendered.send(app, template=template, context=context)
    
    return ""
  
  templating._render = blank_render