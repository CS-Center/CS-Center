# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import assert_login, organization_page, user

from wcics.database.models import Lessons, LessonAuthors, Organizations, OrganizationUsers
from wcics.database.models.roles import LessonRoles, OrganizationRoles

from wcics.utils.url import get_organization, get_org_id

from flask import abort, redirect, render_template

@app.route("/admin/lessons/")
@assert_login
def serve_lesson_admin_home():  
  links = [(organization.oid, organization.name) for organization in user.lesson_admin_organizations()]
  
  if len(links) == 0:
    abort(403)
  
  return render_template("adminpages/lesson-home.html", sudo = True, active = "lessons", links = links)

@app.route("/organization/<org>/admin/lessons/")
@organization_page
@assert_login
def serve_lesson_organization(org):
  if user.organization_roles.lessons <= LessonRoles.default:
    abort(403)
  
  return render_template("adminpages/lessons.html", 
                         sudo = True, active = "lessons", 
                         oid = get_organization(), 
                         news = Lessons.query.filter_by(oid = get_org_id()).order_by(Lessons.create_time.desc()).all())