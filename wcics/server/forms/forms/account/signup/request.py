# -*- coding: utf-8 -*-

from wcics.server.forms.validator_sets import *

from flask_wtf import FlaskForm
from wtforms import StringField, SubmitField

class SignupForm(FlaskForm):
  email = StringField("Email", account_signup_request_email)
  submit = SubmitField("Sign Up")