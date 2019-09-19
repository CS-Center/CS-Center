# -*- coding: utf-8 -*-

from wcics.server.forms.validator_sets import *

from flask_wtf import FlaskForm
from wtforms import BooleanField, StringField, PasswordField, SubmitField

class CreateAccountForm(FlaskForm):
  email = StringField("Email", account_signup_create_email)
  username = StringField("Username", account_signup_create_username)
  real_name = StringField("Real Name", account_signup_create_real_name)
  password = PasswordField("Password", account_signup_create_password)
  repeat_password = PasswordField("Repeat Password", account_signup_create_repeat_password)
  legal_agreement = BooleanField(
    "I have read and agree to the <a href='/tos/'>terms of service</a> and <a href='/privacy/'>privacy policy</a>",
    account_signup_create_legal_agreement
  )
  subscribed = BooleanField("Send me email notifications of posted events and announcements", account_signup_create_subscribed)
  submit = SubmitField("Create Account")