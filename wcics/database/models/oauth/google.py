# -*- coding: utf-8 -*-

from ..aliases import *
from ..helper import Helper

class google_links(dbmodel, Helper):
  uid = dbcol(dbint, dbforkey("users.id"), primary_key = True)
  gid = dbcol(dbstr, primary_key = True, unique = True)
  
  def __repr__(self):
    return "<google_link uid=%d gid=%s>" % (self.uid, self.gid)

GoogleLinks = google_links