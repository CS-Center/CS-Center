# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.create_account import create_account
from wcics.auth.errors import ExpiredJWT, InvalidJWT
from wcics.auth.jwt import make_jwt, verify_jwt
from wcics.auth.manage_user import set_user

from wcics.database.models import Users

from wcics.server.consts import ERROR_MESSAGES
from wcics.server.errors import RedirectError
from wcics.server.forms import CreateAccountForm, flash_form_errors

from wcics.utils.tokens import get_email_from_token
from wcics.utils.url import get_next_page

from flask import flash, redirect, render_template, request

def get_email_from_token():
  try:
    return verify_jwt(request.args.get("token", ""))["email"]
  except ExpiredJWT:
    flash(ERROR_MESSAGES["creation_token_expired"], category = "ERROR")
    raise RedirectError(redirect("/signup/", code = 303))
  except InvalidJWT:
    flash(ERROR_MESSAGES["creation_token_invalid"], category = "ERROR")
    raise RedirectError(redirect("/signup/", code = 303))

@app.route("/create-account/", methods = ["GET", "POST"])
def serve_create_account_request():
  try:
    email = get_email_from_token()
  except RedirectError as e:
    return e.response

  user = Users.query.filter_by(email = email).first()

  if user:
      set_user(user)
      flash("Welcome back! This email address already owns an account. If you wish to add/change your password, go to the Edit Profile page.", category = "SUCCESS")
      return redirect(get_next_page(), code = 303)

  form = CreateAccountForm()

  if form.validate_on_submit():
    return serve_create_account(form)

  form.legal_agreement.checked = False

  flash_form_errors(form)
  return render_template("account/create-account.html", active = "Sign Up", form = form, email = email, next_page = get_next_page())

def serve_create_account(form):
  try:
    email = get_email_from_token()
  except RedirectError as e:
    return e.response

  username = form.username.data
  real_name = form.real_name.data
  password = form.password.data
  subscribed = form.subscribed.data

  user = create_account(email, username, real_name, password, subscribed)
  flash("Your new account has been created. Welcome to CS Center!", category = "SUCCESS")
  set_user(user)
  return redirect(get_next_page(), code = 303)
