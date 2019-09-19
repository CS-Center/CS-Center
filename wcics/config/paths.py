# -*- coding: utf-8 -*-

from os import environ

import pathlib

HOME_DIR = str(pathlib.Path.home())

# Paths to folders of interest

ROOT_FOLDER_PATH = HOME_DIR + "/workspace"
  
SYSTEM_FOLDER_PATH = ROOT_FOLDER_PATH + "/system"
SERVER_FOLDER_PATH = ROOT_FOLDER_PATH + "/{branch}/wcics/server".format(branch = environ["WCICS_BRANCH"])
CONFIG_FOLDER_PATH = ROOT_FOLDER_PATH + "/{branch}/wcics/config/config-files".format(branch = environ["WCICS_BRANCH"])
KEYS_FOLDER_PATH = SYSTEM_FOLDER_PATH + "/keys"