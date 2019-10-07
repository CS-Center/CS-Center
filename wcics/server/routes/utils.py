# -*- coding: utf-8 -*-

from wcics import app, db, md, md_no_mj

from wcics.auth.manage_user import user

from wcics import consts

from wcics.database.models import AttendanceCodes, AttendanceRecords, News, Roles, Users
from wcics.database.models.roles import UserRoles
from wcics.database.models.roles.consts import roles

from wcics.server.consts import DEFAULT_MESSAGES, ERRORS, FLASHES, REPORT_MESSAGES, PAGE_THRESHOLDS
from wcics.server.errors import NotLoggedInError, SessionNotFreshError
from wcics.server.links import links, sudo_links

from wcics.utils.time import get_time, time_delta_display
from wcics.utils.url import get_org_id, get_organization

from flask_misaka import Markup
from flask import json, redirect, render_template, request, Response

import base64, datetime, html, math

from urllib.parse import urlencode

@app.errorhandler(NotLoggedInError)
def redirect_to_login(e):
  return redirect("/login/?next=%s" % request.full_path, code = 303)

@app.errorhandler(SessionNotFreshError)
def redirect_to_reauth(e):
  return redirect("/login/?next=%s&reauth=yes" % request.full_path, code = 303)

@app.template_filter()
def inline_markdown(text):
  return Markup(str(md.render(text)).replace("<p>", "").replace("</p>", ""))

@app.template_filter()
def display_user(user):
  if not user:
    return "<span>user does not exist</span>"
  uid = user if isinstance(user, int) else user.id
  user = Users.query.filter_by(id = uid).first()
  if not user:
    return "<span>user does not exist</span>"
  colorclass = "usercolor-admin" if user.admin else "usercolor-default"
  display = "<span class='%s' href='/user/%d'><b>%s</b></span>" % (colorclass, user.id, html.escape(user.username))
  return display

@app.template_filter()
def display_user_as_link(user):
  if not user:
    return "<span>user does not exist</span>"
  uid = user if isinstance(user, int) else user.id
  return "<a class='unstyled-link' href='/user/%d'>%s</a>" % (uid, display_user(uid))

@app.template_filter()
def english_join(array):
  array = list(array)
  if len(array) == 0:
    return ""
  if len(array) == 1:
    return array[0]
  if len(array) == 2:
    return " and ".join(array)
  return ", ".join(array[:-1]) + ", and " + array[-1]

@app.template_filter()
def attendance_link(org):
  return "<a href=\"/organization/%s/attendance/\">%s</a>" % (org.oid, org.name)

@app.template_global()
def get_user_list(include_all = False):
  if not include_all and (not user or user.roles.users <= UserRoles.default):
    abort(403)
  
  query = db.session.query(Users.username, Users.id)
  if not include_all:
    query = query.filter(Users.id != user.id)
    if not user.admin:
      query = query.join(Roles).filter(Roles.users < UserRoles.moderator)
  users = query.all()
  
  return users

@app.template_global()
def page_button_array(page, pages):
  show = [False] * pages
  if page <= PAGE_THRESHOLDS["merge_left"]:
    for i in range(max(PAGE_THRESHOLDS["merge_left"], page)):
      if 0 <= i < pages:
        show[i] = True
  if page >= pages - PAGE_THRESHOLDS["merge_right"]:
    for i in range(pages - PAGE_THRESHOLDS["merge_right"], pages):
      if 0 <= i < pages:
        show[i] = True
  for i in range(PAGE_THRESHOLDS["edge_left"]):
    if 0 <= i < pages:
      show[i] = True
  for i in range(pages - PAGE_THRESHOLDS["edge_right"], pages):
    if 0 <= i < pages:
      show[i] = True
  for i in range(page - PAGE_THRESHOLDS["show_left"], page + PAGE_THRESHOLDS["show_right"]):
    if 0 < i <= pages:
      show[i - 1] = True
  array = []
  for i in range(pages):
    if show[i]:
      array.append(i + 1)
    elif not array or array[-1] != -1:
      array.append(-1)
  return array

@app.template_global()
def url_with_modifications(**kwargs):
  values = {key: request.args[key] for key in request.args}
  for key in kwargs:
    if kwargs[key] is None:
      if key in values:
        del values[key]
    else:
      values[key] = kwargs[key]
  return request.path + ("?" + urlencode(values) if values else "")

@app.route("/get-users.js")
def serve_user_list_js():
  return Response("var users = %s;" % json.dumps(get_user_list(request.args.get("include_all", "n") == "y")), mimetype = "text/javascript")

@app.route("/article-authors.js")
def serve_article_authors_js():
  return Response("var authors = %s;" % json.dumps(News.query.filter_by(id = request.args.get("article_id", "")).first_or_404().author_ids), mimetype = "text/javascript")

@app.context_processor
def context_processor():
  return {
    "attendance": AttendanceRecords,
    "attendance_codes": AttendanceCodes,
    "datetime": datetime,
    "dtft": datetime.datetime.fromtimestamp,
    "get_org_id": get_org_id,
    "get_organization": get_organization,
    "get_time": get_time,
    "user": user,
    "links": links,
    "sudo_links": sudo_links,
    "roles": roles,
    "time_delta_display": time_delta_display,
    "FLASHES": FLASHES
  }

@app.route("/markdown/", methods = ["POST"])
def convert_markdown():
  return (md if request.json["mathjax"] else md_no_mj).render(request.json["html"])

def paged_data(data, page_size, arg = "page", default_page = 1):
  pages = math.ceil(len(data) / page_size)
  try:
    page = int(request.args.get(arg, default_page))
  except:
    page = default_page
  return (page, pages, data[page * page_size - page_size:page * page_size])