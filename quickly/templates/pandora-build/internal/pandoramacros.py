# -*- coding: utf-8 -*-
#
# Copyright © 2010 Monty Taylor
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

import os, shutil

autoconf_m4=[
  "ac_cxx_header_stdcxx_98.m4",
  "ac_cxx_compile_stdcxx_0x.m4",
  "po.m4",
  "acx_pthread.m4",
  "iconv.m4",
  "progtest.m4"]

def copy_pandora_m4(frompath="/usr/share/aclocal/"):
    """Copy in the latest relevant m4 files to the project"""
    new_files=[]
    for root, dirs, files in os.walk("m4"):
        old_m4_files = [f for f in files if os.path.isfile("%s/m4/%s" % (root,f)) and f.startswith("pandora_")]
        for m4_file in old_m4_files:
            os.unlink("m4/%s" % m4_file)

    for root, dirs, files in os.walk(frompath):
        new_m4_files = [f for f in files if os.path.isfile("%s/%s" % (frompath, f)) and f.startswith("pandora_")]
        for m4_file in new_m4_files:
            new_files.append("m4/%s" % m4_file)
            shutil.copy("%s/%s" % (frompath, m4_file), "m4/%s" % m4_file)
    return new_files


def copy_pandora_config(frompath="/usr/share/pandora-build"):
    new_files = []
    for subdir in ("m4","config"):
        for root, dirs, files in os.walk("%s/%s" % (frompath, subdir)):
            new_m4_files = [f for f in files if os.path.isfile("%s/%s/%s" %
            (frompath, subdir, f)) and f != 'pandora-plugin.ini']
            for m4_file in new_m4_files:
                new_files.append("%s/%s" % (subdir, m4_file))
                shutil.copy("%s/%s/%s" % (frompath, subdir, m4_file), "%s/%s" % (subdir, m4_file))
    return new_files

def copy_autoconf_archive(frompath="/usr/share/aclocal"):
    """Copy in needed files from autoconf archive"""
    new_files = []
    for m4 in autoconf_m4:
        new_files.append("m4/%s" % m4)
        shutil.copy("%s/%s" % (frompath, m4), "m4/%s" % m4)
    return new_files


def copy_pandora_files():
    new_files = []
    new_files.extend(copy_pandora_m4())
    new_files.extend(copy_pandora_config())
    new_files.extend(copy_autoconf_archive())
    return new_files

def get_pandora_version():
    """ Find the PANDORA_CANONICAL_VERSION """
    canonical_file = file("m4/pandora_canonical.m4", 'r')
    for line in canonical_file:
        if "PANDORA_CANONICAL_VERSION" in line:
            return line[line.rfind("[")+1:line.rfind("]")]

