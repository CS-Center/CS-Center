# -*- coding: utf-8 -*-

# Return the contents of a file
def load_file(filename):
  with open(filename, "r") as f:
    return f.read()
  
# Write contents to a file
def write_file(filename, content):
  with open(filename, "w+") as f:
    f.write(content)

# Append contents to a file
def append_file(filename, content):
  with open(filename, "a+") as f:
    f.write(content)
