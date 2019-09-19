# -*- coding: utf-8 -*-

# Interaction testing for wcics.auth.jwt

from wcics import app

from wcics.auth import jwt, _jwt

testing_payload = dict(a = 3, b = 4, c = 7)

def test_interaction():
  # Ensure that the JWT created by _jwt is equivalent to the one created by jwt
  tkn1 = jwt.make_jwt(testing_payload)
  tkn2 = _jwt.make_jwt(testing_payload, app.secret_key)
  
  assert tkn1 == tkn2
  
  # Make sure that all payloads are equivalent
  payload1 = jwt.verify_jwt(tkn1)
  payload2 = _jwt.verify_jwt(tkn2, app.secret_key)
  
  assert payload1 == payload2