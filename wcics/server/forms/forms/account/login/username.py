# -*- coding: utf-8 -*-

from wcics.server.forms.validator_sets import *

from flask_wtf import FlaskForm
from wtforms import PasswordField, StringField, SubmitField

class UsernameLoginForm(FlaskForm):
  username = StringField("Username", account_login_username_username)
  password = PasswordField("Password", account_login_username_password)
  submit = SubmitField("Log In")