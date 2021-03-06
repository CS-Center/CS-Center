# -*- coding: utf-8 -*-

from .aliases import *
from .helper import Helper
from ..consts.lessons import LESSON_ID_MAX_LENGTH, LESSON_TITLE_MAX_LENGTH, LESSON_BODY_MAX_LENGTH, LESSON_DESC_MAX_LENGTH
from .topics import Topics

from wcics.utils.time import get_time

class lessons(dbmodel, Helper):
  id = dbcol(dbint, primary_key = True)
  lid = dbcol(dbstr(LESSON_ID_MAX_LENGTH), unique = True, nullable = False)
  oid = dbcol(dbint, dbforkey('organizations.id', ondelete = "CASCADE", onupdate = "CASCADE"))
  title = dbcol(dbstr(LESSON_TITLE_MAX_LENGTH), nullable = False)
  desc = dbcol(dbstr(LESSON_DESC_MAX_LENGTH), nullable = False)
  body = dbcol(dbstr(LESSON_BODY_MAX_LENGTH), nullable = False)
  create_time = dbcol(dbint, default = get_time, nullable = False)
  
  def has_author(self, uid):
    return LessonAuthors.query.filter_by(lid = self.id, uid = uid).count() > 0
  
  @property
  def authors(self):
    from .users import Users
    # importing inside the function to avoid a circular import at the top
    # the alternative is worse, which uses a private method and key access to the name directly
    return Users.query.join(LessonAuthors).filter(LessonAuthors.lid == self.id).all()
 
  @property
  def author_ids(self):
    return [uid for (uid,) in db.session.query(LessonAuthors.uid).filter_by(lid = self.id).all()]
  
  @property
  def topics(self):
    return LessonTopics.query.filter_by(lid = self.id).all()
  
class lesson_topics(dbmodel, Helper):
  lid = dbcol(dbint, dbforkey(lessons.id, onupdate = "CASCADE", ondelete = "CASCADE"), primary_key = True)
  tid = dbcol(dbint, dbforkey(Topics.id, onupdate = "CASCADE", ondelete = "CASCADE"), primary_key = True)
  
class lesson_authors(dbmodel, Helper):
  lid = dbcol(dbint, dbforkey(lessons.id, onupdate = "CASCADE", ondelete = "CASCADE"), primary_key = True)
  uid = dbcol(dbint, dbforkey('users.id', onupdate = "CASCADE", ondelete = "CASCADE"), primary_key = True)
  oid = dbcol(dbint, dbforkey('organizations.id', onupdate = "CASCADE", ondelete = "CASCADE"), nullable = False)
  
  __table_args__ = (db.ForeignKeyConstraint(("uid", "oid"), ("organization_users.uid", "organization_users.oid")), )
  
Lessons = lessons
LessonTopics = lesson_topics
LessonAuthors = lesson_authors