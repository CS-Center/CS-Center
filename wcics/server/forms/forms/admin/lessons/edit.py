# -*- coding: utf-8 -*-

from wcics.server.forms.validator_sets import *

from flask_wtf import FlaskForm
from wtforms import BooleanField, StringField, SubmitField, TextAreaField

class LessonEditForm(FlaskForm):
  def __init__(self, lesson):
    FlaskForm.__init__(self)
    self.assoc_obj = lesson
    
    self.lid.data = lesson.lid
    self.title.data = lesson.title
    self.desc.data = lesson.desc
    self.body.data = lesson.body

  lid = StringField("ID", admin_lesson_edit_nid)
  title = StringField("Title", admin_lesson_edit_title)
  desc = TextAreaField("Description", admin_lesson_edit_desc)
  body = TextAreaField("Body", admin_lesson_edit_body)
  authors = StringField("", admin_lesson_edit_authors)
  delete = BooleanField("", admin_lesson_edit_delete)
  submit = SubmitField("Update")