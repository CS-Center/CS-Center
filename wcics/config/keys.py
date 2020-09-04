import os, sys, flask

keyfile_name = os.environ['CSCENTER_KEYFILE']

keys = flask.json.load(open(keyfile_name))
