# -*- coding: utf-8 -*-

# -*- coding: utf-8 -*-

from wcics import app, db

from wcics.auth.manage_user import user

from wcics.database.models import Organizations
from wcics.database.utils import db_commit

from wcics.server.forms import OrganizationJoinByCodeForm, flash_form_errors

from wcics.utils.url import get_org_id

from flask import flash, render_template, request

from sqlalchemy import func

@app.route("/organization/<org>/landing/", methods = ["GET", "POST"])
def serve_organization_landing(org):
  form = OrganizationJoinByCodeForm()
  organization = Organizations.query.filter_by(id = get_org_id()).first()
  
  if form.validate_on_submit():
    organization.add_user(user)
    db_commit()
    
    flash("Joined %s!" % organization.name, category = "SUCCESS")
    
  flash_form_errors(form)
  
  return render_template("organizations/landing.html", active = "Organizations", organization = organization, form = form)
