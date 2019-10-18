# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import set_user, user

from wcics.auth.hash import pass_hash

from wcics.database.models import Users

from wcics.server.consts import ERROR_MESSAGES
from wcics.server.forms import EmailLoginForm, UsernameLoginForm, flash_form_errors

from wcics.utils.url import get_next_page

from flask import flash, redirect, render_template, request

@app.route("/login/", methods = ["GET", "POST"])
def serve_login_request():
  reauth = request.args.get("reauth", "") == "yes"
  if user and not reauth:
    return redirect(get_next_page(), code = 303)

  use_username = request.args.get("id", "username") == "username"
  form = UsernameLoginForm() if use_username else EmailLoginForm()
  
  if form.validate_on_submit():
    return serve_login(form, use_username, reauth)
  else:
    flash_form_errors(form)
    return serve_login_page(form, use_username, reauth)
  
def serve_login_page(form, use_username, reauth):
  return render_template(
    "account/login.html",
    active = "Log In",
    form = form,
    use_username = use_username,
    username = form.username.data if use_username else "",
    email = "" if use_username else form.email.data,
    next_page = get_next_page(),
    reauth = reauth
  )

def serve_login(form, use_username, reauth):
  if use_username:
    user = Users.query.filter_by(username = form.username.data).first()
  else:
    user = Users.query.filter_by(email = form.email.data).first()
  
  if not reauth:
    flash("Welcome back!", category = "SUCCESS")
  set_user(user)
  return redirect(get_next_page(), code = 303)
