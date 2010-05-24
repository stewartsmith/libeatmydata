/* BEGIN LICENSE
 * END LICENSE */

#include "config.h"

#include <drizzled/plugin/context.h>

#include "project_name.h"

using namespace drizzled;

namespace project_name_plugin
{

static int init(plugin::Context &context)
{
  context.add(new camel_case_name);
  return 0;
}

} /* namespace project_name_plugin */

DRIZZLE_PLUGIN(project_name_plugin::init, NULL);
