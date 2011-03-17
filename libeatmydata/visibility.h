/*
 * Libeatmydata
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

#ifndef LIBEATMYDATA_VISIBILITY_H
#define LIBEATMYDATA_VISIBILITY_H

/**
 * Be sure to put LIBEATMYDATA_API in front of all public API symbols, or one of
 * the other macros as appropriate. The default visibility without a macro is
 * to be hidden (LIBEATMYDATA_LOCAL).
 */

#if defined(BUILDING_LIBEATMYDATA) && defined(HAVE_VISIBILITY)
# if defined(__GNUC__)
#  define LIBEATMYDATA_API __attribute__ ((visibility("default")))
#  define LIBEATMYDATA_INTERNAL_API __attribute__ ((visibility("hidden")))
#  define LIBEATMYDATA_API_DEPRECATED __attribute__ ((deprecated,visibility("default")))
#  define LIBEATMYDATA_LOCAL  __attribute__ ((visibility("hidden")))
# elif (defined(__SUNPRO_C) && (__SUNPRO_C >= 0x550)) || (defined(__SUNPRO_CC) && (__SUNPRO_CC >= 0x550))
#  define LIBEATMYDATA_API __global
#  define LIBEATMYDATA_INTERNAL_API __hidden
#  define LIBEATMYDATA_API_DEPRECATED __global
#  define LIBEATMYDATA_LOCAL __hidden
# elif defined(_MSC_VER)
#  define LIBEATMYDATA_API extern __declspec(dllexport)
#  define LIBEATMYDATA_INTERNAL_API extern __declspec(dllexport)
#  define LIBEATMYDATA_DEPRECATED_API extern __declspec(dllexport)
#  define LIBEATMYDATA_LOCAL
# endif
#else  /* defined(BUILDING_LIBEATMYDATA) && defined(HAVE_VISIBILITY) */
# if defined(_MSC_VER)
#  define SCALESTACK_API extern __declspec(dllimport)
#  define LIBEATMYDATA_INTERNAL_API extern __declspec(dllimport)
#  define LIBEATMYDATA_API_DEPRECATED extern __declspec(dllimport)
#  define LIBEATMYDATA_LOCAL
# else
#  define LIBEATMYDATA_API
#  define LIBEATMYDATA_INTERNAL_API
#  define LIBEATMYDATA_API_DEPRECATED
#  define LIBEATMYDATA_LOCAL
# endif /* defined(_MSC_VER) */
#endif  /* defined(BUILDING_LIBEATMYDATA) && defined(HAVE_VISIBILITY) */


#endif /* LIBEATMYDATA_VISIBILITY_H */
