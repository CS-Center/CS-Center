import sys

def error(msg):
  print(msg, file = sys.stderr)
  return 1