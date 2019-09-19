# -*- coding: utf-8 -*-

from .consts import RANDOM_PASSWORD_LENGTH

from wcics.database.consts import USER_SALT_LENGTH

# argon2 is our hashing algorithm
import argon2

# Import a secure random generator
from random import SystemRandom

Crytopgen = SystemRandom()

# Get a new salt
def new_salt():
  return bytes(Crytopgen.randint(0, 255) for _ in range(USER_SALT_LENGTH))

# Get a new random password
def new_password():
  return "".join(chr(Crytopgen.randint(33, 127)) for _ in range(RANDOM_PASSWORD_LENGTH))

# Hash a password with the given salt. 
def pass_hash(password, salt):
  return argon2.argon2_hash(password, salt)