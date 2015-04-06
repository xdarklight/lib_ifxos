#ifndef _IFXOS_VXWORKS_COMMON_H
#define _IFXOS_VXWORKS_COMMON_H
/******************************************************************************

                               Copyright  2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifdef VXWORKS
/** \file
   This file contains common definitions used for the VxWorks OS adaption
*/

/* ============================================================================
   IFX VxWorks adaptation
   ========================================================================= */

/** \defgroup IFXOS_IF_VXWORKS Defines for VxWorks Adaptaion

   This Group contains the VxWorks specific definitions and function.

\par VxWorks Endianess
   Under VxWorks the following macros must be set form outside or by
   external VxW headers (see <netinet/in.h>)
   _LITTLE_ENDIAN,  _BIG_ENDIAN, _BYTE_ORDER

\attention
   The "__LITTLE_ENDIAN", "__BIG_ENDIAN" and "__BYTE_ORDER" are currently used 
   within some external header files.

\ingroup IFXOS_INTERFACE
*/

#ifdef __cplusplus
   extern "C" {
#endif

/* ============================================================================
   IFX VxWorks adaptation - Includes
   ========================================================================= */
#include <vxworks.h>
#include <iosLib.h>        /* DEV_HDR */
#include <sys/ioctl.h>     /* _IO */
#include <netinet/in.h>

/* ============================================================================
   IFX VxWorks adaptation - Macro definitions
   ========================================================================= */

/** \addtogroup IFXOS_IF_VXWORKS
@{ */
#ifndef _LITTLE_ENDIAN
#  error "Missing definition for Little Endian"
#endif

#ifndef _BIG_ENDIAN
#  error "Missing definition for Big Endian"
#endif

#ifndef _BYTE_ORDER
#  error "Missing definition for Byte Order"
#endif

#ifndef __LITTLE_ENDIAN
#  define __LITTLE_ENDIAN _LITTLE_ENDIAN
#else
#  if (__LITTLE_ENDIAN != _LITTLE_ENDIAN)
#     error "macro define __LITTLE_ENDIAN missmatch"
#  endif
#endif

#ifndef __BIG_ENDIAN
#  define __BIG_ENDIAN    _BIG_ENDIAN
#else
#  if (__BIG_ENDIAN != _BIG_ENDIAN)
#     error "macro define __BIG_ENDIAN missmatch"
#  endif
#endif

#ifndef __BYTE_ORDER
#  if (_BYTE_ORDER == _LITTLE_ENDIAN)
#     define __BYTE_ORDER                 __LITTLE_ENDIAN
#  elif (_BYTE_ORDER == _BIG_ENDIAN )
#     define __BYTE_ORDER                 __BIG_ENDIAN
#  else
#     error "Unknown System Byteorder!"
#  endif
#endif

#ifndef _IFXOS_COMMON_H
#  error "missing IFXOS endian defines, include 'ifx_common.h' at first"
#endif

#if (__BYTE_ORDER == __LITTLE_ENDIAN)
#  define IFXOS_BYTE_ORDER                IFXOS_LITTLE_ENDIAN
#elif (__BYTE_ORDER == __BIG_ENDIAN )
#  define IFXOS_BYTE_ORDER                IFXOS_BIG_ENDIAN
#else
#  error "no matching __BYTE_ORDER found"
#endif

/** @} */

#ifdef __cplusplus
}
#endif
#endif      /* #ifdef VXWORKS */
#endif      /* #ifndef _IFXOS_VXWORKS_COMMON_H */

