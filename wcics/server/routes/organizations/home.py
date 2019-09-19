# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import organization_page

from wcics.database.models import Organizations

from wcics.utils.url import get_org_id

from flask import render_template

@app.route("/organization/<org>/")
@organization_page
def serve_organization_home(org):
  return render_template("organizations/home.html", active = "Home", organization = Organizations.query.filter_by(id = get_org_id()).first())
