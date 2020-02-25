# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import organization_page

from wcics.database.models import News, Organizations

from wcics.utils.url import get_org_id

from flask import render_template

@app.route("/news/<nid>")
def serve_main_news(nid):
  return serve_news("main", nid)

@app.route("/organization/<oid>/news/<nid>")
@organization_page
def serve_news(oid, nid):
  return render_template("main/news.html", active = "", data = News.query.filter_by(oid = get_org_id(), nid = nid).first_or_404())