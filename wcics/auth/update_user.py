# -*- coding: utf-8 -*-

import sys

from wcics import app, logger

from wcics.auth.hash import pass_hash
from wcics.auth.manage_user import set_user

from wcics.database.models import Users
from wcics.database.utils import db_commit, db_rollback

from wcics.utils.time import get_time

# Throws NoSuchUser
def update_user(uid, **kwargs):
  user = Users.query.filter_by(id = uid).first()
    
  # TODO handle email change
  
  if "username" in kwargs:
    user.username = kwargs.pop("username")
  
  if "desc" in kwargs:
    user.desc = kwargs.pop("desc")
  
  if "real_name" in kwargs:
    user.real_name = kwargs.pop("real_name")
  
  if "npass" in kwargs:
    user.password_hash = pass_hash(kwargs.pop("npass"), user.salt)
    user.revoke_tokens_before = get_time()
    set_user(user)
  
  if "subscribed" in kwargs:
    user.subscribed = kwargs.pop("subscribed")

  if kwargs:
    logger.warn("WARNING update_user not complete: %s" % kwargs)
  
  db_commit()