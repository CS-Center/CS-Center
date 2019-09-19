# -*- coding: utf-8 -*-

from wcics.server.forms.validator_sets import *

from flask_wtf import FlaskForm
from wtforms import BooleanField, StringField, SubmitField, TextAreaField

class OrganizationSudoForm(FlaskForm):
  name = StringField("Name", admin_organization_name)
  description = TextAreaField("Description", admin_organization_description)
  join_code = StringField("Join Code", admin_organization_join_code)
  can_join_code = BooleanField("Allow Join by Code", admin_organization_can_join_code)
  can_apply = BooleanField("Allow Request to Join", admin_organization_can_apply)
  submit = SubmitField("Update Organization")