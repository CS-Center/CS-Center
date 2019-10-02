# -*- coding: utf-8 -*-

from wcics import app

from wcics.auth.manage_user import organization_page

from wcics.consts import CONFIG_FOLDER_PATH, STATIC_FOLDER_PATH

from wcics.database.models import News, Organizations

from wcics.server.consts import NEWS_PER_PAGE
from wcics.server.routes.utils import paged_data

from wcics.utils.files import load_file
from wcics.utils.url import get_org_id
from wcics.utils.indexed import indexed_url

from flask import redirect, render_template, request, send_file

from math import ceil

@app.route("/o/<org>/")
@app.route("/org/<org>/")
def org_home_redirect(org):
  return redirect("/organization/%s/" % org, code = 303)

@app.route("/o/<org>/<path:route>")
@app.route("/org/<org>/<path:route>")
def organization_redirects(org, route):
  return redirect("/organization/%s/%s" % (org, route), code = 303)

@app.route("/organization/<org>/")
@organization_page
def serve_org_home(org):
  page, pages, news = paged_data(News.query.filter_by(oid = get_org_id()).order_by(News.time.desc()).all(), NEWS_PER_PAGE)
  return render_template("main/index.html", active = "Home", page = page, pages = pages, news = news, organization = Organizations.query.filter_by(id = get_org_id()).first())

@app.route("/")
def serve_main_home():
  return serve_org_home("main")

@app.route("/about/")
def serve_about():
  return render_template("main/about.html", active = "About")

@app.route("/calendar/")
def serve_calendar():
  return render_template("main/calendar.html", active = "Calendar")

@app.route("/contact/")
def serve_contact():
  return render_template("main/contact.html", active = "About")

@app.route("/external/")
def serve_external():
  return render_template("main/external.html", active = "", url = request.args.get("url", "/"))

@app.route("/faq/")
def serve_faq():
  return render_template("main/faq.html", active = "About")

@app.route("/privacy/")
def serve_privacy():
  return render_template("main/privacy.html", active = "")

@app.route("/tos/")
def serve_terms():
  return render_template("main/tos.html", active = "")

@app.route("/robots.txt")
def serve_robots():
  return load_file(CONFIG_FOLDER_PATH + "/robots.txt")

# Temporary redirects here are a must, otherwise the route would hardcode to a location which would be BAD
@app.route("/alticon.ico")
def serve_alticon():
  return redirect(indexed_url("/static/ico/alticon.ico")[0], code = 303)

@app.route("/favicon.ico")
def serve_favicon():
  return redirect(indexed_url("/static/ico/favicon.ico")[0], code = 303)
