/* BEGIN LICENSE
 * END LICENSE */

/**
 * @file
 * @brief An Example sentence_name Class
 */

#ifndef include_guard
#define include_guard

#include "project_name/visibility.h"

#include <string>

open_namespace

/**
 * The main class implementing class_name
 */
class all_caps_project_name_API class_name
{
  const std::string name;

public:

  explicit class_name(const std::string &input);
  explicit class_name(const char *input);

  virtual ~class_name();

  /**
   * Get the name of this class_name
   */
  const std::string &get_name() const;

private:
  
  /**
   * Don't allow default construction.
   */
  class_name();

  /**
   * Don't allow copying of objects.
   */
  class_name(const class_name &);

  /**
   * Don't allow assignment of objects.
   */
  class_name& operator=(const class_name &);
};

/*
 * Public methods.
 */
inline class_name::~class_name()
{ }

inline const std::string &class_name::get_name() const
{
  return name;
}

close_namespace

#endif /* include_guard */
