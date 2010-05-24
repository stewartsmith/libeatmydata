#!/usr/bin/python
# -*- coding: utf-8 -*-
# Copyright © 2010 Monty Taylor
# Copyright 2009 Didier Roche
#
# This file is part of Quickly drizzle-plugin template
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

import sys
import os
import shutil
import subprocess

from quickly import templatetools, configurationhandler, tools
from internal import quicklyutils, naming, pandoramacros

import gettext
from gettext import gettext as _
# set domain text
gettext.textdomain('quickly')

project_root_prefix = "project_root_"


def help():
    print _("""Usage:
$ quickly create pandora-build project_name project_type

where "project_name" is the name of the project to create.

This will create a new project dir with the initial skeleton files needed.
""")
templatetools.handle_additional_parameters(sys.argv, help)


# get the name of the project
if len(sys.argv) < 2:
    print _("""Project name not defined.\n""")
if len(sys.argv) < 3:
    print _("""Plugin Type not defined.\nUsage is quickly create pandora-build plugin_name plugin_type""")
    sys.exit(4)

path_and_project = sys.argv[1].split('/')

names = None
type_names = None
# check that project name follow quickly rules and reformat it.
# TODO: need to handle input in the form of StoragEngine and turn it in to
#  storage_engine, Storage Engine, STORAGE_ENGINE and StorageEngine
#  respectively
try:
    names = naming.naming_context(path_and_project[-1])
    type_names = naming.naming_context(sys.argv[2])
except templatetools.bad_project_name, e:
    print(e)
    sys.exit(1)


include_guard_names = []
include_guard_names.append(names.all_caps_name)
include_guard_names.append(names.all_caps_name)
include_guard_names.append("H")
include_guard = "_".join(include_guard_names)

open_namespace = "namespace %s\n{" % names.project_name
close_namespace = "} /* namespace %s */" % names.project_name

substitutions = (
            ("type_camel_case_name",type_names.camel_case_name),
            ("type_sentence_name",type_names.sentence_name),
            ("plugin_type",type_names.project_name),
            ("include_guard", include_guard),
            ("all_caps_project_name", names.all_caps_name),
            ("project_name",names.project_name),
            ("camel_case_name",names.camel_case_name),
            ("sentence_name",names.sentence_name),
            ("all_caps_name",names.all_caps_name),
            ("class_name",names.project_name),
            ("open_namespace",open_namespace),
            ("close_namespace",close_namespace),
            )


os.chdir(names.base_name)

# get origin path
pathname = templatetools.get_template_path_from_project()
abs_path_project_root = os.path.join(pathname, 'project_root')

for abs_path_project_root in [os.path.join(pathname, f) for f in ('project_root', '%s%s' % (project_root_prefix, type_names.project_name))]:
    if os.path.isdir(abs_path_project_root):
        for root, dirs, files in os.walk(abs_path_project_root):
            try:
                relative_dir = root.split('%s/' % abs_path_project_root)[1]
            except:
                relative_dir = ""
            # python dir should be replace by python (project "pythonified" name)
            if relative_dir.startswith('project_name'):
                relative_dir = relative_dir.replace('project_name', names.project_name)
         
            for directory in dirs:
                if directory == 'project_name':
                    directory = names.project_name
                try:
                    os.mkdir(os.path.join(relative_dir, directory))
                except OSError:
                    # We don't care - we may be merging dirs
                    pass
            for filename in files:
                quicklyutils.file_from_template(root, filename, relative_dir, substitutions)

try:
    os.mkdir("m4")
except OSError:
    pass

pandoramacros.copy_pandora_files()
pandora_version = pandoramacros.get_pandora_version()
    
try:
    os.chmod("config/autorun.sh", 0755)
    os.chmod("config/pandora-plugin", 0755)
    os.chmod("config/config.rpath", 0755)
    os.chmod("test_run.sh", 0755)
except:
    pass

configurationhandler.loadConfig()
configurationhandler.project_config['project-type'] = type_names.project_name
configurationhandler.project_config['pandora-version'] = pandora_version
configurationhandler.saveConfig()

# add it to revision control
print _("Adding to bzr repository and commiting")
bzr_instance = subprocess.Popen(["bzr", "add"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
if bzr_instance.wait() != 0:
  # We are not in a bzr branch yet. Create one.
  print _("Creating bzr branch")
  bzr_instance = subprocess.Popen(["bzr", "init"], stdout=subprocess.PIPE)
  bzr_instance.wait()
  bzr_instance = subprocess.Popen(["bzr", "add"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
  bzr_instance.wait()

bzr_instance = subprocess.Popen(["bzr", "commit", "-m", "Initial project creation with Quickly!", "."], stderr=subprocess.PIPE)
bzr_instance.wait()

print _("Congrats, your new project is setup! cd %s/ to start hacking.") % os.getcwd()

sys.exit(0)
