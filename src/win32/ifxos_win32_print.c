/******************************************************************************

                               Copyright  2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#if defined(WIN32) && !defined(NUCLEUS_PLUS)

/** \file
   This file contains the IFXOS Layer implementation for Win32 - 
   Print Handling.
*/

/* ============================================================================
   IFX Win32 adaptation - Global Includes
   ========================================================================= */
#include <stdio.h>

#include "ifx_types.h"
#include "ifxos_print.h"

/* ============================================================================
   IFX Win32 adaptation - User Space, Print Handling
   ========================================================================= */

#if ( defined(IFXOS_HAVE_PRINT_STREAM) && (IFXOS_HAVE_PRINT_STREAM == 1) )
#  define IFXOS_FCT_DBG_PRINTF_GET    IFXOS_LocalDbgPrintout
#  define IFXOS_FCT_ERR_PRINTF_GET    IFXOS_LocalErrPrintout

IFXOS_STATIC IFX_int_t IFXOS_LocalDbgPrintout(const IFX_char_t *format, ...);
IFXOS_STATIC IFX_int_t IFXOS_LocalErrPrintout(const IFX_char_t *format, ...);
#else
#  define IFXOS_FCT_DBG_PRINTF_GET    printf
#  define IFXOS_FCT_ERR_PRINTF_GET    printf
#endif      /* #if ( defined(IFXOS_HAVE_PRINT_STREAM) && (IFXOS_HAVE_PRINT_STREAM == 1) ) */


/** \addtogroup IFXOS_PRINT_WIN32
@{ */
#if ( defined(IFXOS_HAVE_PRINT) && (IFXOS_HAVE_PRINT == 1) )
/** Debug printout function pointer - set used print function */
IFXOS_FCT_DbgPrintf IFXOS_fctDbgPrintf = IFXOS_FCT_DBG_PRINTF_GET;
/** Error printout function pointer - set used print function */
IFXOS_FCT_ErrPrintf IFXOS_fctErrPrintf = IFXOS_FCT_ERR_PRINTF_GET;

#if ( defined(IFXOS_HAVE_PRINT_STREAM) && (IFXOS_HAVE_PRINT_STREAM == 1) )
/** output stream for debug printouts */
IFXOS_File_t *pIFXOS_DbgPrintStream = IFX_NULL;
/** output stream for error printouts */
IFXOS_File_t *pIFXOS_ErrPrintStream = IFX_NULL;

/**
   Wrapper function for debug printout to change to out stream
   This function is form type "IFXOS_FCT_DbgPrintf" and wrap the default printf
   to use change the output stream.

\ret
   On success - number of written bytes.
*/
IFXOS_STATIC IFX_int_t IFXOS_LocalDbgPrintout(const IFX_char_t *format, ...)
{
   va_list     ap;
   IFX_int_t   nRet = 0;

   va_start(ap, format);
   nRet = vfprintf((pIFXOS_DbgPrintStream ? pIFXOS_DbgPrintStream : stdout), format, ap);
   va_end(ap);

   return nRet;
}

/**
   Wrapper function for error printout to change to out stream

   This function is form type "IFXOS_FCT_ErrPrintf" and wrap the default printf
   to use change the output stream.

\ret
   On success - number of written bytes.
*/
IFXOS_STATIC IFX_int_t IFXOS_LocalErrPrintout(const IFX_char_t *format, ...)
{
   va_list     ap;
   IFX_int_t   nRet = 0;

   va_start(ap, format);
   nRet = vfprintf((pIFXOS_ErrPrintStream ? pIFXOS_ErrPrintStream : stdout), format, ap);
   va_end(ap);

   return nRet;
}
#endif

#if ( defined(IFXOS_HAVE_PRINT_EXT_DBG_FCT) && (IFXOS_HAVE_PRINT_EXT_DBG_FCT == 1) )
/**
   Win32 - Set the user specific printout function for debug printouts.

\param
   fctExtDbg   - function pointer to the user debug print function

\return      
   NONE
*/
IFX_void_t IFXOS_PrintDbgFctSet(IFXOS_FCT_DbgPrintf fctExtDbg)
{
   if (fctExtDbg)
   {
      IFXOS_fctDbgPrintf = fctExtDbg;
   }
   else
   {
      IFXOS_fctDbgPrintf = IFXOS_FCT_DBG_PRINTF_GET;
   }
   return;
}
#endif

#if ( defined(IFXOS_HAVE_PRINT_EXT_ERR_FCT) && (IFXOS_HAVE_PRINT_EXT_ERR_FCT == 1) )
/**
   Win32 User - Set the user specific printout function for error printouts.

\param
   fctExtErr   - function pointer to the user error print function

\return      
   NONE
*/
IFX_void_t IFXOS_PrintErrFctSet(IFXOS_FCT_ErrPrintf fctExtErr)
{
   if (fctExtErr)
   {
      IFXOS_fctErrPrintf = fctExtErr;
   }
   else
   {
      IFXOS_fctErrPrintf = IFXOS_FCT_ERR_PRINTF_GET;
   }
   return;
}
#endif

#endif      /* #if ( defined(IFXOS_HAVE_PRINT) && (IFXOS_HAVE_PRINT == 1) ) */

/** @} */

#endif      /* #ifdef WIN32 */

