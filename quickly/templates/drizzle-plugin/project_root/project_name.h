/* BEGIN LICENSE
 * END LICENSE */

#ifndef PLUGIN_all_caps_name_all_caps_name_H
#define PLUGIN_all_caps_name_all_caps_name_H

#include <drizzled/plugin/plugin_type.h>

namespace project_name
{

class camel_case_name :
  public drizzled::plugin::type_camel_case_name
{
  camel_case_name(camel_case_name&);
  camel_case_name operator=(camel_case_name&);
public:
  camel_case_name();

  virtual ~camel_case_name();
}

inline camel_case_name::~camel_case_name()
{ }

} /* namespace project_name */

#endif /* PLUGIN_all_caps_name_all_caps_name_H */
