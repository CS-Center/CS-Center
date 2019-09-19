# -*- coding: utf-8 -*-

from wcics.server.forms.validator_sets import *

from flask_wtf import FlaskForm
from wtforms import StringField, SubmitField

class OrganizationJoinByCodeForm(FlaskForm):
  join_code = StringField("Join Code", organizations_join_by_code_join_code)
  submit = SubmitField("Join Organization")