# -*- coding: utf-8 -*-

from flask import render_template

from wcics import app

from wcics.auth.manage_user import organization_page
from wcics.server.routes.utils import paged_data

from wcics.server.consts import LESSONS_PER_PAGE

from wcics.utils.url import get_org_id

from wcics.database.models import Organizations, Lessons

from wcics.utils.routes import error_page

@app.route("/lessons/")
@app.route("/organization/<oid>/lessons/")
@organization_page
def get_lessons(oid = "main"):
  org = Organizations.query.filter_by(oid = oid).first()
  
  page, pages, lessons = paged_data(Lessons.query.filter_by(oid = org.id).order_by(Lessons.name).all(), LESSONS_PER_PAGE)
  
  return render_template("learn/lessons.html", lessons = lessons, organization = org, active = "Learn", page = page, pages = pages)

@app.route("/lesson/<lid>/")
@app.route("/organization/<oid>/lesson/<lid>/")
@organization_page
def serve_lesson(lid, oid = "main"):
  lesson = Lessons.query.filter_by(lid = lid).join(Organizations).filter(Organizations.oid == oid).first()
  
  if lesson is None:
    return error_page(404, message = "No such lesson with code '%s'." % lid)
    
  return render_template("learn/lesson.html", lesson = lesson, active = "Learn")