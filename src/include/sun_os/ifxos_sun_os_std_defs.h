#ifndef _IFXOS_SUN_OS_STD_DEFS_H
#define _IFXOS_SUN_OS_STD_DEFS_H
/******************************************************************************

                               Copyright  2008
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

/** \file
   Function prototypes from standard headers for the IFXOS package.
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

#endif      /* #define _IFXOS_SUN_OS_STD_DEFS_H */

