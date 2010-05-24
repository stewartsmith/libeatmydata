#!/usr/bin/python
# -*- coding: utf-8 -*-
# Copyright © 2010 Monty Taylor
# Copyright 2009 Didier Roche
#
# This file is part of Quickly pandora-build template
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
import sys
import subprocess
from internal import naming, quicklyutils
from quickly import configurationhandler, templatetools

import gettext
from gettext import gettext as _
# set domain text
gettext.textdomain('quickly')

def help():
    print _("""Usage:
$ quickly add [class_name]
add a class 

class_name is a namespace-qualified classname relative to the current
location.

For instance $ quickly add drizzle::optimizer::HashJoin will create:
1. Two files, drizzle/optimizer/hash_join.h and
   drizzle/optimizer/hash_join.cc
2. A class defined in the .h file and some basic elements defined in the .cc
   file.

""")
def shell_completion(argv):
    ''' Complete args '''
    # option completion
    return ""
templatetools.handle_additional_parameters(sys.argv, help)

abs_template_path = templatetools.get_template_path_from_project()
abs_command_path = os.path.abspath(os.path.dirname(sys.argv[0]))


if len(sys.argv) < 2:
    print _("add command needs to be followed an action name.\n"
"Usage is: quickly add [classname]")
    sys.exit(4)

path_and_project = sys.argv[1].split('::')

if not configurationhandler.project_config:
    configurationhandler.loadConfig()
project_name = configurationhandler.project_config['project']

class_names = None
project_names = None
# check that project name follow quickly rules and reformat it.
# TODO: need to handle input in the form of StoragEngine and turn it in to
#  storage_engine, Storage Engine, STORAGE_ENGINE and StorageEngine
#  respectively
try:
    class_names = naming.naming_context(path_and_project[-1])
    project_names = naming.naming_context(project_name)
except templatetools.bad_project_name, e:
    print(e)
    sys.exit(1)


target_dir = ""
open_namespace = ""
close_namespace = ""
include_guard_names = []
namespace_elems = []

# If we haven't been given a namespace, add the namespace of the project
if (len(path_and_project) == 1):
    namespace_elems = [project_name]
else:
    namespace_elems = path_and_project[:-1]
    
for namespace_elem in namespace_elems:
    namespace_naming = naming.naming_context(namespace_elem)
    target_dir = os.path.join(target_dir, namespace_naming.project_name)
    if not os.path.exists(target_dir):
        os.mkdir(target_dir)
        bzr_add = subprocess.Popen(["bzr", "add", "--no-recurse", target_dir],
                                   stdout=subprocess.PIPE)
        bzr_add.wait()

    include_guard_names.append(namespace_naming.all_caps_name)

    open_namespace = "%snamespace %s\n{\n" % (open_namespace, namespace_naming.project_name)
    close_namespace = "} /* namespace %s */\n%s" % (namespace_naming.project_name, close_namespace)

include_guard_names.append(class_names.all_caps_name)
include_guard_names.append("H")
include_guard = "_".join(include_guard_names)

substitutions = (
            ("include_guard", include_guard),
            ("all_caps_project_name", project_names.all_caps_name),
            ("project_name",project_names.project_name),
            ("camel_case_name",class_names.camel_case_name),
            ("sentence_name",class_names.sentence_name),
            ("all_caps_name",class_names.all_caps_name),
            ("class_name",class_names.project_name),
            ("open_namespace",open_namespace),
            ("close_namespace",close_namespace),
            )

template_dir = os.path.join(abs_template_path, os.path.join('project_root', 'project_name'))

quicklyutils.file_from_template(template_dir, 
                                "class_name.h",
                                target_dir, 
                                substitutions)

target_header_file = os.path.join(target_dir,class_names.project_name) + ".h"
target_source_file = os.path.join(target_dir,class_names.project_name) + ".cc"

bzr_add = subprocess.Popen(["bzr", "add", target_header_file],
                           stdout=subprocess.PIPE)
bzr_add.wait()

quicklyutils.file_from_template(template_dir, 
                                "class_name.cc",
                                target_dir, 
                                substitutions)

bzr_add = subprocess.Popen(["bzr", "add", target_source_file],
                           stdout=subprocess.PIPE)
bzr_add.wait()

makefile = open("Makefile.am", "r")
new_makefile = open("Makefile.am.new", "w")

in_headers = False
accumulate_headers = []
in_sources = False
accumulate_sources = []
for line in makefile.readlines():

    if not in_headers and not in_sources:
        new_makefile.write(line)

    if in_headers:
        if line.strip() == "":
            in_headers = False
            # sort headers and then write out to file with \ on all but the
            # last
            accumulate_headers.append(target_header_file)
            accumulate_headers.sort()
            new_makefile.write('  ')
            new_makefile.write(' \\\n  '.join(accumulate_headers))
            new_makefile.write('\n\n')
        else:
            slash_pos = line.rfind('\\')
            if slash_pos != -1:
                accumulate_headers.append(line[:slash_pos].strip())
            else:
                accumulate_headers.append(line.strip())

    if in_sources:
        if line.strip() == "":
            in_sources = False
            accumulate_sources.append(target_source_file)
            accumulate_sources.sort()
            new_makefile.write('  ')
            new_makefile.write(' \\\n  '.join(accumulate_sources))
            new_makefile.write('\n\n')
        else:
            slash_pos = line.rfind('\\')
            if slash_pos != -1:
                accumulate_sources.append(line[:slash_pos].strip())
            else:
                accumulate_sources.append(line.strip())

    if line == 'nobase_include_HEADERS += \\\n':
        in_headers = True

    if line == 'lib%s_la_SOURCES = \\\n' % project_names.project_name:
        in_sources = True


new_makefile.close()
makefile.close()
os.rename("Makefile.am.new", "Makefile.am")
