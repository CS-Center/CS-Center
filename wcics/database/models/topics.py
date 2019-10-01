# -*- coding: utf-8 -*-

from ..consts.topics import *
from .aliases import *
from .helper import Helper

class topics(dbmodel, Helper):
  id = dbcol(dbint, primary_key = True)
  tid = dbcol(dbstr(TOPIC_ID_MAX_LENGTH), unique = True, nullable = False)
  oid = dbcol(dbint, dbforkey('organizations.id', ondelete = "CASCADE", onupdate = "CASCADE"))
  ptid = dbcol(dbint, dbforkey('topics.id', ondelete = "CASCADE", onupdate = "CASCADE"))
  name = dbcol(dbstr(TOPIC_NAME_MAX_LENGTH), nullable = False, unique = True)
  desc = dbcol(dbstr(TOPIC_DESC_MAX_LENGTH), nullable = False)
  
  @classmethod
  def is_parent(cls, par, ch):
    top = db.session.query(Topics.id, Topics.ptid).filter_by(id = par).cte(recursive = True)
    mid = top.union_all(db.session.query(Topics.id, Topics.ptid).filter_by(ptid = top.c.id))
    res = db.session.query(mid).filter(mid.c.id == ch).first()
    return bool(res)
  
  def __repr__(self):
    return "<topic id=%s parent=%s>" % (self.tid, self.ptid)
  
# Rename alias
Topics = topics