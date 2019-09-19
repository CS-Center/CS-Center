# -*- coding: utf-8 -*-

from flask import flash

def flash_form_errors(form, final = None):
  flashed = False
  for e in form.errors:
    for error in form.errors[e]:
      flash(error, category = "ERROR")
      flashed = True
  if flashed and final is not None:
    flash(final, category = "ERROR")