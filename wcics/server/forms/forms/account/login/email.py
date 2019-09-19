# -*- coding: utf-8 -*-

from wcics.server.forms.validator_sets import *

from flask_wtf import FlaskForm
from wtforms import PasswordField, StringField, SubmitField

class EmailLoginForm(FlaskForm):
  email = StringField("Email", account_login_email_email)
  password = PasswordField("Password", account_login_email_password)
  submit = SubmitField("Log In")