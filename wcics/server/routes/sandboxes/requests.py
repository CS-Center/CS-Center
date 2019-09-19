# -*- coding: utf-8 -*-

from wcics import app

from flask import render_template

@app.route("/sandbox/requests/")
def requests_sandbox():
  return render_template("sandboxes/requests.html")