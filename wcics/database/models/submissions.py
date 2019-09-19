# -*- coding: utf-8 -*-

from .aliases import *
from .helper import Helper
from .problems import Problems, ProblemCases
from .users import Users
from ..consts.submissions import *

class Submissions(Helper, dbmodel):
  id = dbcol(dblong, primary_key = True)
  pid = dbcol(dbint, dbforkey(Problems.id, onupdate = "CASCADE", ondelete = "CASCADE"), nullable = False)
  uid = dbcol(dbint, dbforkey(Users.id, onupdate = "CASCADE", ondelete = "CASCADE"), nullable = False)
  code = dbcol(dbstr(SUBMISSION_CODE_MAX_LENGTH), nullable = False)
  lang = dbcol(dbstr(SUBMISSION_LANG_MAX_LENGTH), nullable = False)
  
  # Null verdict indicates that the submission is not complete
  # This verdict is special: it indicates either IE, CE or successful grading
  verdict = dbcol(dbstr(2), nullable = True)
  
  compilation_output = dbcol(dbstr(SUBMISSION_COMPILER_OUTPUT_MAX_LENGTH), nullable = False)
  
  __tablename__ = "submissions"
  
class SubmissionCases(Helper, dbmodel):
  sid = dbcol(dblong, dbforkey(Submissions.id, onupdate = "CASCADE", ondelete = "CASCADE"), primary_key = True)
  pcid = dbcol(dblong, dbforkey(ProblemCases.id, onupdate = "CASCADE", ondelete = "CASCADE"), primary_key = True)
  points = dbcol(dbint, nullable = False)
  verdict = dbcol(dbstr(5), nullable = False)
  info = dbcol(dbstr(SUBMISSION_INFO_MAX_LENGTH), nullable = False)
  clipped_output = dbcol(dbstr(SUBMISSION_OUTPUT_MAX_LENGTH), nullable = False)
  
  __tablename__ = "submission_cases"