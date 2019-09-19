# -*- coding: utf-8 -*-

from wcics.server.forms.validator_sets import *

from flask_wtf import FlaskForm
from wtforms import StringField, SubmitField

class ResetPasswordForm(FlaskForm):
  email = StringField("Email", account_reset_password_email)
  submit = SubmitField("Request Password Reset")