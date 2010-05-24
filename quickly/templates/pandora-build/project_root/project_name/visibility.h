/*
 * sentence_name
 *
 * Copyright (C) 2010 Eric Day (eday@oddments.org)
 * All rights reserved.
 *
 * Use and distribution licensed under the BSD license. See the
 * COPYING file in the root project directory for full text.
 */

/**
 * @file
 * @brief Common Macro Definitions
 */

#ifndef all_caps_name_VISIBILITY_H
#define all_caps_name_VISIBILITY_H

/**
 * Be sure to put all_caps_name_API in front of all public API symbols, or one of
 * the other macros as appropriate. The default visibility without a macro is
 * to be hidden (all_caps_name_LOCAL).
 */

#if defined(BUILDING_all_caps_name) && defined(HAVE_VISIBILITY)
# if defined(__GNUC__)
#  define all_caps_name_API __attribute__ ((visibility("default")))
#  define all_caps_name_INTERNAL_API __attribute__ ((visibility("hidden")))
#  define all_caps_name_API_DEPRECATED __attribute__ ((deprecated,visibility("default")))
#  define all_caps_name_LOCAL  __attribute__ ((visibility("hidden")))
# elif (defined(__SUNPRO_C) && (__SUNPRO_C >= 0x550)) || (defined(__SUNPRO_CC) && (__SUNPRO_CC >= 0x550))
#  define all_caps_name_API __global
#  define all_caps_name_INTERNAL_API __hidden
#  define all_caps_name_API_DEPRECATED __global
#  define all_caps_name_LOCAL __hidden
# elif defined(_MSC_VER)
#  define all_caps_name_API extern __declspec(dllexport)
#  define all_caps_name_INTERNAL_API extern __declspec(dllexport)
#  define all_caps_name_DEPRECATED_API extern __declspec(dllexport)
#  define all_caps_name_LOCAL
# endif
#else  /* defined(BUILDING_all_caps_name) && defined(HAVE_VISIBILITY) */
# if defined(_MSC_VER)
#  define SCALESTACK_API extern __declspec(dllimport)
#  define all_caps_name_INTERNAL_API extern __declspec(dllimport)
#  define all_caps_name_API_DEPRECATED extern __declspec(dllimport)
#  define all_caps_name_LOCAL
# else
#  define all_caps_name_API
#  define all_caps_name_INTERNAL_API
#  define all_caps_name_API_DEPRECATED
#  define all_caps_name_LOCAL
# endif /* defined(_MSC_VER) */
#endif  /* defined(BUILDING_all_caps_name) && defined(HAVE_VISIBILITY) */


#endif /* all_caps_name_VISIBILITY_H */
