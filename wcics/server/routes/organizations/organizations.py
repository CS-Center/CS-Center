# -*- coding: utf-8 -*-

# -*- coding: utf-8 -*-

from wcics import app, db

from wcics.database.models import Organizations, OrganizationUsers

from wcics.server.consts import ORGANIZATIONS_PER_PAGE
from wcics.server.routes.utils import paged_data

from flask import render_template
from sqlalchemy import func

@app.route("/organizations/")
def serve_organizations():
  page, pages, organizations = paged_data(Organizations.query.filter_by(hidden = False).order_by(db.session.query(func.count(OrganizationUsers.oid))).all(), ORGANIZATIONS_PER_PAGE)
  return render_template("organizations/organizations.html", active = "Organizations", page = page, pages = pages, organizations = organizations)
