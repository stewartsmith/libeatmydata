# -*- coding: utf-8 -*-
# Copyright 2010 Didier Roche
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

from quickly import configurationhandler

def set_bzrbranch(bzr_branch):
    '''set default bzr branch from where to pull and push'''

    if not configurationhandler.project_config:
        configurationhandler.loadConfig()
    configurationhandler.project_config['bzrbranch'] = bzr_branch

def get_bzrbranch():
    '''get default bzr branch from where to pull and push'''
 
    if not configurationhandler.project_config:
        configurationhandler.loadConfig()

    try:
        bzr_branch = configurationhandler.project_config['bzrbranch']
    except KeyError:
        bzr_branch = None
    return bzr_branch
