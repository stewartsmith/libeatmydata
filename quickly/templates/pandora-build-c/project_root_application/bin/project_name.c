/* BEGIN LICENSE
 * END LICENSE */

#include "config.h"

#include <stdio.h>

#include "project_name/project_name.h"

int main(int argc, char** argv)
{
  (void)argc; (void)argv;
  project_name_st *the_project_name= project_name_create("sentence_name");
  printf("%s\n", project_name_get_name(the_project_name));
  project_name_destroy(&the_project_name);
  return 0;
}
