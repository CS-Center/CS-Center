# -*- coding: utf-8 -*-

from flask import render_template

from wcics import app

from wcics.auth.manage_user import organization_page

from wcics.database.models import Organizations

from wcics.utils.indexed import indexed_url

#@app.route("/learn/")
#@app.route("/organization/<oid>/learn/")
#@organization_page
def learnpage(oid = "main"):
  org = Organizations.query.filter_by(oid = oid).first()
  
  return render_template("learn/learn.html", org = org, idx = indexed_url)
