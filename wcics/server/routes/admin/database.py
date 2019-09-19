# -*- coding: utf-8 -*-

from wcics import app, db

from wcics.auth.manage_user import assert_login, user

from wcics.database.models.roles import DatabaseRoles

from wcics.server.forms import DatabaseSudoForm

from flask import abort, flash, redirect, render_template, request

import base64, sys

@app.route("/admin/database/", methods = ["GET", "POST"])
@assert_login
def serve_database_sudo():
  if user.roles.database <= DatabaseRoles.default:
    abort(403)
    
  form = DatabaseSudoForm()
  
  data = None
  
  if form.validate_on_submit():
    try:
      result = db.engine.execute(form.query.data)
      columns = [(key, []) for key in result.keys()]
      for row in result:
        for (key, col), val in zip(columns, row):
          col.append(val)
      filtered = [column for column in columns if not all(type(val) == memoryview or val is None for val in column[1])]
      keys = [column[0] for column in filtered]
      zipped = [column[1] for column in filtered]
      data = [keys] + list(map(list, zip(*zipped)))
    except:
      flash(str(sys.exc_info()[1]), category = "ERROR")
      data = None

  try:
    form.query.data = base64.b64decode(request.args.get("query", "")).decode("utf-8")
  except:
    pass # Ignore padding errors
  
  return render_template("adminpages/database.html", sudo = True, active = "database", form = form, data = data)