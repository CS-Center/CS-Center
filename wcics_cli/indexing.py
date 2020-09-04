# File for 'indexing' a directory
# Generates or removes _hashed_static files
# This allows for exceptionally efficient caching

import os, hashlib, base64, sys

def main(args):
  if args[0] == 'make':
    return index_make(args[1:])

  elif args[0] == 'clean':
    return index_clean(args[1:])

  else:
    raise ValueError(f"Invalid subcommand {args[0]}, valid values are: make, clean.")

def index_make(args):
  if not args:
    return error("A directory is required in order to index make")

  print("Cleaning tree first...", file = sys.stderr)

  sub = _clean(args[0])

  if sub:
    return sub

  print("Indexing tree...", file = sys.stderr)

  return _make(args[0])

indexed_prefix = "_hashed_static"

def _make(dir):
  with os.scandir(dir) as it:
    for entry in it:
      if entry.is_dir():
        subres = _make(entry.path)

        if subres:
          return subres

      elif not entry.name.startswith(indexed_prefix):
        # entry.is_file()

        with open(entry.path, "rb") as f:
          content = f.read()

          hashed_content = hashlib.sha256(content).hexdigest()

          newfilename = dir + "/" + indexed_prefix + "." + hashed_content + "." + entry.name

          with open(newfilename, "wb") as newf:
            newf.write(content)

def index_clean(args):
  if not args:
    return error("A directory is required in order to index_clean")

  return _clean(args[0])

def _clean(dir):
  with os.scandir(dir) as it:
    for entry in it:
      if entry.is_dir():
        _clean(entry.path)

      else:
        if entry.name.startswith(indexed_prefix):
          os.unlink(entry.path)
