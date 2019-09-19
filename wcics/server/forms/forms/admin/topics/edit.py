# -*- coding: utf-8 -*-

from wcics.server.forms.validator_sets import *

from flask_wtf import FlaskForm
from wtforms import StringField, SubmitField, TextAreaField

class TopicSudoEditForm(FlaskForm):
  def __init__(self, topic):
    FlaskForm.__init__(self)
    self.assoc_obj = topic

  tid = StringField("ID", admin_topic_edit_tid)
  name = StringField("Name", admin_topic_edit_name)
  description = TextAreaField("Description", admin_topic_edit_description)
  submit = SubmitField("Update")