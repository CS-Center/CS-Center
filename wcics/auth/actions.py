# -*- coding: utf-8 -*-

from .consts import ACTION_EXPIRY, SIGNUP_EXPIRY
from .errors import InvalidActionPayload, ActionEmailChange, InvalidActionType
from .hash import pass_hash, new_password
from .jwt import verify_jwt, make_jwt

from wcics.database.models import Users
from wcics.database.utils import db_commit

from wcics.mail.actions import send_link_email, send_reset_email, send_verify_email

from wcics.utils.time import get_time

# Verify a user's email for signup
# Throws MailError
def verify_email(email, next):
  token = make_jwt(dict(
    exp = get_time() + SIGNUP_EXPIRY,
    email = email
  ))

  send_verify_email(email, token, next)
  
# Reset a user's password
# Throws NoSuchUser
# Throws MailError
def reset_password(email):
  user = Users.query.filter_by(email = email).first()
  
  if not user:
    return
  
  token = make_jwt(dict(
    exp = get_time() + ACTION_EXPIRY,
    uid = user.id,
    email = email
  ))
  
  send_reset_email(email, token)

# Direct login, tell the user to link
def link_email(email, next):
  user = Users.query.filter_by(email = email).first()
  
  if not user:
    return
  
  token = make_jwt(dict(
    exp = get_time() + SIGNUP_EXPIRY,
    uid = user.id,
    email = email
  ))
  
  send_link_email(email, token, next)