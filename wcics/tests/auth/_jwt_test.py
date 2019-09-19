# -*- coding: utf-8 -*-

import pytest

from wcics import app

import jwt

from wcics.auth._jwt import *

testing_payload = dict(a = 3, b = 4, c = 7)

# Make sure that the make_jwt and verify_jwt functions interact correctly
@pytest.mark.parametrize(['key'], [['secret'], ['asgdya'], [app.secret_key]])
def test_interaction(key):  
  tkn = make_jwt(testing_payload, key)
  assert tkn == jwt.encode(testing_payload, key, algorithm = "HS256").decode('utf-8')
  
  payload = verify_jwt(tkn, key)
  assert payload == jwt.decode(tkn, key, algorithms = ["HS256"])