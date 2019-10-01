# -*- coding: utf-8 -*-

from ..consts.attendance import ATTENDANCE_CODE_MAX_LENGTH

from .aliases import *
from .helper import Helper

from wcics.utils.time import get_time
from wcics.utils.url import get_org_id

class attendance_codes(Helper, dbmodel):
  """Store all the attenadnce codes, along with their time intervals"""
  
  id = dbcol(dbint, primary_key = True)
  oid = dbcol(dbint, dbforkey('organizations.id', onupdate = "CASCADE", ondelete = "CASCADE"))
  code = dbcol(dbstr(ATTENDANCE_CODE_MAX_LENGTH), nullable = False)
  start = dbcol(dbint, nullable = False)
  end = dbcol(dbint, nullable = False)
  
  __table_args__ = (dbunicon("oid", "code"),)
  
  @classmethod
  def current_objs(self, oid):
    """Get the current attendance codes"""
    
    t = get_time()
    return self.query.filter(self.oid == oid, self.start <= t, t <= self.end).all()
  
  @classmethod
  def current_codes(self, oid):
    return [obj.code for obj in self.current_objs(oid)]
  
  def __repr__(self):
    return "<attendance_code %s>" % self.code
  
class attendance_records(Helper, dbmodel):
  """Store all the attendance records"""
  
  cid = dbcol(dbint, dbforkey('attendance_codes.id', onupdate = "CASCADE", ondelete = "CASCADE"), primary_key = True)
  uid = dbcol(dbint, dbforkey('users.id', onupdate = "CASCADE", ondelete = "CASCADE"), primary_key = True)
  time = dbcol(dbint, nullable = False, default = 0)
  
  def __repr__(self):
    record = AttendanceCodes.query.filter_by(id = cid).first()
    return "<attendance_record uid=%s cid=%s>" % (self.uid, record.cid)
  
  @classmethod
  def count(self, user):
    if isinstance(user, int):
      uid = user
    else:
      uid = user.id
    return self.query.filter_by(uid = uid, oid = get_org_id()).count()
  
# Aliases
AttendanceCodes = attendance_codes
AttendanceRecords = attendance_records