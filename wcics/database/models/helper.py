# -*- coding: utf-8 -*-

from wcics import db, logger

# A helper class for our models
class Helper:
  @classmethod
  def add(cls, **kwargs):
    obj = cls(**kwargs)
    db.session.add(obj)
    return obj
  
  @classmethod
  def remove(cls, obj):
    db.session.delete(obj)