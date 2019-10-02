# -*- coding: utf-8 -*-

from .aliases import *
from .helper import Helper
from ..consts.lessons import LESSON_ID_MAX_LENGTH, LESSON_NAME_MAX_LENGTH, LESSON_BODY_MAX_LENGTH, LESSON_DESC_MAX_LENGTH
from .topics import Topics
from .users import Users

from wcics.utils.time import get_time

class lessons(dbmodel, Helper):
  id = dbcol(dbint, primary_key = True)
  lid = dbcol(dbstr(LESSON_ID_MAX_LENGTH), unique = True, nullable = False)
  oid = dbcol(dbint, dbforkey('organizations.id', ondelete = "CASCADE", onupdate = "CASCADE"))
  name = dbcol(dbstr(LESSON_NAME_MAX_LENGTH), nullable = False)
  desc = dbcol(dbstr(LESSON_DESC_MAX_LENGTH), nullable = False)
  body = dbcol(dbstr(LESSON_BODY_MAX_LENGTH), nullable = False)
  create_time = dbcol(dbint, default = get_time, nullable = False)
  
  @property
  def authors(self):
    return Users.query.join(LessonAuthors).filter(LessonAuthors.lid == self.id).all()
  
class lesson_topics(dbmodel, Helper):
  lid = dbcol(dbint, dbforkey(lessons.id, onupdate = "CASCADE", ondelete = "CASCADE"), primary_key = True)
  tid = dbcol(dbint, dbforkey(Topics.id, onupdate = "CASCADE", ondelete = "CASCADE"), primary_key = True)
  
class lesson_authors(dbmodel, Helper):
  lid = dbcol(dbint, dbforkey(lessons.id, onupdate = "CASCADE", ondelete = "CASCADE"), primary_key = True)
  uid = dbcol(dbint, dbforkey(Users.id, onupdate = "CASCADE", ondelete = "CASCADE"), primary_key = True)
  
Lessons = lessons
LessonTopics = lesson_topics
LessonAuthors = lesson_authors