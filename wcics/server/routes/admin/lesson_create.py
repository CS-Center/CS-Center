from wcics import app

from wcics.database.models import Organizations, Lessons, LessonAuthors

from wcics.database.models.roles.consts import LessonRoles

from wcics.database.utils import db_commit

from wcics.server.forms import LessonCreateForm, flash_form_errors

from wcics.auth.manage_user import user, organization_page, assert_login

from flask import abort, render_template, flash, redirect

@app.route("/organization/<org>/admin/lesson-create/", methods = ["GET", "POST"])
@organization_page
@assert_login
def serve_lesson_create_request(org):
  if user.organization_roles.lessons <= LessonRoles.default:
    abort(403)
    
  form = LessonCreateForm()
  
  if form.validate_on_submit():
    flash("Successfully created lesson item!", category = "SUCCESS")
    lesson_admin_create(form, org)
    return redirect("/organization/%s/admin/lessons/" % org, code = 303)
  else:
    flash_form_errors(form)
    
  return render_template("adminpages/lesson-create.html", sudo = True, active = "lessons", form = form)

def lesson_admin_create(form, oid):
  org = Organizations.query.filter_by(oid = oid).first()
  
  article = Lessons.add(oid = org.id, lid = form.lid.data, title = form.title.data, desc = form.desc.data, body = form.body.data)
  db_commit()
  
  for uid in form.authors.data.split():
    LessonAuthors.add(lid = article.id, uid = int(uid), oid = org.id)
    
  db_commit()