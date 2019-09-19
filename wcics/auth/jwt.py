# -*- coding: utf-8 -*-

# JWT tokens are used. More info at https://jwt.io
# This file assumes that the app can be imported
# Uses the app secret key to sign and verify
import jwt

from wcics import app
from wcics.utils.time import get_time
from wcics.auth import _jwt 

from .errors import InvalidJWT, ExpiredJWT
  
# Verify a given JWT token using the app
def verify_jwt(tkn):
  return _jwt.verify_jwt(tkn, app.secret_key)

# Make a new JWT with the given payload
def make_jwt(payload):
  return _jwt.make_jwt(payload, app.secret_key)
  