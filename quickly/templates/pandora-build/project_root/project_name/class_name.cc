/* BEGIN LICENSE
 * END LICENSE */

#include "config.h"

#include "class_name.h"

open_namespace

class_name::class_name(const std::string &input) :
  name(input)
{ }

class_name::class_name(const char *input) :
  name(input)
{ }

close_namespace
