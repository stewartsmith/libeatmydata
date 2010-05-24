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
import sys
import locale
import subprocess

import gettext
from gettext import gettext as _
gettext.textdomain('quickly')

from quickly import templatetools

def help():
    print _("""Usage:
$ quickly tutorial

Opens help file with the tutorial for the current template.
""")
templatetools.handle_additional_parameters(sys.argv, help)

# TODO: FIX to take tutorial from the project path:
# abs_template_path = templatetools.get_template_path_from_project() stop when
# you are outside a project
help_dir = os.path.join(os.path.abspath(os.path.dirname(__file__)), 'help')
help_file = os.path.join(help_dir, 'tutorial-%s.xml'
                                    % locale.getdefaultlocale()[0])
if not os.path.isfile(help_file) and locale.getdefaultlocale()[0] is not None:
    help_file = os.path.join(help_dir,
                            'tutorial-%s.xml'
                            % locale.getdefaultlocale()[0].split('_')[0])
if not os.path.isfile(help_file):
    help_file = os.path.join(help_dir, 'tutorial.xml')
subprocess.Popen(['yelp', help_file], stderr=file("/dev/null"))

