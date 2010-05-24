#!/usr/bin/python
# -*- coding: utf-8 -*-
# Copyright 2009 Didier Roche
#
# This file is part of Quickly ubuntu-application template
#
#This program is free software: you can redistribute it and/or modify it 
#under the terms of the GNU General Public License version 3, as published 
#by the Free Software Foundation.

#This program is distributed in the hope that it will be useful, but 
#WITHOUT ANY WARRANTY; without even the implied warranties of 
#MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR 
#PURPOSE.  See the GNU General Public License for more details.

#You should have received a copy of the GNU General Public License along 
#with this program.  If not, see <http://www.gnu.org/licenses/>.

import os
import shutil
import subprocess
import sys

import internal.apportutils

from internal import quicklyutils, pandoramacros, naming
from quickly import configurationhandler, templatetools, tools

import gettext
from gettext import gettext as _
# set domain text
gettext.textdomain('quickly')

# get project version and template version if no argument given
if len(sys.argv) < 3:
    (project_version, template_version) = templatetools.get_project_and_template_versions("pandora-build")
else:
    project_version = sys.argv[1]
    template_version = sys.argv[2]

root_project_path = tools.get_root_project_path()
print "root path: ", root_project_path
os.chdir(root_project_path)

if not configurationhandler.project_config:
    configurationhandler.loadConfig()

project_name = configurationhandler.project_config['project']
project_type = configurationhandler.project_config['project-type']
pandora_version = configurationhandler.project_config['pandora-version']

names = None
type_names = None
# check that project name follow quickly rules and reformat it.
# TODO: need to handle input in the form of StoragEngine and turn it in to
#  storage_engine, Storage Engine, STORAGE_ENGINE and StorageEngine
#  respectively
try:
    names = naming.naming_context(project_name)
    type_names = naming.naming_context(project_type)
except templatetools.bad_project_name, e:
    print(e)
    sys.exit(1)

substitutions = (
            ("type_camel_case_name",type_names.camel_case_name),
            ("type_sentence_name",type_names.sentence_name),
            ("plugin_type",type_names.project_name),
            ("project_name",names.project_name),
            ("camel_case_name",names.camel_case_name),
            ("sentence_name",names.sentence_name),
            ("all_caps_name",names.all_caps_name),
            )


new_file_list = pandoramacros.copy_pandora_files()
new_pandora_version = pandoramacros.get_pandora_version()

try:
    os.chmod("config/autorun.sh", 0755)
    os.chmod("config/pandora-plugin", 0755)
    os.chmod("config/config.rpath", 0755)
    os.chmod("test_run.sh", 0755)
except:
    pass


configurationhandler.project_config['pandora-version'] = new_pandora_version
configurationhandler.saveConfig()

bzr_cmd = ["bzr", "add"]
bzr_cmd.extend(new_file_list)
bzr_instance = subprocess.Popen(bzr_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
bzr_instance.wait()

bzr_cmd = ["bzr", "commit", "-m", "Updated pandora-build files to version %s" % new_pandora_version]
bzr_cmd.extend(new_file_list)
bzr_cmd.append('.quickly')
bzr_instance = subprocess.Popen(bzr_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
bzr_instance.wait()

# Remove AC_CONFIG_HEADERS and AC_CONFIG_MACRO_DIR from configure.ac
if project_version < '0.99':
    pass

sys.exit(0)
