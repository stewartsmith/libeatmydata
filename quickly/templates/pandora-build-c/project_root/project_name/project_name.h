/* BEGIN LICENSE
 * END LICENSE */

/**
 * @file
 * @brief A Basic Example Class
 */

#ifndef all_caps_name_all_caps_name_H
#define all_caps_name_all_caps_name_H

#include "project_name/visibility.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Forward declaration for main struct implementing project_name
 */
typedef struct project_name_st project_name_st;

all_caps_name_API
project_name_st *project_name_create(const char *name);

all_caps_name_API
void project_name_destroy(project_name_st **the_project_name);

all_caps_name_API
const char *project_name_get_name(const project_name_st *the_project_name);


#ifdef __cplusplus
}
#endif

#endif /* all_caps_name_all_caps_name_H */
