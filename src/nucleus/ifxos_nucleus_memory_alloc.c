/******************************************************************************

                               Copyright  2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifdef NUCLEUS_PLUS

/** \file
   This file contains the IFXOS Layer implemantation for Nucleus 
   Memory Allocation.
*/

/* ============================================================================
   IFX Nucleus adaptation - Global Includes
   ========================================================================= */

#include <nucleus.h>
#include <stdlib.h>

#include "ifx_types.h"
#include "ifxos_memory_alloc.h"

#include "ifxos_sys_show.h"

/** memory pool of IFXOS */
NU_MEMORY_POOL IFXOS_System_Memory;


/* ============================================================================
   IFX Nucleus adaptation - memory handling, malloc
   ========================================================================= */

/** \addtogroup IFXOS_MEM_ALLOC_NUCLEUS 
@{ */

#if ( defined(IFXOS_HAVE_BLOCK_ALLOC) && (IFXOS_HAVE_BLOCK_ALLOC == 1) )
/**
   Nucleus - Allocate a continious memory block of the given size [byte]

\par Implementation
   - Allocates a memory block with the function "malloc"

\param
   memSize_byte   Size of the requested memory block [byte]

\return
   IFX_NULL in case of error, else
   pointer to the allocated memory block.

*/
IFX_void_t *IFXOS_BlockAlloc(
               IFX_size_t memSize_byte)
{
   IFX_void_t *pMemBlock = IFX_NULL;
   STATUS sts;

   if(memSize_byte)
   {
      sts = NU_Allocate_Memory(&IFXOS_System_Memory, &pMemBlock, memSize_byte, NU_NO_SUSPEND);
      if (sts != NU_SUCCESS) 
      {
         return IFX_NULL;
      }
      IFXOS_SYS_MEM_ALLOC_COUNT_INC(IFX_NULL);
   }

   return (pMemBlock);
}

/**
   Nucleus - Free the given memory block.

\par Implementation
   Free a memory block with the function "free"

\param
   pMemBlock   Points to the memory block to free.

\return
   NONE
*/
IFX_void_t IFXOS_BlockFree(
               IFX_void_t *pMemBlock)
{
   if (pMemBlock)
   {
      NU_Deallocate_Memory(pMemBlock);
      IFXOS_SYS_MEM_FREE_COUNT_INC(IFX_NULL);
   }

   return;
}
#endif      /* #if ( defined(IFXOS_HAVE_BLOCK_ALLOC) && (IFXOS_HAVE_BLOCK_ALLOC == 1) ) */


#if ( defined(IFXOS_HAVE_MEM_ALLOC) && (IFXOS_HAVE_MEM_ALLOC == 1) )
/**
   Nucleus - Allocate Memory Space from the OS

\par Implementation
   Allocates a memory block with the function "malloc"

\param
   memSize_byte   Size of the requested memory block [byte]

\return
   IFX_NULL in case of error, else
   pointer to the allocated memory block.
*/
IFX_void_t *IFXOS_MemAlloc(
               IFX_size_t memSize_byte)
{
   IFX_void_t *pMemBlock = IFX_NULL;
   STATUS sts;

   sts = NU_Allocate_Memory(&IFXOS_System_Memory, &pMemBlock, memSize_byte, NU_NO_SUSPEND);
   if (sts != NU_SUCCESS) 
   {
      return IFX_NULL;
   }

   return (pMemBlock);
}

/**
   Nucleus - Free Memory Space

\par Implementation
   Free a memory block with the function "free"

\param
   pMemBlock   Points to the memory block to free.

\return
   NONE
*/
IFX_void_t IFXOS_MemFree(
               IFX_void_t *pMemBlock)
{
   NU_Deallocate_Memory(pMemBlock);
}

#endif      /* #if ( defined(IFXOS_HAVE_MEM_ALLOC) && (IFXOS_HAVE_MEM_ALLOC == 1) ) */

/** @} */

#endif      /* #ifdef NUCLEUS_PLUS */
