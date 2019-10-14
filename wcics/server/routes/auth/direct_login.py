# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.jwt import verify_jwt, ExpiredJWT, InvalidJWT
from wcics.auth.manage_user import set_user, user

from wcics.database.models import Users

from wcics.utils.time import get_time
from wcics.utils.url import get_next_page

from flask import flash, redirect, request

@app.route("/direct-login/")
def direct_login():
  if user:
    return redirect(get_next_page(), code = 303)
  
  try:
    data = verify_jwt(request.args.get("token", ""))
  except ExpiredJWT:
    flash("The password reset token has expired!", category = "ERROR")
    return redirect("/reset-password/", code = 303)
  except InvalidJWT:
    flash("The token provided is invalid!", category = "ERROR")
    return redirect("/reset-password/", code = 303)
  
  set_user(Users.query.filter_by(email = data["email"]).first())
  return redirect(get_next_page(), code = 303)