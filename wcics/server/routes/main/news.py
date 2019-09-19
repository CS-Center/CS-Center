# -*- coding: utf-8 -*-

from wcics import app

from wcics.database.models import News

from flask import render_template

@app.route("/news/<nid>")
def serveNews(nid):
  return render_template("main/news.html", active = "", data = News.query.filter_by(nid = nid).first_or_404())