# -*- coding: utf-8 -*-

from wcics.server.forms.validator_sets import *

from flask_wtf import FlaskForm
from wtforms import BooleanField, StringField, SubmitField, TextAreaField

class NewsSudoCreateForm(FlaskForm):
  nid = StringField("ID", admin_news_create_nid)
  title = StringField("Title", admin_news_create_title)
  body = TextAreaField("Body", admin_news_create_body)
  authors = StringField("", admin_news_create_authors)
  email = BooleanField("Send email notifications", admin_news_create_email)
  submit = SubmitField("Post")