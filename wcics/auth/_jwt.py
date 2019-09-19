# -*- coding: utf-8 -*-
# This file implements the base JWT utils
# Splitting it up enables WCICS custom session to use these utils without needing to import the app
import jwt

from .errors import InvalidJWT, ExpiredJWT

def verify_jwt(tkn, key):
  try:
    # Return the decoded token
    return jwt.decode(tkn, key, algorithms = ["HS256"])
  except jwt.exceptions.ExpiredSignatureError:
    # Expired JWT
    raise ExpiredJWT(tkn)
  except:
    # Raise an error if it is an invalid JWT. 
    raise InvalidJWT(tkn)

# Make a new JWT with the given payload
def make_jwt(payload, key):
  return jwt.encode(payload, key, algorithm = "HS256").decode("utf-8")