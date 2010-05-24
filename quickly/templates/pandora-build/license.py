#!/usr/bin/python
# -*- coding: utf-8 -*-
# Copyright 2009 Didier Roche
#
# This file is part of Quickly ubuntu-application template
#
#This program is free software: you can redistribute it and/or modify it 
#under the terms of the GNU General Public License version 3, as published 
#by the Free Software Foundation.
#
#This program is distributed in the hope that it will be useful, but 
#WITHOUT ANY WARRANTY; without even the implied warranties of 
#MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR 
#PURPOSE.  See the GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License along 
#with this program.  If not, see <http://www.gnu.org/licenses/>.

import datetime
import filecmp
import os
import re
import shutil
import sys
import ConfigParser

from quickly import configurationhandler, templatetools
from internal import quicklyutils

import gettext
from gettext import gettext as _
# set domain text
gettext.textdomain('quickly')

class LicenceError(Exception):
    def __init__(self, msg):
        self.msg = msg
    def __str__(self):
        return repr(self.msg)

BEGIN_LICENCE_TAG_PY = '### BEGIN LICENSE'
END_LICENCE_TAG_PY = '### END LICENSE'
BEGIN_LICENCE_TAG_C = '/* BEGIN LICENSE'
END_LICENCE_TAG_C = ' * END LICENSE */'


def help():
    print _("""Usage:
$quickly license <Your_Licence>

Adds license to project files. Before using this command, you should:

1. Edit the file AUTHORS to include your authorship (this step is automatically done
   if you directly launch "$ quickly release" or "$ quickly share" before changing license)
   In this case, license is GPL-3 by default.
2. If you want to put your own quickly unsupported Licence, add a COPYING file containing
   your own licence.
3. Executes either $ quickly license or $ quickly license <License>
   where <License> can be either:
   - GPL-3 (default)
   - GPL-2

This will modify the Copyright file with the chosen licence (with GPL-3 by default).
Updating previous chosen Licence if needed.
If you previously removed the tags to add your own licence, it will leave it pristine.
If no name is attributed to the Copyright, it will try to retrieve it from Launchpad
(in quickly release or quickly share command only)

Finally, this will copy the Copyright at the head of every files.

Note that if you don't run quickly licence before calling quickly release or quickly
share, this one will execute it for you and guess the copyright holder from your
launchpad account if you didn't update it.
""")

def get_supported_licenses():
    """Get supported licenses"""

    available_licenses = []
    
    for license_file in os.listdir(os.path.dirname(__file__) + "/available_licenses"):
        result = re.split("header_(.*)", license_file)
        if len(result) == 3:
            available_licenses.append(result[1])
 
    return available_licenses
    
def get_plugin_ini_value(opt):
    """Pull a value from plugin.ini"""
    if os.path.exists('plugin.ini'):
        parser=ConfigParser.ConfigParser()
        parser.read('plugin.ini')
        plugin=dict(parser.items('plugin'))
        return plugin.get(opt, None)
    else:
        return None

def prefix_license(old_license, license_prefix):
    """Return the license modified to have a line prefix"""
    new_license=""
    for lic_line in old_license.split('\n')[:-1]:
        new_license += "%s %s\n" % (license_prefix, lic_line)
    return new_license

def copy_license_to_files(license_content):
    """Copy license header to every .py, .cc or .h files"""

    # get the project name
    if not configurationhandler.project_config:
        configurationhandler.loadConfig()
    project_name = configurationhandler.project_config['project']

    # open each python file and main bin file
    for root, dirs, files in os.walk('./'):
        for name in files:
            if name.endswith('.py') or \
              name.endswith('.h') or name.endswith('.hh') or \
              name.endswith('.c') or name.endswith('.cc') or \
              name.endswith('.am') or name.endswith('ac'):
                skip_until_end_found = False
                try:
                    target_file_name = os.path.join(root, name)
                    ftarget_file_name = file(target_file_name, 'r')
                    ftarget_file_name_out = file(ftarget_file_name.name + '.new', 'w')
                    for line in ftarget_file_name:
                        # seek if we have to add or Replace a License
                        if BEGIN_LICENCE_TAG_PY in line:
                            ftarget_file_name_out.write(line) # write this line, otherwise will be skipped
                            skip_until_end_found = True
                            ftarget_file_name_out.write(prefix_license(license_content, "#"))
                        if BEGIN_LICENCE_TAG_C in line:
                            ftarget_file_name_out.write(line) # write this line, otherwise will be skipped
                            skip_until_end_found = True
                            ftarget_file_name_out.write(prefix_license(license_content, " *"))

                        if END_LICENCE_TAG_C in line or END_LICENCE_TAG_PY in line:
                            skip_until_end_found = False

                        if not skip_until_end_found:
                            ftarget_file_name_out.write(line)


                    ftarget_file_name.close()
                    ftarget_file_name_out.close()

                    if skip_until_end_found: # that means we didn't find the END_LICENCE_TAG, don't copy the file
                        print _("WARNING: %s was not found in the file %s. No licence replacement") % (END_LICENCE_TAG, ftarget_file_name.name)
                        os.remove(ftarget_file_name_out.name)
                    else:
                        templatetools.apply_file_rights(ftarget_file_name.name, ftarget_file_name_out.name)
                        os.rename(ftarget_file_name_out.name, ftarget_file_name.name)

                except (OSError, IOError), e:
                    msg = _("%s file was not found") % fcopyright_name
                    raise LicenceError(msg)


def licensing(license=None):
    """Add license or update it to the project files

    Default is GPL-3"""


    fauthors_name = "AUTHORS"
    flicense_name = "COPYING"

    if not configurationhandler.project_config:
        configurationhandler.loadConfig()
    project_name = configurationhandler.project_config['project']
    python_name = templatetools.python_name(project_name)

    # check if we have a license tag in plugin.ini otherwise, default to GPL-3
    if license is None:
        license = get_plugin_ini_value('license')
    if license is None or license == '':
        license = 'GPL'
    if license == 'PLUGIN_LICENSE_GPL':
        license = 'GPL-2'
    if license == 'PLUGIN_LICENSE_BSD':
        license = 'BSD'

    # get Copyright holders in AUTHORS file
    license_content = ""
    try:
        for line in file(fauthors_name, 'r'):
            if "<Your Name> <Your E-mail>" in line:
                # if we have an author in setup.py, grab it
                try:
                    author = get_plugin_ini_value('author')
                    if author is None:
                        raise quicklyutils.cant_deal_with_setup_value()
                    line = "Copyright (C) %s %s\n" % (datetime.datetime.now().year, author, author_email)
                    # update AUTHORS file
                    fout = file('%s.new' % fauthors_name, 'w')
                    fout.write(line)
                    fout.flush()
                    fout.close()
                    os.rename(fout.name, fauthors_name)
                except quicklyutils.cant_deal_with_setup_value:
                    msg = _('Copyright is not attributed. ' \
                            'Edit the AUTHORS file to include your name for the copyright replacing ' \
                            '<Your Name> <Your E-mail>. Update it in setup.py or use quickly share/quickly release ' \
                            'to fill it automatically')
                    raise LicenceError(msg)
            license_content += "%s" % line
    except (OSError, IOError), e:
        msg = _("%s file was not found") % fauthors_name
        raise LicenceError(msg)

    # add header to license_content
    # check that COPYING file is provided if using a personal license
    supported_licenses_list = get_supported_licenses()
    if license in supported_licenses_list:
        header_file_path = os.path.dirname(__file__) + "/available_licenses/header_" + license
    else:
        header_file_path = flicense_name
    try:
        for line in file(header_file_path, 'r'):
            license_content += line
    except (OSError, IOError), e:
        if header_file_path == flicense_name:
            msg = _("%s file was not found. It is compulsory for user defined license") % flicense_name
        else:
            msg = _("Header of %s license not found. Quickly installation corrupted?") % header_file_path
        raise LicenceError(msg)

    copy_license_to_files(license_content)


def shell_completion(argv):
    """Propose available license as the third parameter"""
    
    # if then license argument given, returns available licenses
    if len(argv) == 1:
        print " ".join(get_supported_licenses())


if __name__ == "__main__":

    templatetools.handle_additional_parameters(sys.argv, help, shell_completion)
    license = None
    if len(sys.argv) > 2:
        print _("This command only take one optional argument: License\nUsage is: quickly license <license>")
        sys.exit(4)
    if len(sys.argv) == 2:
        license = sys.argv[1]
    try:
        licensing(license)
    except LicenceError, error_message:
        print(error_message)
        sys.exit(1)

