# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import assert_login, user
from wcics.auth.update_user import update_user

from wcics.database.models import Users
from wcics.database.utils import db_commit

from wcics.server.consts import ERROR_MESSAGES
from wcics.server.forms import EditProfileForm, flash_form_errors

from flask import flash, redirect, render_template, request

@app.route("/edit-profile/", methods = ["GET", "POST"])
@assert_login
def serve_edit_profile_request():
  form = EditProfileForm(user)
  
  if form.validate_on_submit():
    edit_profile(form)
  
  if form.description.data is None:
    form.description.data = user.desc

  flash_form_errors(form, "Changes were not saved!")
  return render_template("account/edit-profile.html", active = "Account", form = form)

def edit_profile(form):
  kwargs = {}
  
  if form.description.data != user.desc:
    kwargs["desc"] = form.description.data
  
  if form.username.data != user.username:
    kwargs["username"] = form.username.data
  
  if form.email.data != user.email:
    kwargs["email"] = form.email.data
    flash("An email was sent to your new email address to complete the email change!", category = "SUCCESS")
    
  if form.real_name.data != user.real_name:
    kwargs["real_name"] = form.real_name.data
  
  if form.subscribed.data != user.subscribed:
    kwargs["subscribed"] = form.subscribed.data

  update_user(user.id, **kwargs)
  db_commit()
  flash("Successfully updated your user data!", category = "SUCCESS")