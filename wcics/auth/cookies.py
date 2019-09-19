# -*- coding: utf-8 -*-
# File defines a single function, a set cookie that takes the app cookie configuration into account
# If the app cannot be imported, use _cookies

from wcics import app
from wcics.auth import _cookies

def set_cookie(response, key, val):
  _cookies.set_cookie(app.config, response, key, val)