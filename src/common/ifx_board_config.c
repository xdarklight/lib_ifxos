/******************************************************************************

                              Copyright (c) 2009
                            Lantiq Deutschland GmbH
                     Am Campeon 3; 85579 Neubiberg, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

/** \file
   Routines to set the configuration of the board
*/


/* ===========================
   Includes
   =========================== */
#include "ifx_types.h"
#include "ifxos_debug.h"
#include "ifxos_memory_alloc.h"
#include "ifxos_file_access.h"
#include "ifxos_time.h"
#include "ifxos_std_defs.h"

/* routines to read from and write to ini-files          */
#include "ifx_ini_access.h"
/* routines to set the configuration of the board        */
#include "ifx_board_config.h"

/* ======================================================
   Local function declaration
   ====================================================== */
static IFX_int32_t CFG_DestinationID2Index(IFX_int32_t id);
static IFX_int32_t CFG_DestinationName2ID(IFX_char_t *name);
static IFX_return_t CFG_ReadRegisterFromFile(IFX_char_t *pSection, IFX_uint32_t keyIndex, IFX_char_t *filein, CFG_BOARD_REGISTER *pRegister);
static IFX_return_t CFG_SetBoardRegister(IFX_char_t *pSection, IFX_uint32_t keyIndex, CFG_BOARD_REGISTER *pRegister, IFX_enDis_t  testValue);

/* ===========================
   Local defines
   =========================== */
#define CFG_LIBRARY_VERSION      "0.0.0.3"

/* ===========================
   Global variables
   =========================== */
IFXOS_PRN_USR_MODULE_CREATE(CFG_MODULE, IFXOS_PRN_LEVEL_OFF);

/** Valid board destinations */
CFG_BOARD_DESTINATIONS           *Cfg_BoardDestinations;


/* ===========================
   Global variables
   =========================== */
static IFX_boolean_t bInitialized = IFX_FALSE;


/* ===========================
   Functions
   =========================== */
/**
   Get the version of the board configuration library

\param
   buffer:  where the version will be written
\param
   size:    size of the buffer

\return
   IFX_SUCCESS in case of success
   IFX_ERROR   in case of error
*/
IFX_return_t CFG_GetLibVersion(IFX_char_t *buffer, IFX_int32_t size)
{
   if ( (IFX_NULL == buffer) || (size <= 0) )
   {
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_HIGH, (CFG_PREFIX"ERROR - Get version failed!!\r\n\r"));
      return IFX_ERROR;
   }

   IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_LOW,
         (CFG_PREFIX"Version (%s)\r\n\r", CFG_LIBRARY_VERSION));

   strncpy(buffer, CFG_LIBRARY_VERSION, size);
   return IFX_SUCCESS;
}


/** Set the debug level of the board configuration library

\param
   debug_level: debug level for the printouts

\return
   IFX_SUCCESS in case of success
   IFX_ERROR   in case of error
*/
IFX_return_t CFG_SetDebugLevel(IFX_uint32_t debug_level)
{
   if ( (debug_level < IFXOS_PRN_LEVEL_LOW) || (debug_level > IFXOS_PRN_LEVEL_OFF) )
   {
      debug_level = IFXOS_PRN_LEVEL_HIGH;
   }

   /* set the trace level for the lib_board_config library */
   IFXOS_PRN_USR_LEVEL_SET(CFG_MODULE, debug_level);
   IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_LOW, (CFG_PREFIX"Debug level set to %d\n\r", debug_level));

   return IFX_SUCCESS;
}


/**
   Load the configuration file in memory
   
\param
   filename: name of the configuration file
\param
   ppFileIn: returns the ptr to the allocated block

\return
   Size of loaded file
   IFX_ERROR: in case of error if filesize was zero.
*/
IFX_size_t CFG_LoadFile(IFX_char_t *filename, IFX_char_t **ppFileIn)
{
   IFX_size_t filesize = IFX_ERROR;
   
#if ( defined(IFXOS_HAVE_FILE_ACCESS) && (IFXOS_HAVE_FILE_ACCESS == 1) )
   IFXOS_File_t *filefd;
   IFX_size_t ret;
   IFX_char_t *pLoadedCfgFile = IFX_NULL;
   IFXOS_stat_t stats;

   /* Open file for input: */
   if( (filefd = IFXOS_FOpen( filename, "r")) == IFX_NULL )
   {
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_HIGH, (CFG_PREFIX"ERROR - Open the file <%s> failed!!\r\n\r", filename));

      return IFX_ERROR;
   }

   /* get the size */
   if (IFXOS_Stat(filename, &stats))
   {

      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_HIGH, (CFG_PREFIX"ERROR - Get size for file <%s> failed!!\r\n\r", filename));

      IFXOS_FClose(filefd);
      return IFX_ERROR;
   }

   filesize = stats.st_size;

   IFXOS_PRN_USR_DBG_NL( CFG_MODULE, IFXOS_PRN_LEVEL_LOW, 
          (CFG_PREFIX"Load the file <%s> - Length %d\r\n\r", filename, (IFX_int_t)filesize));

   /* allocate memory */
   pLoadedCfgFile = IFXOS_MemAlloc(filesize);
   if (IFX_NULL == pLoadedCfgFile)
   {
      IFXOS_PRN_USR_DBG_NL( CFG_MODULE, IFXOS_PRN_LEVEL_HIGH, 
             (CFG_PREFIX"ERROR - Not enough memory to load <%s>!!\n\r", filename));

      IFXOS_FClose(filefd);
      return IFX_ERROR;
   }

   /* copy the data into memory and close the file */
   memset( (void*)pLoadedCfgFile, 0x00, filesize );

   if ( (ret = IFXOS_FRead(pLoadedCfgFile, 1, filesize, filefd)) != filesize)
   {
      IFXOS_PRN_USR_DBG_NL( CFG_MODULE, IFXOS_PRN_LEVEL_HIGH, 
             (CFG_PREFIX"ERROR - Read File <%s> incomplete!!\n\r", filename));

      IFXOS_FClose(filefd);
      IFXOS_MemFree(pLoadedCfgFile);

      return IFX_ERROR;
   }

   IFXOS_FClose(filefd);

   *ppFileIn = pLoadedCfgFile;

#endif /* #if ( defined(IFXOS_HAVE_FILE_ACCESS) && (IFXOS_HAVE_FILE_ACCESS == 1) ) */

   return filesize;
}


/**
   Set the requested board configuration from the file.

\param
   nBoardConfig:    number of the desired Board Configuration
\param
   pLoadedCfgFile:  already loaded configuration file to read from

\return
   IFX_SUCCESS in case of success
   IFX_ERROR   in case of error
*/
IFX_return_t CFG_SetBoardConfig(IFX_uint32_t nBoardConfig, IFX_char_t *pLoadedCfgFile)
{
   IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_LOW, (CFG_PREFIX"Set the board configuration %d\n\r", nBoardConfig));

   /* check if the library has been initialized */
   if (bInitialized != IFX_TRUE)
   {
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_HIGH, (CFG_PREFIX"ERROR - Library not initialized!!\n\r"));
      return IFX_ERROR;
   }

   /* check the given pointer on the configuration file */
   if (IFX_NULL == pLoadedCfgFile)
   {
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_HIGH, (CFG_PREFIX"ERROR - Wrong configuration file argument!!\n\r"));
      return IFX_ERROR;
   }

   /* print the configuration file's version section */
   CFG_PrintFileVersion(pLoadedCfgFile);

   /* check if the requested board configuration exists */
   if (CFG_DisplayConfigDescr(nBoardConfig, pLoadedCfgFile) != IFX_SUCCESS)
   {
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_HIGH, (CFG_PREFIX"ERROR - Board configuration %d not found!!\n\r", nBoardConfig));
      return IFX_ERROR;
   }

   /* set the valid required configuration to the board */
   if (CFG_SetBoardConfigFromFile(nBoardConfig, pLoadedCfgFile) <= 0)
   {
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_HIGH, (CFG_PREFIX"ERROR - Set the board configuration %d failed!!\n\r", nBoardConfig));
      return IFX_ERROR;
   }

   return IFX_SUCCESS;
}


/**
   Set the default board configuration from the file

\param
   filein:  already loaded configuration file to read from

\return
   IFX_SUCCESS in case of success
   IFX_ERROR   in case of error
*/
IFX_return_t CFG_SetDefaultBoardConfig(IFX_char_t *filein)
{
   IFX_int32_t nDefaultBoardConfig = -1;

   IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_LOW, (CFG_PREFIX"Set the default board configuration\n\r"));

   /* check if the library has been initialized */
   if (bInitialized != IFX_TRUE)
   {
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_HIGH, (CFG_PREFIX"ERROR - Library not initialized!!\n\r"));
      return IFX_ERROR;
   }

   /* read the default board configuration from the file */
   GET_KEY_INT(CFG_SECTION_BOARD_CONFIG, CFG_KEY_ACT_BOARD_CONFIG, nDefaultBoardConfig);
   if (nDefaultBoardConfig == -1)
   {
      /* default board config not found */
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_HIGH,
               (CFG_PREFIX"[%s]: ERROR - KEY \"%s\" not found!!\n\r",
                  CFG_SECTION_BOARD_CONFIG,
                  CFG_KEY_ACT_BOARD_CONFIG));
      return IFX_ERROR;
   }

   /* set the default board configuration */
   return CFG_SetBoardConfig(nDefaultBoardConfig, filein);
}


/**
   Set the board configuration from the given array

\param
   BoardConfigArray: pointer on the configuration register array.

\return
   IFX_SUCCESS in case of success
   IFX_ERROR   in case of error
*/
IFX_return_t CFG_SetThisBoardConfig(const CFG_BOARD_REGISTER *BoardConfigArray)
{
   IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_LOW,
            (CFG_PREFIX"[Array]: Load the board configuration from array\n\r"));

   /* check if the library has been initialized */
   if (bInitialized != IFX_TRUE)
   {
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_HIGH, (CFG_PREFIX"ERROR - Library not initialized!!\n\r"));
      return IFX_ERROR;
   }

   /* set the array board configuration */
   if (CFG_SetBoardConfigFromArray(BoardConfigArray) <= 0)
   {
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_HIGH, (CFG_PREFIX"ERROR - Set the board configuration from array failed!!\n\r"));
      return IFX_ERROR;
   }

   return IFX_SUCCESS;
}


/**
   Initialization of the configuration library

\param
   debug_level: debug level for the printouts

\return
   IFX_SUCCESS in case of success
   IFX_ERROR   in case of error
*/
IFX_return_t CFG_InitModule(CFG_BOARD_DESTINATIONS *BoardDestinations)
{
   IFX_int32_t i;

   /* get the board destinations */
   if (BoardDestinations == IFX_NULL)
   {
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_HIGH, 
            (CFG_PREFIX"ERROR - No board destinations given!!\n\r"));
      return IFX_ERROR;
   }

   /* save */
   Cfg_BoardDestinations = BoardDestinations;

   /* test if the structure has valid entry end */
   for (i = 0; i < MAX_BOARD_REGISTER_DESTINATIONS; i++)
   {
      if ( ( Cfg_BoardDestinations[i].id == CFG_TABLE_ENTRY_END_ID ) &&
           ( strcmp(Cfg_BoardDestinations[i].name, CFG_TABLE_ENTRY_END_NAME) == 0) )
      {
         break;
      }
   }

   if ( (0 == i) || (MAX_BOARD_REGISTER_DESTINATIONS == i) )
   {
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_HIGH,
            (CFG_PREFIX"ERROR - Invalid board destinations, no \"%s\"!!\n\r",
             CFG_TABLE_ENTRY_END_NAME));
      return IFX_ERROR;
   }

   bInitialized = IFX_TRUE;
   return IFX_SUCCESS;
}


/**
   Release of the board configuration library

\return
   IFX_SUCCESS in case of success
   IFX_ERROR   in case of error
*/
IFX_return_t CFG_ReleaseModule()
{
   IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_LOW, (CFG_PREFIX"Release the library\n\r"));

   Cfg_BoardDestinations = IFX_NULL;
   bInitialized = IFX_FALSE;

   return IFX_SUCCESS;
}


/* ======================================================
   Implementation: ini-file global cfg settings
   ====================================================== */
/**
   Return the corresponding index of the destination ID
   in the register read/write structure

\param
   id:  destination ID

\return
   index of the id in case of success
   -1 if id not found
*/
static IFX_int32_t CFG_DestinationID2Index(IFX_int32_t id)
{
   IFX_int32_t i;

   i = 0;
   while (Cfg_BoardDestinations[i].id != CFG_TABLE_ENTRY_END_ID)
   {
      if (Cfg_BoardDestinations[i].id != id)
      {
         i++;
      }
      else
      {
         return i;
      }
   }

   return -1;
}


/**
   Return the corresponding id of the destination name.

\param
   name: name of the destination.

\return
   id of the destination in case of success
   CFG_TABLE_ENTRY_END_ID if failes
*/
static IFX_int32_t CFG_DestinationName2ID(IFX_char_t *name)
{
   IFX_int32_t i;

   i = 0;
   while (Cfg_BoardDestinations[i].id != CFG_TABLE_ENTRY_END_ID)
   {
      if (strcmp(name, Cfg_BoardDestinations[i].name) == 0)
      {
         return Cfg_BoardDestinations[i].id;
      }
      else
      {
         i++;
      }
   }

   return CFG_TABLE_ENTRY_END_ID;
}


/**
   Print the configuration file's version section

\param
   filein:  pointer to the board config struct.

\return
   IFX_SUCCESS in case of success
   IFX_ERROR   in case of error
*/
IFX_return_t CFG_PrintFileVersion(IFX_char_t *filein)
{
   IFX_char_t buffer[256];

   /* read: cfg file version, only for print out */
   buffer[0] = '\0';
   if ( ( GET_KEY_STR( CFG_SECTION_VERSION, CFG_KEY_CFG_FILE_VERSION, "", buffer, sizeof(buffer) )
        ) > 0 )
   {
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_NORMAL, (CFG_PREFIX"[%s]: %s = %s\n\r",
                  CFG_SECTION_VERSION, CFG_KEY_CFG_FILE_VERSION, buffer));
   }

   /* read: cfg file board type, only for print out */
   buffer[0] = '\0';
   if ( ( GET_KEY_STR( CFG_SECTION_VERSION, CFG_KEY_CFG_FILE_TYPE, "", buffer, sizeof(buffer) )
        ) > 0 )
   {
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_NORMAL, (CFG_PREFIX"[%s]: %s = %s\n\r",
                  CFG_SECTION_VERSION, CFG_KEY_CFG_FILE_TYPE, buffer));
   }

   return IFX_SUCCESS;
}


/**
   Display the description of the board configuration .

\param
   filein:  pointer on the loaded file
\param
   nBoardConfig:  number of the board configuration
\return
   IFX_SUCCESS if the given board type is supported
   IFX_ERROR   if the given board type is not supported
*/
IFX_return_t CFG_DisplayConfigDescr(IFX_uint32_t nBoardConfig, IFX_char_t *filein)
{
   IFX_char_t buffer[256];
   IFX_char_t Section[CFG_SECTION_BOARD_CONFIG_SIZE];

   /* create section name */
   sprintf(Section, "%s%d", (IFX_char_t*)CFG_SECTION_BOARD_CONFIG_X, nBoardConfig);

   /* read: configuration name */
   buffer[0] = '\0';
   if ( ( GET_KEY_STR( Section, CFG_KEY_BOARD_CONFIG_NAME, "", buffer, sizeof(buffer) )
        ) <= 0 )
   {
      /* configuration name not found */
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_HIGH, (CFG_PREFIX"[%s]: ERROR - KEY \"%s\" not found!!\n\r",
                  Section, CFG_KEY_BOARD_CONFIG_NAME));
      return IFX_ERROR;
   }

   /* display: configuration name */
   IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_NORMAL, (CFG_PREFIX"[%s]: %s = %s\n\r",
               Section, CFG_KEY_BOARD_CONFIG_NAME, buffer));

   /* read: configuration short description, only for print out */
   buffer[0] = '\0';
   if ( ( GET_KEY_STR( Section, CFG_KEY_BOARD_CONFIG_S_DESCR, "", buffer, sizeof(buffer) )
        ) <= 0 )
   {
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_HIGH, (CFG_PREFIX"[%s]: WARNING - KEY \"%s\" not found\n\r",
                  Section, CFG_KEY_BOARD_CONFIG_S_DESCR));
   }
   else
   {
      /* display: configuration description */
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_NORMAL, (CFG_PREFIX"[%s]: %s = %s\n\r",
                  Section, CFG_KEY_BOARD_CONFIG_S_DESCR, buffer));
   }

   /* read: configuration description, only for print out */
   buffer[0] = '\0';
   if ( ( GET_KEY_STR( Section, CFG_KEY_BOARD_CONFIG_DESCR, "", buffer, sizeof(buffer) )
        ) <= 0 )
   {
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_HIGH, (CFG_PREFIX"[%s]: WARNING - KEY \"%s\" not found\n\r",
                  Section, CFG_KEY_BOARD_CONFIG_DESCR));
   }
   else
   {
      /* display: configuration description */
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_NORMAL, (CFG_PREFIX"[%s]: %s = %s\n\r",
                  Section, CFG_KEY_BOARD_CONFIG_DESCR, buffer));
   }

   return IFX_SUCCESS;
}


/* ===========================
   implementation: ini-file board register handling
  =========================== */


/**
   Read a board register entry form the cfg file.

\param
      pSection:    pointer to the ini-file section.
\param
      keyIndex:    index of the register line within the section.
\param
      filein:      configuration file to read from (already loaded)
\param
      pRegister:   register structure that will be filled

\return
   IFX_SUCCESS  entry found.
   IFX_ERROR    end of section : entry not found or "CFG_TABLE_ENTRY_END_NAME" found

\remark
   pRegister will be filled with the read values
   The Destination field is set to CFG_TABLE_ENTRY_END_ID in case of invalid destination
*/
static IFX_return_t CFG_ReadRegisterFromFile(IFX_char_t *pSection,
                                   IFX_uint32_t keyIndex,
                                   IFX_char_t *filein,
                                   CFG_BOARD_REGISTER *pRegister)
{
   IFX_char_t     *Dest, *Addr, *Value, *Mask, *tmp;
   IFX_char_t     regLine[CFG_BOARD_CONFIG_REGLINE_SIZE];
   IFX_char_t     pKeyCharIndex[16];

   /* create key and read the line */
   sprintf(pKeyCharIndex,"%d",keyIndex);
   if ( ( GET_KEY_STR( pSection, pKeyCharIndex, "", regLine, CFG_BOARD_CONFIG_REGLINE_SIZE )
        ) <= 0 )
   {
      /* end of config section reached --> leave */
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_LOW,
               (CFG_PREFIX"[%s-%02d]: End of the configuration section\n\r",
                  pSection, keyIndex));

      return IFX_ERROR;
   }

   /* interpret the ini-file line, get the token */
   Dest  = strtok_r(regLine, ",", &tmp);
   Addr  = strtok_r(IFX_NULL, ",", &tmp);
   Value = strtok_r(IFX_NULL, ",", &tmp);
   Mask  = strtok_r(IFX_NULL, ",", &tmp);

   IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_LOW,
            (CFG_PREFIX"[%s-%02d]: Read from file (Dest, Addr, Value, Mask) = (\"%s\", \"%s\", \"%s\", \"%s\")\n\r",
               pSection, keyIndex, Dest, Addr, Value, Mask));

   /* check the mandatory fields */
   if (Dest == IFX_NULL || Addr == IFX_NULL)
   {
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_HIGH,
               (CFG_PREFIX"[%s-%02d]: ERROR - corrupted register\n\r",
                  pSection, keyIndex));

      /* end of section still not reached, but entry not valid */
      pRegister->Destination = CFG_TABLE_ENTRY_END_ID;
      return IFX_SUCCESS;
   }

   /* check if a pause is required */
   if ( 0 == strcmp(Dest, CFG_TABLE_ENTRY_PAUSE_NAME) )
   {
      /* a pause is required */
      pRegister->Destination = CFG_TABLE_ENTRY_PAUSE_ID;
      pRegister->Value = (IFX_uint32_t) strtoul(Addr,  IFX_NULL, 0);
      return IFX_SUCCESS;
   }

   /* check the mandatory fields, all but Mask are mandatory */
   if (Value == IFX_NULL || Dest == IFX_NULL || Addr == IFX_NULL)
   {
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_HIGH,
               (CFG_PREFIX"[%s-%02d]: ERROR - corrupted register\n\r",
                  pSection, keyIndex));

      /* end of section still not reached, but entry not valid */
      pRegister->Destination = CFG_TABLE_ENTRY_END_ID;
      return IFX_SUCCESS;
   }

   /* check if the end of section has been reached */
   if ( 0 == strcmp(Dest, CFG_TABLE_ENTRY_END_NAME) )
   {
      /* end of config section reached --> leave */
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_LOW,
               (CFG_PREFIX"[%s-%02d]: End of the configuration section\n\r",
                  pSection, keyIndex));

      return IFX_ERROR;
   }

   else
   {
      /* check the register destination */
      pRegister->Destination = CFG_DestinationName2ID(Dest);
      if (pRegister->Destination == CFG_TABLE_ENTRY_END_ID)
      {
         /* unknown register destination --> return CFG_TABLE_ENTRY_END_ID */
         IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_HIGH,
                  (CFG_PREFIX"[%s-%02d]: ERROR - Unknown destination \"%s\" found!!\n\r",
                     pSection, keyIndex, Dest));
         return IFX_SUCCESS;
      }
   }

   /* convert register values */
   pRegister->Value   = (IFX_uint32_t)strtoul(Value, IFX_NULL, 16);
   pRegister->Address = (IFX_uint32_t)strtoul(Addr,  IFX_NULL, 16);
   if (Mask != IFX_NULL)
   {
      pRegister->Mask = (IFX_uint32_t)strtoul(Mask, IFX_NULL, 16);
   }
   else
   {
      pRegister->Mask = (IFX_uint32_t)-1;
   }

   return IFX_SUCCESS;
}


/**
   Set the hardware board register.

\param
   pSection:  pointer to the ini-file section.
\param
   keyIndex:  index of the register line within the section.
\param
   pRegister: valid register to write to.
\param
   bTestValue: if set, test the written value

\return
   IFX_SUCCESS if the new register value is written with success.
   IFX_ERROR otherwise.
*/
static IFX_return_t CFG_SetBoardRegister(IFX_char_t *pSection,
                                         IFX_uint32_t keyIndex,
                                         CFG_BOARD_REGISTER *pRegister,
                                         IFX_enDis_t  bTestValue)
{
   IFX_int32_t    dest, i;
   IFX_uint32_t   addr, mask, value;
   IFX_uint32_t   newRegVal, oldRegVal;

   /* destination is valid --> set to HW */
   mask  = pRegister->Mask;
   value = pRegister->Value;
   addr  = pRegister->Address;
   dest  = pRegister->Destination;

   /* check if a pause is required */
   if (CFG_TABLE_ENTRY_PAUSE_ID == dest)
   {
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_LOW,
               (CFG_PREFIX"[%s-%02d]: Sleep %d seconds\n\r",
                  pSection, keyIndex, value));
                  
      IFXOS_SecSleep(value);

      return IFX_SUCCESS;
   }

   /* get the index in the destination structure */
   i = CFG_DestinationID2Index(dest);
   if ( (i == CFG_TABLE_ENTRY_END_ID) || (i < 0) )
   {
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_LOW,
               (CFG_PREFIX"[%s-%02d]: ERROR - Wrong destination [%d]!!\n\r",
                  pSection, keyIndex, dest));
      return IFX_ERROR;      
   }

   IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_LOW,
            (CFG_PREFIX"[%s-%02d]: Set (Dest, Addr, Value, Mask) = (\"%s\"[%d], 0x%08x, 0x%08x, 0x%08x)\n\r",
               pSection, keyIndex, Cfg_BoardDestinations[dest].name, dest, addr, value, mask));

   /* check if there are read/write functions for the destination */
   if ( ( Cfg_BoardDestinations[i].read  == IFX_NULL) ||
        ( Cfg_BoardDestinations[i].write == IFX_NULL) )
   {
      /* warning cannot process register */
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_HIGH,
               (CFG_PREFIX"[%s-%02d]: ERROR - No rd/wr functions for destination \"%s\"[%d]!!\n\r",
                  pSection, keyIndex, Cfg_BoardDestinations[dest].name, dest));

      return IFX_ERROR;
   }

   /* read current register value */
   if (mask == (IFX_uint32_t)-1)
   {
      oldRegVal = 0;
   }

   else
   {
      if ( (Cfg_BoardDestinations[i].read)(addr, &oldRegVal) == IFX_ERROR )
      {
         IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_HIGH,
                  (CFG_PREFIX"[%s-%02d]: ERROR - Read old register value from \"%s\"[%d] at 0x%08x failed!!\n\r",
                     pSection, keyIndex, Cfg_BoardDestinations[dest].name, dest, addr));
         return IFX_ERROR;
      }
      else
      {
         oldRegVal = oldRegVal &~mask;
      }
   }

   newRegVal = value & mask;
   newRegVal |= oldRegVal;
   if ( (Cfg_BoardDestinations[i].write)(addr, newRegVal) == IFX_ERROR )
   {
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_HIGH,
               (CFG_PREFIX"[%s-%02d]: ERROR - Write new register value 0x%08x to \"%s\"[%d] at 0x%08x failed!!\n\r",
                  pSection, keyIndex, newRegVal, Cfg_BoardDestinations[dest].name, dest, addr));
      return IFX_ERROR;
   }

   if (IFX_ENABLE == bTestValue)
   {
      /* verify the new register's value */
      if ( ( (Cfg_BoardDestinations[i].read)(addr, &oldRegVal) != IFX_SUCCESS ) ||
           ( newRegVal != oldRegVal ) )
      {
         IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_HIGH,
                  (CFG_PREFIX"[%s-%02d]: ERROR - New value to \"%s\"[%d] at 0x%08x not verified!! (read 0x%08x != 0x%08x)\n\r",
                     pSection, keyIndex, Cfg_BoardDestinations[dest].name, dest, addr, oldRegVal, newRegVal));
         return IFX_ERROR;
      }

      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_LOW,
               (CFG_PREFIX"[%s-%02d]: New value verified, (Dest, Addr, Value) = (\"%s\"[%d], 0x%08x, 0x%08x)\n\r",
                  pSection, keyIndex, Cfg_BoardDestinations[dest].name, dest, addr, oldRegVal));

   }

   return IFX_SUCCESS;
}


/**
   Read the board configuration[nBoardConfig] from the ini-file
   and write it to the board.

\param
   nBoardConfig: index of the board configuration section.
\param
   indexStart:  start index of the register entries (default 0).
\param
   filein:      pointer to the configuration file (already loaded).

\return
   number of written registers.
*/
IFX_uint32_t CFG_SetBoardConfigFromFile(IFX_uint32_t nBoardConfig, IFX_char_t *filein)
{
   IFX_enDis_t   bTestValue;
   IFX_int32_t   indexStart, keyIndex;
   CFG_BOARD_REGISTER Register;
   IFX_char_t Section[CFG_SECTION_BOARD_CONFIG_SIZE];
   IFX_char_t ConfigName[CFG_BOARD_CONFIG_NAME_SIZE];

   /* create the appropriate section */
   sprintf(Section,"%s%d", CFG_SECTION_BOARD_CONFIG_X, nBoardConfig);

   /* read the name of the board configuration, only for print out */
   ConfigName[0] = '\0';
   GET_KEY_STR( Section, CFG_KEY_BOARD_CONFIG_NAME, "", ConfigName, sizeof(ConfigName) );

   /* read if the written register's value has to be tested */
   bTestValue = CFG_DEF_BOARD_CONFIG_TEST_VALUE;
   GET_KEY_INT(Section, CFG_KEY_BOARD_CONFIG_TEST_VALUE, bTestValue);

   /* read the start index of the ini-file register entries */
   indexStart = CFG_DEF_BOARD_CONFIG_START;
   GET_KEY_INT(Section, CFG_KEY_BOARD_CONFIG_START, indexStart);

   /* read a register line from file and write it to the board register */
   for(keyIndex = indexStart; keyIndex < MAX_BOARD_CONFIG_ENTRIES; keyIndex++)
   {
      Register.Destination = CFG_TABLE_ENTRY_END_ID;
      if ( ( CFG_ReadRegisterFromFile(Section, keyIndex, filein, &Register) != IFX_SUCCESS ) ||
           ( Register.Destination == CFG_TABLE_ENTRY_END_ID ) )
      {
         /* IFX_ERROR: end of section (line not found or CFG_TABLE_ENTRY_END_NAME found)
            IFX_SUCCESS: line found, check destination
            If the line was invalid (incomplete, destination not found or unknown)
            the destination was set to CFG_TABLE_ENTRY_END_ID */
         break;
      }

      /* destination is valid --> set to HW */
      CFG_SetBoardRegister(Section, keyIndex, &Register, bTestValue);
   }

   IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_LOW,
               (CFG_PREFIX"[%s]: %d register(s) set [%d->%d]\n\r",
                  Section, (keyIndex-indexStart), indexStart,
                  (keyIndex-1 > indexStart) ? (keyIndex-1) : indexStart ));

   return (keyIndex - indexStart);
}


/**
   Set a given configuration registers array to the board.

\param
   regArray:   pointer on the configuration register array.

\return
   number of written registers.
*/
IFX_uint32_t CFG_SetBoardConfigFromArray(const CFG_BOARD_REGISTER *regArray)
{
   IFX_int32_t    i;
   IFX_enDis_t    bTestValue;
   IFX_uint32_t   keyIndex;

   if (regArray == IFX_NULL)
   {
      /* wrong array */
      IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_HIGH,
               (CFG_PREFIX"ERROR - Wrong board configuration array!!\n\r"));
      return IFX_ERROR;
   }

   /* check if the written register's value has to be tested */
   bTestValue = CFG_DEF_BOARD_CONFIG_TEST_VALUE;

   /* read a register struct from array and write it to the board register */
   for(keyIndex = 0; keyIndex < MAX_BOARD_CONFIG_ENTRIES; keyIndex++)
   {
      if (regArray[keyIndex].Destination != CFG_TABLE_ENTRY_END_ID)
      {
         /* get the index in the destination structure */
         i = CFG_DestinationID2Index(regArray[keyIndex].Destination);

         /* check the register destination */
         if ( (i == CFG_TABLE_ENTRY_END_ID) || (i < 0) )
         {
            IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_LOW,
                     (CFG_PREFIX"[Array-%02d]: ERROR - Wrong destination [%d]!!\n\r",
                        keyIndex, regArray[keyIndex].Destination));
            break;      
         }

         /* destination is valid --> set to HW */
         CFG_SetBoardRegister("Array", keyIndex, (CFG_BOARD_REGISTER*) &regArray[keyIndex], bTestValue);
      }

      else
      {
         /* end of config reached --> leave */
         IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_LOW,
                  (CFG_PREFIX"[Array-%02d]: End of Array\n\r", keyIndex));
         break;
      }
   }

   IFXOS_PRN_USR_DBG_NL(CFG_MODULE, IFXOS_PRN_LEVEL_LOW,
               (CFG_PREFIX"[Array]: %d register(s) set [%d->%d]\n\r",
                  keyIndex, 0, (keyIndex-1 > 0) ? (keyIndex-1) : 0 ));

   return keyIndex;
}

