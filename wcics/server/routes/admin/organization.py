# -*- coding: utf-8 -*-

from wcics import app, consts, db

from wcics.auth.manage_user import assert_login, organization_page, user

from wcics.consts import KEYS_FOLDER_PATH

from wcics.database.models.organizations import Organizations, OrganizationInvites
from wcics.database.models.roles import OrganizationManagerRoles
from wcics.database.utils import db_commit

from wcics.server.forms import OrganizationSudoForm, flash_form_errors

from wcics.utils.files import write_file
from wcics.utils.url import get_org_id

from flask import abort, flash, redirect, render_template, request

import json

@app.route("/organization/<org>/admin/organization/", methods = ["GET", "POST"])
@organization_page
@assert_login
def serve_organization_sudo(org):
  if user.organization_roles.admin < OrganizationManagerRoles.admin:
    abort(403)

  form = OrganizationSudoForm()
  
  organization = Organizations.query.filter_by(id = get_org_id()).first()
  
  if form.validate_on_submit():
    organization.name = form.name.data
    organization.desc = form.description.data
    organization.join_code = form.join_code.data
    organization.can_join_code = form.can_join_code.data
    organization.can_apply = form.can_apply.data
    
    db_commit()
    
    flash("Successfully updated organization!", category = "SUCCESS")
  
  if form.name.data is None:
    form.name.data = organization.name

  if form.description.data is None:
    form.description.data = organization.desc
  
  if form.join_code.data is None:
    form.join_code.data = organization.join_code
  
  form.can_join_code.data = organization.can_join_code
  form.can_apply.data = organization.can_apply
  
  flash_form_errors(form)
  
  return render_template("adminpages/organization.html", sudo = True, active = "organization", form = form)