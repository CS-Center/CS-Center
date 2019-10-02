from wcics import app

import os, hashlib, base64

_index_cache = {}

# Get the indexed url and integrity for a static path
def indexed_url(orig_path):
  if app.debug:
    return orig_path, None
  
  res = _index_cache.get(orig_path)
  
  if res is not None:
    return res
  
  # Strip /static from the front of path
  # Also strip trailing slashes, because they cause annoying errors
  path = orig_path[len("/static"):].rstrip("/")
  
  dir = "/".join(path.split("/")[:-1])
  
  filename = path.split("/")[-1]
  
  try:
    with open(app.static_folder + path, "rb") as f:
      content = f.read()

      hashed = hashlib.sha256(content)

      val = orig_path.rstrip("/") + "/_hashed_static." + hashed.hexdigest() + "." + filename, "sha256-" + base64.b64encode(hashed.digest()).decode("utf-8")

      _index_cache[orig_path] = val

      return val
  except FileNotFoundError:
    raise FileNotFoundError("indexed_url was called on a file that does not exist: '%s'" % path)