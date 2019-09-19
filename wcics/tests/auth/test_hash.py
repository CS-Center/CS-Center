# -*- coding: utf-8 -*-

import pytest

from wcics.auth.hash import *

# Ensure that the salt function returns a bytestring of length USER_SALT_LENGTH
def test_new_salt():
  salt = new_salt()
  
  assert len(salt) == USER_SALT_LENGTH
  assert isinstance(salt, bytes)
  
# Ensure that the new_password function returns a string of length RANDOM_PASSWORD_LENGTH with characters between 33 - 127, inclusive
def test_new_password():
  pwd = new_password()
  
  assert len(pwd) == RANDOM_PASSWORD_LENGTH
  assert isinstance(pwd, str)
  
  for c in pwd:
    assert 33 <= ord(c) <= 127
  
# Ensure that the pass_hash function calls the underlying argon2 function
@pytest.mark.parametrize(['pwd', 'salt'], [['abc', b'asdhuisad'], ['sadgy', b'vsyudadas'], ['yfuasduia', b'gyusadbhvuwa']])
def test_hash_interaction(pwd, salt):
  res = pass_hash(pwd, salt)
  assert res == argon2.argon2_hash(pwd, salt)