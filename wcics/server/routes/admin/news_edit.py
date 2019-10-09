# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import assert_login, organization_page, user

from wcics.database.models import News, NewsAuthors, Users
from wcics.database.models.roles import NewsRoles
from wcics.database.utils import db_commit

from wcics.server.forms import NewsSudoEditForm, flash_form_errors

from wcics.utils.routes import error_page
from wcics.utils.url import get_org_id

from flask import abort, flash, redirect, render_template

@app.route("/organization/<org>/admin/news-edit/<nid>", methods = ["GET", "POST"])
@organization_page
@assert_login
def serve_news_sudo_edit_request(org, nid):
  article = News.query.filter_by(oid = get_org_id(), nid = nid).first()
  
  if not article:
    return error_page(404, "There is no news item with this ID.")

  if not (user.organization_roles.news >= NewsRoles.moderator or user.organization_roles.news >= NewsRoles.default and article.has_author(user.id)):
    abort(403)

  form = NewsSudoEditForm(article)
  
  if form.nid.data is None:
    form.nid.data = article.nid
  
  if form.title.data is None:
    form.title.data = article.title
  
  if form.body.data is None:
    form.body.data = article.body
    
  if form.validate_on_submit():
    if news_sudo_edit(article, form):
      flash("Successfully deleted news item!", category = "SUCCESS")
      return redirect("/admin/news/", code = 303)
    flash("Successfully updated news item!", category = "SUCCESS")
  else:
    flash_form_errors(form, "Changes not were saved!")
  
  return render_template("adminpages/news-edit.html", sudo = True, active = "news", article = article, form = form)

def news_sudo_edit(article, form):
  if form.delete.data:
    News.remove(article)
    db_commit()
    return True
  
  article.title = form.title.data
  article.body = form.body.data
    
  if user.organization_roles.news >= NewsRoles.moderator:
    authors = list(map(int, form.authors.data.split()))

    for news_author in NewsAuthors.query.filter_by(nid = article.id).all():
      if news_author.uid not in authors:
        NewsAuthors.remove(news_author)
      else:
        authors.remove(news_author.uid)

    for new_author in authors:
      NewsAuthors.add(nid = article.id, uid = new_author)

  db_commit()