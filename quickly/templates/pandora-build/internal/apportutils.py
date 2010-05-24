import os
import shutil
import subprocess

from gettext import gettext as _

import quickly
import quicklyutils

from lxml import etree

LPI_import_block = """
# optional Launchpad integration
# this shouldn't crash if not found as it is simply used for bug reporting
try:
    import LaunchpadIntegration
    launchpad_available = True
except:
    launchpad_available = False

"""

LPI_init_menu_block = """
        global launchpad_available
        if launchpad_available:
            # see https://wiki.ubuntu.com/UbuntuDevelopment/Internationalisation/Coding for more information
            # about LaunchpadIntegration
            helpmenu = self.builder.get_object('%(help_menu)s')
            if helpmenu:
                LaunchpadIntegration.set_sourcepackagename('%(project_name)s')
                LaunchpadIntegration.add_items(helpmenu, 0, False, True)
            else:
                launchpad_available = False"""

def update_apport(project_name, old_lp_project, new_lp_project):
    if not new_lp_project:
        return
    # crashdb file doesn't support spaces or dashes in the crash db name
    safe_project_name = project_name.replace(" ", "_").replace("-","_")
    crashdb_file = "%s-crashdb.conf"%project_name
    hook_file = "source_%s.py"%project_name


    pathname = quickly.templatetools.get_template_path_from_project()
    template_pr_path = os.path.join(os.path.abspath(pathname), "store",
                                    "apport")
    relative_crashdb_dir = os.path.join("etc", "apport", "crashdb.conf.d")
    relative_apport_dir = "apport"
    
    existing_crashdb = os.path.join(relative_crashdb_dir, crashdb_file)
    existing_hook = os.path.join(relative_apport_dir, hook_file)
    
    template_crashdb_dir = os.path.join(template_pr_path, relative_crashdb_dir)
    template_hook_dir = os.path.join(template_pr_path, relative_apport_dir)

    # if the project name has changed, or any of the files are missing, then
    # attempt to set up the apport configuration and hooks
    if  not old_lp_project == new_lp_project \
        or not os.path.isfile(existing_crashdb) \
        or not os.path.isfile(existing_hook):

        subst_existing = ((old_lp_project, new_lp_project),)
        subst_new = (   ("safe_project_name", safe_project_name),
                        ("project_name", project_name),
                        ("lp_project", new_lp_project))

        if os.path.isfile(existing_crashdb):
            print _("Updating project name references in existing apport crashdb configuration")
            quicklyutils.file_from_template(relative_crashdb_dir, crashdb_file, relative_crashdb_dir, subst_existing)
        elif os.path.isdir(template_crashdb_dir):
            print _("Creating new apport crashdb configuration")
            if not os.path.isdir(relative_crashdb_dir):
                os.makedirs(relative_crashdb_dir)
            quicklyutils.file_from_template(template_crashdb_dir, "project_name-crashdb.conf", relative_crashdb_dir, subst_new)

        if not os.path.isfile(existing_hook) and os.path.isdir(template_hook_dir):
            print _("Creating new apport hooks")
            if not os.path.isdir(relative_apport_dir):
                os.makedirs(relative_apport_dir)
            quicklyutils.file_from_template(template_hook_dir, "source_project_name.py", relative_apport_dir, subst_new)

def insert_lpi_if_required(project_name):
    existing_bin_filename = os.path.join("bin",project_name)
    camel_case_project_name = quickly.templatetools.get_camel_case_name(project_name)
    existing_ui_filename = os.path.join("data","ui", "%sWindow.ui"%camel_case_project_name)
    
    if os.path.isfile(existing_bin_filename) and os.path.isfile(existing_ui_filename):
        tree = etree.parse(existing_ui_filename)
        help_menu = find_about_menu(tree)
        
        if help_menu:
            existing_bin_file = file(existing_bin_filename, "r")
            existing_lines = existing_bin_file.readlines()
            existing_bin_file.close()
            new_lines = detect_or_insert_lpi(existing_lines, project_name, help_menu)
            if new_lines:
                print _("Adding launchpad integration to existing application")
                ftarget_file_name_out = file(existing_bin_file.name + '.new', 'w')
                ftarget_file_name_out.writelines(new_lines)
                ftarget_file_name_out.close()
                quickly.templatetools.apply_file_rights(existing_bin_file.name, ftarget_file_name_out.name)
                os.rename(ftarget_file_name_out.name, existing_bin_file.name)
            return True
    return False
        
def detect_or_insert_lpi(existing_lines, project_name, help_menu):
    integration_present = False
    import_insert_line = None
    init_insert_line = None
    current_line = 0
    for line in existing_lines:
        if "import LaunchpadIntegration" in line \
            or "if launchpad_available:" in line:
            integration_present = True
            break
        if not import_insert_line and "import gtk" in line:
            import_insert_line = current_line
        if not init_insert_line and "self.builder.connect_signals(self)" in line:
            init_insert_line = current_line
        current_line += 1
    
    if not integration_present \
        and import_insert_line \
        and init_insert_line \
        and import_insert_line < init_insert_line:
        init_menu_block = LPI_init_menu_block%{"project_name":project_name, "help_menu":help_menu}
        existing_lines = existing_lines[:import_insert_line+1] + \
            ["%s\n"%l for l in LPI_import_block.splitlines()] + \
            existing_lines[import_insert_line+1:init_insert_line+1] + \
            ["%s\n"%l for l in init_menu_block.splitlines()] + \
            existing_lines[init_insert_line+1:]
        return existing_lines
    else:
        return None


def find_about_menu(tree):
    """Finds the current help menu in the passed xml document by looking for the gtk-about element"""
    help_item = tree.xpath('//property[@name="label" and .="gtk-about"]/../../../@id')
    if len(help_item) == 1: # only one element matching this should be found
        return help_item[0]
    else:
        return None
