from wcics import app

from flask import Markup

import os, hashlib, base64

_index_cache = {}

# Get the indexed url and integrity for a static path
def indexed_url(orig_path):
  if app.debug:
    return orig_path
  
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

      val = "/static" + dir + "/_hashed_static." + hashed.hexdigest() + "." + filename
      
      _index_cache[orig_path] = val

      return val
  except FileNotFoundError:
    raise FileNotFoundError("indexed_url was called on a file that does not exist: '%s'" % path)
    
@app.template_global("static_link")
def make_static_link(path, tag = None, link_tag = None, **kwargs):
  if tag is None:
    if path.endswith(".js"):
      tag = "script"
    else:
      tag = "link"
      
  if link_tag is None:
    if path.endswith(".js"):
      link_tag = "src"
    else:
      link_tag = "href"
  
  subres = indexed_url(path)
  
  return Markup("<{tag} {extra_tags} {link_tag}={link}></{tag}>".format(
    tag = tag, 
    link_tag = link_tag, 
    link = repr(subres), 
    # val should be a string, so this will escape and the like
    extra_tags = " ".join(key + "=" + repr(val) for key, val in kwargs.items())
  ))

# As for above, but specialized for CSS
@app.template_global("css_link")
def make_css_link(path, **kwargs):
  kwargs.setdefault('rel', 'stylesheet')
  kwargs.setdefault('type', 'text/css')
  
  return make_static_link(path = path, **kwargs)

# As for above, but specialized for javascript
@app.template_global("js_link")
def make_js_link(path, **kwargs):
  kwargs.setdefault('type', 'text/javascript')
  
  return make_static_link(path = path, **kwargs)

