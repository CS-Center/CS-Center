# -*- coding: utf-8 -*-

from .hash import new_salt, pass_hash

from wcics.database.models import Users
from wcics.database.utils import db_commit

def create_blank_account(email, username, real_name, subscribed):
  new_user = Users.add(email = email, username = username, real_name = real_name, subscribed = subscribed, salt = new_salt())
  db_commit()
  new_user.create_dependencies()
  return new_user

# Creates a new account
def create_account(email, username, real_name, password, subscribed):
  new_user = create_blank_account(email, username, real_name, subscribed)
  new_user.password_hash = pass_hash(password, new_user.salt)
  db_commit()
  return new_user