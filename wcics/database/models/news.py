# -*- coding: utf-8 -*-

from .aliases import *
from .helper import Helper
from ..consts.news import NEWS_ID_MAX_LENGTH, NEWS_TITLE_MAX_LENGTH, NEWS_BODY_MAX_LENGTH

from wcics import db

from wcics.utils.time import get_time

# News table
# Contains all of our news
class news(dbmodel, Helper):
  id = dbcol(dbint, primary_key = True)
  oid = dbcol(dbint, dbforkey('organizations.id', onupdate = "CASCADE", ondelete = "CASCADE"))
  nid = dbcol(dbstr(NEWS_ID_MAX_LENGTH), unique = True, nullable = False)
  body = dbcol(dbstr(NEWS_BODY_MAX_LENGTH), nullable = False)
  time = dbcol(dbint, default = get_time, nullable = False)
  title = dbcol(dbstr(NEWS_TITLE_MAX_LENGTH), nullable = False)
  
  __table_args__ = (dbunicon("oid", "nid"),)
  
  @property
  def authors(self):
    from .users import Users
    # importing inside the function to avoid a circular import at the top
    # the alternative is worse, which uses a private method and key access to the name directly
    return Users.query.join(NewsAuthors).filter(NewsAuthors.nid == self.id).all()
  
  @property
  def author_ids(self):
    return [uid for (uid,) in db.session.query(NewsAuthors.uid).filter_by(nid = self.id).all()]
  
  def has_author(self, uid):
    return NewsAuthors.query.filter_by(nid = self.nid, uid = uid).count() > 0
  
  def __repr__(self):
    return "<news %s>" % self.nid
  
# Store the authors to each news article
class news_authors(dbmodel, Helper):
  nid = dbcol(dbint, dbforkey('news.id', onupdate="CASCADE", ondelete="CASCADE"), primary_key = True)
  uid = dbcol(dbint, dbforkey('users.id', onupdate="CASCADE", ondelete="CASCADE"), primary_key = True)
  
  def __repr__(self):
    return "<news_author article=%s, user=%s>" % (self.nid, self.uid)
  
# This is to allow easy table renaming
News = news
NewsAuthors = news_authors