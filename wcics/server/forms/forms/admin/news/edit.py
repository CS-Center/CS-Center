# -*- coding: utf-8 -*-

from wcics.server.forms.validator_sets import *

from flask_wtf import FlaskForm
from wtforms import BooleanField, StringField, SubmitField, TextAreaField

class NewsSudoEditForm(FlaskForm):
  def __init__(self, article):
    FlaskForm.__init__(self)
    self.assoc_obj = article
    
    self.nid.data = article.nid
    self.title.data = article.title
    self.body.data = article.body

  nid = StringField("ID", admin_news_edit_nid)
  title = StringField("Title", admin_news_edit_title)
  body = TextAreaField("Body", admin_news_edit_body)
  authors = StringField("", admin_news_edit_authors)
  delete = BooleanField("", admin_news_edit_delete)
  submit = SubmitField("Update")