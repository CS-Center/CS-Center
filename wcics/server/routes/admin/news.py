# -*- coding: utf-8 -*-

from wcics import app, db

from wcics.auth.manage_user import assert_login, organization_page, user

from wcics.database.models import News, NewsAuthors, Organizations, OrganizationUsers
from wcics.database.models.roles import NewsRoles, OrganizationRoles

from wcics.utils.url import get_organization, get_org_id

from flask import abort, redirect, render_template

@app.route("/admin/news/")
@assert_login
def serve_news_sudo_home():  
  links = [(organization.oid, organization.name) for organization in user.news_admin_organizations()]
  
  if len(links) == 0:
    abort(403)
  
  return render_template("adminpages/news-home.html", sudo = True, active = "news", links = links)

@app.route("/organization/<org>/admin/news/")
@organization_page
@assert_login
def serve_news_sudo(org):
  if user.organization_roles.news <= NewsRoles.default:
    abort(403)
  
  return render_template("adminpages/news.html", 
                         sudo = True, 
                         active = "news", 
                         oid = get_organization(), 
                         news = News.query.filter_by(oid = get_org_id()).order_by(News.time.desc()).all())