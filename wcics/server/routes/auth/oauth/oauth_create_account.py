# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.create_account import create_blank_account
from wcics.auth.jwt import verify_jwt, InvalidJWT, ExpiredJWT
from wcics.auth.manage_user import set_user

from wcics.database.models import GoogleLinks, GithubLinks
from wcics.database.utils import db_commit

from wcics.server.forms import OAuthCreateAccountForm, flash_form_errors

from wcics.utils.url import get_next_page
from wcics.utils.routes import error_page

from flask import abort, flash, redirect, render_template, request

@app.route("/oauth-create-account/", methods = ["GET", "POST"])
def oauth_create_account():
  try:
    data = verify_jwt(request.args.get("token", ""))
  except (InvalidJWT, ExpiredJWT):
    return error_page(code = 400, message = "Invalid token in request. Please contact us.", errorname = "Bad Request")
    
  form = OAuthCreateAccountForm()

  if form.email.data is None and "email" in data:
    form.email.data = data["email"]

  if form.username.data is None and "username" in data:
    form.username.data = data["username"]

  if form.real_name.data is None and "real_name" in data:
    form.real_name.data = data["real_name"]

  if form.validate_on_submit():
    user = create_blank_account(form.email.data, form.username.data, form.real_name.data, form.subscribed.data)

    if data["provider"] == "Google":
      GoogleLinks.add(uid = user.id, gid = data["pid"])
    elif data["provider"] == "GitHub":
      GithubLinks.add(uid = user.id, gid = data["pid"])

    db_commit()

    set_user(user)

    flash("Welcome!", category = "SUCCESS")
    return redirect(get_next_page(), code = 303)

  flash_form_errors(form)

  form.legal_agreement.checked = False

  return render_template("account/oauth-create-account.html", active = "Sign Up", form = form, next_page = get_next_page(), provider = data["provider"])
