# -*- coding: utf-8 -*-
# Copyright 2010 Didier Roche, some part based on
# Martin Pitt <martin.pitt@ubuntu.com>
# and http://blog.launchpad.net/api/recipe-for-uploading-files-via-the-api
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

import datetime
import os
import sys
import subprocess

import gettext
from gettext import gettext as _
gettext.textdomain('quickly')

#TODO: see if 0 release in the project

def create_release(project, version):
    '''Create new release and milestone for LP project.

    If more than one release already exists, take the last one.'''

    release_date = datetime.date.today().strftime('%Y-%m-%d')
    if len(project.series) == 0:
        print "No serie is not supported right now. Not uploading the tarball for you"
        sys.exit(1)
    serie = project.series[project.series.total_size - 1]
    milestone = serie.newMilestone(name=version,
                                   date_targeted=release_date)
    return milestone.createProductRelease(date_released=release_date)

def push_tarball_to_launchpad(project, version, tarball, changelog_content):
    '''Push new tarball to Launchpad, create release if needed and sign it'''

        # Find the release in the project's releases collection.
    release = None
    for rel in project.releases:
        if rel.version == version:
            release = rel
            break
    if not release:
        release = create_release(project, version)

    # Get the file contents.
    file_content = open(tarball, 'r').read()
    # Get the signature, if available.
    signature = tarball + '.asc'
    if not os.path.exists(signature):
        print _('Calling GPG to create tarball signature...')
        if subprocess.call(['gpg', '--armor', '--sign', '--detach-sig',
                            tarball]) != 0:
            sys.stderr.write(_('Signing the tarball failed, not uploading the ' \
                               'signature'))

    if os.path.exists(signature):
        signature_content = open(signature, 'r').read()
    else:
        signature_content = None

    # Create a new product release file.
    release.add_file(filename=tarball, description='%s tarball' % version,
            file_content=file_content, content_type='appplication/x-gzip',
            file_type='Code Release Tarball', signature_filename=signature,
            signature_content=signature_content)

    if not changelog_content:
        changelog_content = _('New release available: %s') % version
    else:
        changelog_content = "\n".join(changelog_content)
    release.changelog = changelog_content
    release.release_notes = changelog_content
    try:
        release.lp_save()
    except HTTPError, e:
        print(_('An error happened during tarball upload:'), e.content)
        sys.exit(1)

