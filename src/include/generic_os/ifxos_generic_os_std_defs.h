#ifndef _IFXOS_GENERIC_OS_STD_DEFS_H
#define _IFXOS_GENERIC_OS_STD_DEFS_H
/******************************************************************************

                              Copyright (c) 2009
                            Lantiq Deutschland GmbH
                     Am Campeon 3; 85579 Neubiberg, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

/** \file
   Function proto-types for generic OS processing of the IFXOS based SW.
*/

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>      /* sprintf */
#include <stdlib.h>     /* strtoul */
#include <string.h>     /* strtok_r, strcmp, strncmp, strlen, memcpy, memset, NULL */
#include <ctype.h>      /* isspace */
#include <errno.h>      /* errno */

/* #define strtok_r(a,b,c) strtok(a,b) */

#ifdef __cplusplus
/* extern "C" */
}
#endif

#endif      /* #define _IFXOS_GENERIC_OS_STD_DEFS_H */

