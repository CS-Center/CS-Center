from wcics import app

from wcics.auth.manage_user import assert_login, organization_page, user

from wcics.database.models.roles.consts import LessonRoles

from wcics.database.models import Lessons, LessonAuthors

from wcics.database.utils import db_commit

from wcics.server.forms import LessonEditForm, flash_form_errors

from wcics.utils.routes import error_page
from wcics.utils.url import get_org_id

from flask import abort, flash, redirect, render_template

@app.route("/organization/<org>/admin/lesson-edit/<int:id>", methods = ["GET", "POST"])
@organization_page
@assert_login
def serve_lesson_edit(org, id):
  lesson = Lessons.query.filter_by(oid = get_org_id(), id = id).first()
  
  if not lesson:
    return error_page(404, "There is no lesson with the ID %d." % id)
  
  if not (user.organization_roles.lessons >= LessonRoles.admin or user.organization_roles.lessons >= LessonRoles.default and lesson.has_author(user.id)):
    abort(403)
    
  form = LessonEditForm(lesson)
  
  if form.validate_on_submit():
    if lesson_edit(lesson, form):
      flash("Successfully deleted lesson!", category = "SUCCESS")
      return redirect("/organization/%s/admin/lessons/" % org, code = 303)
    
    flash("Successfully updated lesson!", category = "SUCCESS")
  else:
    flash_form_errors(form, "Changes were not saved!")
  
  return render_template("adminpages/lesson-edit.html", sudo = True, active = "lessons", lesson = lesson, form = form)

def lesson_edit(lesson, form):
  if form.delete.data:
    Lessons.remove(lesson)
    db_commit()
    return True
  
  lesson.desc = form.desc.data
  lesson.title = form.title.data
  lesson.body = form.body.data
  lesson.lid = form.lid.data
  
  print(lesson.lid)
    
  if user.organization_roles.lessons >= LessonRoles.admin:
    authors = list(map(int, form.authors.data.split()))

    for lesson_author in LessonAuthors.query.filter_by(lid = lesson.id).all():
      if lesson_author.uid not in authors:
        LessonAuthors.remove(lesson_author)
      else:
        authors.remove(lesson_author.uid)
         
    for new_author in authors:
      LessonAuthors.add(lid = lesson.id, uid = new_author, oid = get_org_id())

  db_commit()