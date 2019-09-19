# -*- coding: utf-8 -*-

from wcics import app

from flask import render_template, request

import base64

@app.route("/sandbox/markdown/")
def markdown_sandbox():
  try:
    query = base64.b64decode(request.args.get("query", "")).decode("utf-8")
  except:
    query = "" # Ignore padding errors
  return render_template("sandboxes/markdown.html", query = query)