# -*- coding: utf-8 -*-

from wcics.auth.errors import ExpiredJWT, InvalidJWT
from wcics.auth.jwt import verify_jwt

from wcics.database.models import Users

from wcics.server.consts import ERROR_MESSAGES
from wcics.server.errors import RedirectError

from flask import flash, redirect, request

def get_email_from_token():
  try:
    return verify_jwt(request.args.get("token", ""))["email"]
  except ExpiredJWT:
    flash(ERROR_MESSAGES["creation_token_expired"], category = "ERROR")
    raise RedirectError(redirect("/signup/", code = 303))
  except InvalidJWT:
    flash(ERROR_MESSAGES["creation_token_invalid"], category = "ERROR")
    raise RedirectError(redirect("/signup/", code = 303))