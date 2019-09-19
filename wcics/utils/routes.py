# -*- coding: utf-8 -*-

from wcics.server.consts import DEFAULT_MESSAGES, ERRORS, REPORT_MESSAGES

from flask import render_template

def error_page(code, message = None, reportmessage = None, errorname = None):
  return render_template("utils/error-template.html",
                statcode = code,
                errorname = errorname if errorname is not None else ERRORS.get(code, "Unknown Error"),
                reportmessage = reportmessage if reportmessage is not None else REPORT_MESSAGES.get(code, "If this issue persists"),
                message = message if message is not None else DEFAULT_MESSAGES.get(code, "Something bad happened.")
  )