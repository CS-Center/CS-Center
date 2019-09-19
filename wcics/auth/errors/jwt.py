# -*- coding: utf-8 -*-

import json, base64

# Thrown when a JWT is formatted wrong
class InvalidJWT(Exception):
  def __init__(self, tkn):
    self.tkn = tkn
    
# Thrown when a JWT has expired
class ExpiredJWT(Exception):
  def __init__(self, tkn):
    _, payload, _ = tkn.split(".")
    payload = json.loads(base64.b64decode(payload + "==").decode('utf-8'))
    
    self.expiry = payload['exp']