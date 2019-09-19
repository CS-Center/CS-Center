# -*- coding: utf-8 -*-

from wcics.server.forms.validator_sets import *

from flask_wtf import FlaskForm
from wtforms import PasswordField, SubmitField

class ChangePasswordForm(FlaskForm):
  password = PasswordField("New Password", account_change_password_password)
  repeat_password = PasswordField("Repeat Password", account_change_password_repeat_password)
  submit = SubmitField("Change Password")