# -*- coding: utf-8 -*-

from wcics import app, db, md

from wcics.auth.manage_user import assert_login, organization_page, user

from wcics.database.models import News, NewsAuthors, Organizations, Users
from wcics.database.models.roles import NewsRoles
from wcics.database.utils import db_commit

from wcics.mail.utils import send_many

from wcics.server.forms import NewsSudoCreateForm, flash_form_errors

from wcics.utils.time import get_time
from wcics.utils.url import get_org_id

from flask import abort, flash, redirect, render_template

import time

@app.route("/organization/<org>/admin/news-create/", methods = ["GET", "POST"])
@organization_page
@assert_login
def serve_news_sudo_create_request(org):
  if user.organization_roles.news <= NewsRoles.default:
    abort(403)

  form = NewsSudoCreateForm()
    
  if form.validate_on_submit():
    flash("Successfully created news item!", category = "SUCCESS")
    news_sudo_create(form)
    return redirect("/organization/%s/admin/news/" % org, code = 303)
  else:
    flash_form_errors(form)
  
  return render_template("adminpages/news-create.html", sudo = True, active = "news", form = form)

def news_sudo_create(form):
  article = News.add(oid = get_org_id(), nid = form.nid.data, title = form.title.data, body = form.body.data, time = get_time())
  db_commit()
  
  for uid in form.authors.data.split():
    NewsAuthors.add(nid = article.id, uid = int(uid))
  
  if form.email.data:
    co = Organizations.query.filter_by(id = get_org_id())
    send_many([tup[0] for tup in db.session.query(Users.email).filter(Users.subscribed == True).all()], "%s Announcement - %s" % ("CS Center" if org.id == 1 else org.name, form.title.data), md.render(form.body.data))
  
  db_commit()