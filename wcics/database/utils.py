# -*- coding: utf-8 -*-

# Database utility functions
from wcics import db

def db_rollback():
  # Rollback the session if an error occurs
  db.session.rollback()

def db_commit():
  # Commit to the database
  db.session.commit()
  
def create_tables():
  # Create all the mappings
  db.create_all()
  
def get_descendants(table, pkey : str, fkey : str, *args, **kwargs):
  top = db.session.query(getattr(table, pkey), getattr(table, fkey)).filter(*args).filter_by(**kwargs).cte(recursive = True)
  mid = top.union_all(db.session.query(getattr(table, pkey), getattr(table, fkey)).filter(getattr(top.c, pkey) == getattr(table, fkey)))
  return table.query.filter(getattr(table, pkey) == getattr(mid.c, pkey)).all()