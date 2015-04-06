/******************************************************************************

                               Copyright  2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifdef VXWORKS

/** \file
   This file contains the IFXOS Layer implementation for VxWorks Application 
   "Miscellaneous functions".
*/

/* ============================================================================
   IFX VxWorks adaptation - Global Includes
   ========================================================================= */

#include <vxworks.h>
#include <sysLib.h>
#include <rebootLib.h>

#include "ifx_types.h"
#include "ifxos_misc.h"

/* ============================================================================
   IFX VxWorks adaptation - User Space, Miscellaneous functions
   ========================================================================= */
/** \addtogroup IFXOS_MISC
@{ */

#if ( defined(IFXOS_HAVE_MISC) && (IFXOS_HAVE_MISC == 1) )
/**
   Reboot the board
*/   
IFX_void_t IFXOS_Reboot (void)
{
   reboot(BOOT_CLEAR);
   return;
}
#endif /* #if ( defined(IFXOS_HAVE_MISC) && (IFXOS_HAVE_MISC == 1) ) */

/** @} */

#endif      /* #ifdef VXWORKS */


