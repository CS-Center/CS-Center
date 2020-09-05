# -*- coding: utf-8 -*-

from wcics.server.forms.validator_sets import *

from flask_wtf import FlaskForm
from wtforms import BooleanField, StringField, SubmitField, TextAreaField

class EditProfileForm(FlaskForm):
  def __init__(self, user):
    FlaskForm.__init__(self, obj = user)

  desc = TextAreaField("Profile Description", account_edit_profile_description)
  username = StringField("Username", account_edit_profile_username)
  real_name = StringField("Real Name", account_edit_profile_real_name)
  email = StringField("Email", account_edit_profile_email)
  subscribed = BooleanField("Send me email notifications of posted events and announcements", account_edit_profile_subscribed)
  submit = SubmitField("Update Profile")
