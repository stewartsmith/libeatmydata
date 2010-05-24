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
import stat
import sys
import subprocess

import gettext
from gettext import gettext as _
gettext.textdomain('quickly')

from quickly import configurationhandler
from quickly import templatetools

def help():
    print _("""Usage:
$quickly run

Runs your application. This is the best way to try test it out
while you are developing it. It starts up the main project window.

$ quickly run -- values -<whathever>
to pass "-whatever" and "values" to the executed program. Without that
if you use for instance --help, it would be Quickly help and not your
program one.
""")
templatetools.handle_additional_parameters(sys.argv, help)

# if config not already loaded
if not configurationhandler.project_config:
    configurationhandler.loadConfig()

# check if we can execute a graphical project
if not templatetools.is_X_display():
    print _("Can't access to X server, so can't run gtk application")
    sys.exit(1)

project_bin = 'bin/' + configurationhandler.project_config['project']
command_line = [project_bin]
command_line.extend([arg for arg in sys.argv[1:] if arg != "--"])

# run with args if bin/project exist
st = os.stat(project_bin)
mode = st[stat.ST_MODE]
if mode & stat.S_IEXEC:
    subprocess.call(command_line)
else:
    print _("Can't execute %s") % project_bin
    sys.exit(1)

