/* BEGIN LICENSE
 * END LICENSE */

#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "project_name.h"

struct project_name_st
{
  const char *name;
};

project_name_st *project_name_create(const char *name)
{
  project_name_st *the_project_name=
    (project_name_st *)malloc(sizeof(project_name_st));
  assert(the_project_name);
  the_project_name->name= name;
  return the_project_name;
}

void project_name_destroy(project_name_st **the_project_name)
{
  if (the_project_name)
  {
    free(*the_project_name);
    *the_project_name= NULL;
  }
}

const char *project_name_get_name(const project_name_st *the_project_name)
{
  if (the_project_name)
    return the_project_name->name;
  return NULL;
}

