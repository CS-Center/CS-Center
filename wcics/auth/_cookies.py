# -*- coding: utf-8 -*-
# File defines a single function, a set cookie that takes a configuration
# Use only if the app cannot be imported, otherwise use .cookies

def set_cookie(config, response, key, val):
  response.set_cookie(key, val,
                      secure = config['COOKIES_SECURE'], 
                      httponly = config['COOKIES_HTTPONLY'], 
                      samesite = config['COOKIES_SAMESITE'])