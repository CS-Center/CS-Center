# -*- coding: utf-8 -*-

from .aliases import *
from .helper import Helper
from .organizations import Organizations
from .users import Users
from wcics.database.consts.problems import *

psection = dbstr(PROBLEM_SECTION_MAX_LENGTH)

class Problems(dbmodel, Helper):
  id = dbcol(dbint, primary_key = True)
  pid = dbcol(dbstr(PROBLEM_PID_MAX_LENGTH), nullable = False)
  oid = dbcol(dbint, dbforkey(Organizations.id, onupdate = "CASCADE", ondelete = "CASCADE"), nullable = False)

  memory_limit = dbcol(dblong, nullable = False)
  timelimit = dbcol(dbfloat, nullable = False)

  # Problem sections
  title = dbcol(psection, nullable = False)
  desc = dbcol(psection, nullable = False)
  constraints = dbcol(psection, nullable = False)
  input_specs = dbcol(psection, nullable = False)
  output_specs = dbcol(psection, nullable = False)

  # force (pid, oid) to be unique
  __table_args__ = (dbunicon('pid', 'oid'), )

  __tablename__ = "problems"

class ProblemAuthors(dbmodel, Helper):
  pid = dbcol(dbint, dbforkey(Problems.id, onupdate = "CASCADE", ondelete = "CASCADE"), primary_key = True)
  uid = dbcol(dbint, dbforkey(Users.id, onupdate = "CASCADE", ondelete = "CASCADE"), primary_key = True)

  __tablename__ = "problem_authors"

class ProblemSamples(dbmodel, Helper):
  id = dbcol(dblong, primary_key = True)
  pid = dbcol(dbint, dbforkey(Problems.id, onupdate = "CASCADE", ondelete = "CASCADE"), nullable = False)
  ordering = dbcol(dbint, nullable = False)

  # i/o, expl
  input = dbcol(dbstr(PROBLEM_SAMPLE_MAX_LENGTH), nullable = False)
  output = dbcol(dbstr(PROBLEM_SAMPLE_MAX_LENGTH), nullable = False)
  explanation = dbcol(dbstr(PROBLEM_SAMPLE_MAX_LENGTH), nullable = False)

  # Force pid, ordering to be unique
  __table_args = (dbunicon("pid", "ordering"), )

  __tablename__ = "problem_samples"
  
