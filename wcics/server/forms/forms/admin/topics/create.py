# -*- coding: utf-8 -*-

from wcics.server.forms.validator_sets import *

from flask_wtf import FlaskForm
from wtforms import StringField, SubmitField, TextAreaField

class TopicSudoCreateForm(FlaskForm):
  tid = StringField("ID", admin_topic_create_tid)
  name = StringField("Name", admin_topic_create_name)
  description = TextAreaField("Description", admin_topic_create_description)
  submit = SubmitField("Create")