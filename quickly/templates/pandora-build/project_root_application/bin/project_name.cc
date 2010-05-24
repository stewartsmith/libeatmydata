/* BEGIN LICENSE
 * END LICENSE */

#include "config.h"

#include <iostream>

#include "project_name/class_name.h"

int main(int, char**)
{
  const project_name::class_name the_class_name("sentence_name");
  std::cout << the_class_name.get_name() << std::endl;
  return 0;
}
