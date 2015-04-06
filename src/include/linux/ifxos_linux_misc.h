#ifndef _IFXOS_LINUX_MISC_H
#define _IFXOS_LINUX_MISC_H
/******************************************************************************

                               Copyright  2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifdef LINUX

/** \file
   This file contains LINUX definitions of Miscellaneous functions.
*/

/** \defgroup IFXOS_MISC_LINUX_APPL Miscellaneous functions (Linux User Space)

   This Group contains the LINUX "Miscellaneous" definitions and function. 


\ingroup IFXOS_LAYER_LINUX
*/

#ifdef __cplusplus
   extern "C" {
#endif

/* ============================================================================
   IFX LINUX adaptation - Includes
   ========================================================================= */

/* ============================================================================
   IFX LINUX adaptation - supported features
   ========================================================================= */

/** IFX LINUX adaptation - User support "Miscellaneous functions" */
#ifndef IFXOS_HAVE_MISC
#  define IFXOS_HAVE_MISC                       1
#endif

/* ============================================================================
   IFX LINUX adaptation - types
   ========================================================================= */

#ifdef __cplusplus
}
#endif
#endif      /* #ifdef LINUX */
#endif      /* #ifndef _IFXOS_LINUX_MISC_H */


