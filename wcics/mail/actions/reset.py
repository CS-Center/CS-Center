# -*- coding: utf-8 -*-

from ..utils import send_single
from wcics.consts import DOMAIN

# Get directory of messages
import os
messages = os.path.dirname(os.path.dirname(__file__)) + "/messages"

reset_subject = open(messages + "/reset/subject.txt").read()
reset_body = open(messages + "/reset/body.html").read()

# Send a password reset email
def send_reset_email(email, token):
  # Reset message has a few formatting elements:
  ## email -> the user's email
  ## jwt -> the jwt of the action
  
  # Pass the domain for URL creation  
  send_single(email, reset_subject, reset_body.format(
    email = email,
    jwt = token,
    DOMAIN = DOMAIN
  ))