# -*- coding: utf-8 -*-
# Main server file; run directly to start the server

from wcics import app, socketio

from wcics.consts import PORT
from wcics.server.routes import *

from flask import request

if __name__ == "__main__":
  # run the server on localhost, at the specified port
  # This implies debug mode
  socketio.run(app, port = PORT, debug = True)