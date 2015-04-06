/******************************************************************************

                               Copyright  2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

/* ============================================================================
   Description : IFX Linux adaptation - Terminal IO System (Application Space)
   ========================================================================= */

#ifdef LINUX

/** \file
   This file contains the IFXOS Layer implementation for LINUX Application 
   "Terminal IO System".
*/

/* ============================================================================
   IFX Linux adaptation - Global Includes
   ========================================================================= */
#define _GNU_SOURCE     1
#include <features.h>

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "ifx_types.h"
#include "ifxos_termios.h"

#ifdef IFXOS_STATIC
#undef IFXOS_STATIC
#endif

#ifdef IFXOS_DEBUG
#define IFXOS_STATIC
#else
#define IFXOS_STATIC   static
#endif

/* ============================================================================
   IFX Linux adaptation - Application Space, Terminal IO System
   ========================================================================= */

/** \addtogroup IFXOS_TERMIOS_LINUX_APPL
@{ */
#if ( defined(IFXOS_HAVE_TERMIOS) && (IFXOS_HAVE_TERMIOS == 1) )

IFXOS_STATIC struct termios stored_stdin_settings,
                            stored_stdout_settings;


/**
   Disable the local echo of the console.
*/   
IFX_void_t IFXOS_EchoOff (void)
{
   struct termios new_settings;

   tcgetattr(fileno(stdout),&stored_stdout_settings);
   new_settings = stored_stdout_settings;
   new_settings.c_lflag &= (~ECHO);
   tcsetattr(fileno(stdout),TCSANOW,&new_settings);
   return;
}

/**
   Enable the local echo of the console.
*/   
IFX_void_t IFXOS_EchoOn (void)
{
   tcsetattr(fileno(stdout),TCSANOW,&stored_stdout_settings);
   return;
}

/**
   Enable the console line mode.
   In this mode the input from the device is available only after receiving NEWLINE . 
   This allows to modify the command line until the Enter key is pressed.
*/   
IFX_void_t IFXOS_KeypressSet (void)
{
   struct termios new_settings;

   tcgetattr(fileno(stdin),&stored_stdin_settings);

   new_settings = stored_stdin_settings;

   /* Disable canonical mode */
   new_settings.c_lflag &= ~(ICANON);
   /* set buffer size to 0 byte / timeout 100 ms */
   new_settings.c_cc[VTIME] = 10;
   new_settings.c_cc[VMIN] = 0;

   tcsetattr(fileno(stdin),TCSANOW,&new_settings);
   return;
}

/**
   Disable the console line mode. 
   Plesae refer to \ref IFXOS_KeypressSet .
*/   
IFX_void_t IFXOS_KeypressReset (void)
{
   tcsetattr(fileno(stdin),TCSANOW,&stored_stdin_settings);
   return;
}
#endif   /* #if ( defined(IFXOS_HAVE_TERMIOS) && (IFXOS_HAVE_TERMIOS == 1) ) */

/** @} */

#endif      /* #ifdef LINUX */

