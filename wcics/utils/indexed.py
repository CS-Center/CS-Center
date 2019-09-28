from wcics import app

import os, hashlib, base64

_index_cache = {}

# Get the indexed url and integrity for a static path
def indexed_url(path):
  if app.debug:
    return path, None
  
  res = _index_cache.get(path)
  
  if res is not None:
    return res
  
  # Strip /static from the front of path
  path = path[len("/static"):]
  
  filename = path.split("/")[-1]
  
  with open(app.static_folder + path, "rb") as f:
    content = f.read()
    
    hashed = hashlib.sha256(content)
    
    return "_hashed_static." + hashed.hexdigest() + "." + filename, "sha256-" + base64.b64encode(hashed.digest()).decode("utf-8")