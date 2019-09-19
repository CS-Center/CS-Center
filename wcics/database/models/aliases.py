# -*- coding: utf-8 -*-

from wcics import db

# Short forms for our types
dbenum = db.Enum
dbcol = db.Column

dbsmallint = db.SmallInteger
dbint = db.Integer
dblong = db.BigInteger

dbtext = db.Text
dbstr = db.String

dbfloat = db.Float

dbbool = db.Boolean

dbbinary = db.LargeBinary

dbdate = db.Date

dbunicon = db.UniqueConstraint

dbrelate = db.relationship
dbforkey = db.ForeignKey
dbmodel = db.Model