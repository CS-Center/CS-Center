# -*- coding: utf-8 -*-

from ..utils import send_single
from wcics.consts import DOMAIN

# Get directory of messages
import os
messages = os.path.dirname(os.path.dirname(__file__)) + "/messages"

reset_subject = open(messages + "/link/subject.txt").read()
reset_body = open(messages + "/link/body.html").read()

# Send a password reset email
def send_link_email(email, token, next):
  # Reset message has a few formatting elements:
  ## email -> the user's email
  ## jwt -> the jwt of the action
  ## next -> the next page
  
  # Pass the domain for URL creation  
  return send_single(email, reset_subject, reset_body.format(
    email = email,
    jwt = token,
    next = next,
    DOMAIN = DOMAIN
  ))