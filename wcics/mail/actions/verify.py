# -*- coding: utf-8 -*-

from ..utils import send_single
from wcics.config.consts import DOMAIN

# Get directory of messages
import os
messages = os.path.dirname(os.path.dirname(__file__)) + "/messages"

verify_subject = open(messages + "/verify/subject.txt").read()
verify_body = open(messages + "/verify/body.html").read()

# Send a verification email
def send_verify_email(email, token, next):
  # Verify message has a few formatting elements:
  ## next -> the next URL after creating the account
  ## jwt -> the jwt of the action

  # Pass the domain for URL creation
  return send_single(email, verify_subject, verify_body.format(
    next = next,
    jwt = token,
    DOMAIN = DOMAIN
  ))
