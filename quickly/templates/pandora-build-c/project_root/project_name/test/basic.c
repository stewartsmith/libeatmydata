/* BEGIN LICENSE
 * END LICENSE */

#include "config.h"

#include <assert.h>

#include <project_name/project_name.h>

/**
 * @TODO Actually write a real test suite here
 */
int main(int argc, char **argv)
{
  (void)argc; (void)argv;
  project_name_st *the_project_name= project_name_create("test_sentence_name");
  assert(the_project_name);
  return 0;
}
