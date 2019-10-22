# -*- coding: utf-8 -*-

from wcics.server.forms.validator_sets import *

from flask_wtf import FlaskForm
from wtforms import BooleanField, StringField, SubmitField, TextAreaField

class LessonCreateForm(FlaskForm):
  lid = StringField("ID", admin_lesson_create_nid)
  title = StringField("Title", admin_lesson_create_title)
  desc = TextAreaField("Description", admin_lesson_create_desc)
  body = TextAreaField("Body", admin_lesson_create_body)
  authors = StringField("", admin_lesson_create_authors)
  submit = SubmitField("Create")