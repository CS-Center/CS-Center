# -*- coding: utf-8 -*-

from .aliases import *
from .helper import Helper
from .problems import Problems
from wcics.database.consts.problem_judging import *

class ProblemSuites(dbmodel, Helper):
  id = dbcol(dbint, primary_key = True)
  pid = dbcol(dbint, dbforkey(Problems.id, onupdate = "CASCADE", ondelete = "CASCADE"), nullable = False)
  suite = dbcol(dbint, nullable = False)
  
  points = dbcol(dbint, nullable = False)
  
  __tablename__ = "problem_suites"

class ProblemCases(dbmodel, Helper):
  pid = dbcol(dbint, dbforkey(Problems.id, onupdate = "CASCADE", ondelete = "CASCADE"), nullable = False)
  sid = dbcol(dbint, dbforkey(ProblemSuites.id, onupdate = "CASCADE", ondelete = "CASCADE"), nullable = False)
  
  input = dbcol(dbstr(PROBLEM_INPUT_MAX_LENGTH), nullable = False)
  output = dbcol(dbstr(PROBLEM_OUTPUT_MAX_LENGTH), nullable = False)
  
  case = dbcol(dbint, nullable = False)
  
  __tablename__ = "problem_cases"
  
class ProblemJudging(dbmodel, Helper):
  pid = dbcol(dbint, dbforkey(Problems.id, onupdate = "CASCADE", ondelete = "CASCADE"), nullable = False, unique = True)

  checker_id = dbcol(dbstr(ID_MAX_LENGTH), nullable = False)
  checker_code = dbcol(dbstr(CODE_MAX_LENGTH))
  checker_lang = dbcol(dbstr(LANG_MAX_LENGTH))
  checker_arg = dbcol(dbstr(ARG_MAX_LENGTH), nullable = False)
  
  interactor_id = dbcol(dbstr(ID_MAX_LENGTH), nullable = False)
  interactor_code = dbcol(dbstr(CODE_MAX_LENGTH))
  interactor_lang = dbcol(dbstr(LANG_MAX_LENGTH))
  interactor_arg = dbcol(dbstr(ARG_MAX_LENGTH), nullable = False)