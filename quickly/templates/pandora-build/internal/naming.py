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

from quickly import templatetools
import quicklyutils

class naming_context(object):

    def __init__(self, base_name):
        self._base_name = base_name
    
        # throws templatetools.bad_project_name, but we're going to let it
        # propogate
        self._project_name = templatetools.python_name(templatetools.quickly_name(base_name))


        self._all_caps_name = self._project_name.upper()
        self._sentence_name, self._camel_case_name = quicklyutils.conventional_names(self._project_name)

    @property
    def base_name(self):
        return self._base_name

    @property
    def project_name(self):
        return self._project_name

    @property
    def all_caps_name(self):
        return self._all_caps_name

    @property
    def sentence_name(self):
        return self._sentence_name

    @property
    def camel_case_name(self):
        return self._camel_case_name
