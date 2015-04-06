#ifndef _IFXOS_LIB_BOARD_CONFIG_H
#define _IFXOS_LIB_BOARD_CONFIG_H
/******************************************************************************

                               Copyright  2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

/** \file
   This file contains definitions the board configuration library. This functions 
   belong to the user (application) space.
*/

/** \defgroup IFXOS_LIB_BOARD_CONFIG Board Configuration.

   This Group contains the board configuration interface. 

\ingroup IFXOS_INTERFACE
*/

#ifdef __cplusplus
   extern "C" {
#endif

/* ===========================
   Includes
   =========================== */
#include "ifx_types.h"

/* ===========================
   Defines
   =========================== */
/**
   WARNING !!
    - After changing these defines, you must compile the library again.
    - These defines must be different from the possible valid destinations.
*/

/** Pause command */
#define CFG_TABLE_ENTRY_PAUSE_ID          622
#define CFG_TABLE_ENTRY_PAUSE_NAME        "Pause"

/** End of configuration */
#define CFG_TABLE_ENTRY_END_ID            -1
#define CFG_TABLE_ENTRY_END_NAME          "EndOfSection"


/* ===================================================
   Type definitions
   =================================================== */
/** Function pointer to write to a board register */
typedef IFX_return_t (*CFG_WRITE_BOARD_REGISTER)(IFX_uint32_t, IFX_uint32_t value);

/** Function pointer to read from a board register */
typedef IFX_return_t (*CFG_READ_BOARD_REGISTER)(IFX_uint32_t, IFX_uint32_t *);

/** Structure for different register destinations */
typedef struct S_CFG_REG_ID_NAME_INFOS
{
   /** register destination ID    */
   IFX_int32_t id;
   /** register destination name  */
   IFX_char_t  *name;
} CFG_REG_ID_NAME_INFOS;

/** Information structure for different board destinations */
typedef struct S_CFG_BOARD_DESTINATIONS
{
   /** register destination ID    */
   IFX_int32_t id;
   /** register destination name  */
   IFX_char_t  *name;
   /** destination read function  */
   CFG_READ_BOARD_REGISTER  read;
   /** destination write function */
   CFG_WRITE_BOARD_REGISTER write;
} CFG_BOARD_DESTINATIONS;

/** Structure for different register read/write functions */
typedef struct S_CFG_REG_RD_WR_INFOS
{
   /** register destination ID    */
   IFX_int32_t id;
   /** destination read function  */
   CFG_READ_BOARD_REGISTER  read;
   /** destination write function */
   CFG_WRITE_BOARD_REGISTER write; 
} CFG_REG_RD_WR_INFOS;

/** Structure for different register accesses to the board */
typedef struct S_CFG_BOARD_REGISTER
{
   /** register destination ID    */
   IFX_uint32_t   Destination;
   /** address of the register    */
   IFX_uint32_t   Address;
   /** value to set */
   IFX_uint32_t   Value;
   /** set only the bits set in the mask */
   IFX_uint32_t   Mask;
} CFG_BOARD_REGISTER;


/* ==================================
   Global functions Declarations
   ================================== */
/** Get the version of the board configuration library */
IFX_return_t CFG_GetLibVersion(IFX_char_t *buffer, IFX_int32_t size);

/** Set the debug level of the board configuration library */
IFX_return_t CFG_SetDebugLevel(IFX_uint32_t debug_level);

/** Initialize the configuration library */
IFX_return_t CFG_InitModule(CFG_BOARD_DESTINATIONS *BoardDestinations);

/** Release the board configuration library */
IFX_return_t CFG_ReleaseModule(IFX_void_t);

/** Load the configuration file in memory */
IFX_size_t CFG_LoadFile(IFX_char_t *filename, IFX_char_t **ppFileIn);

/** Set the default board configuration from the file */
IFX_return_t CFG_SetDefaultBoardConfig(IFX_char_t  *pLoadedCfgFile);

/** Set the board configuration from the given array */
IFX_return_t CFG_SetThisBoardConfig(const CFG_BOARD_REGISTER *BoardConfigArray);

/** Set the requested board configuration from the file */
IFX_return_t CFG_SetBoardConfig(IFX_uint32_t nBoardConfig, IFX_char_t *pLoadedCfgFile);

/** print the configuration file's version section */
IFX_return_t CFG_PrintFileVersion(IFX_char_t *filein);

/** check if the requested board configuration exists. */
IFX_return_t CFG_DisplayConfigDescr(IFX_uint32_t nBoardConfig, IFX_char_t *filein);

/** set a given configuration registers array to the board. */
IFX_uint32_t CFG_SetBoardConfigFromArray(const CFG_BOARD_REGISTER *regArray );

IFX_uint32_t CFG_SetBoardConfigFromFile(IFX_uint32_t nBoardConfig, IFX_char_t *filein);
/* ======================================================
   Global library definitions
   ====================================================== */

#define CFG_PREFIX   "-->CFG_Library: "

/* ============================================================== */
/**
   Max number of board register entries within the BOARD_CONFIG_X section.
*/
#define MAX_BOARD_CONFIG_ENTRIES          1000
#define MAX_BOARD_REGISTER_DESTINATIONS   20

/* ======================================================
   ini-file macro definitions
   ====================================================== */
#define GET_KEY_INT(section,key,variable) variable = GetKeyInt (section, key, variable, filein)
#define GET_KEY_STR(psec, pkey, pdef, pdest, destsize) GetKeyString(psec, pkey, pdef, pdest, destsize, filein)


/* ==================================================== */
/* definitions for the ini-file sections and keywords   */
/* ==================================================== */
/* ============================================================== */
/**
   CFG file section: "VERSION".
   Example:
      [VERSION]
      ConfigFileType=SDFE4_EB
      ConfigFileVersion=0.0.1
*/
#define CFG_SECTION_VERSION        "VERSION"
#define CFG_KEY_CFG_FILE_VERSION   "ConfigFileVersion"
#define CFG_KEY_CFG_FILE_TYPE      "ConfigFileType"

/* ============================================================== */
/**
   CFG file section: "BOARD_CONFIG".
   Example:
      [BOARD_CONFIG]
      DefaultBoardConfig=0
*/
#define CFG_SECTION_BOARD_CONFIG   "BOARD_CONFIG"
#define CFG_KEY_ACT_BOARD_CONFIG   "DefaultBoardConfig"

/* ============================================================== */
/**
   CFG file section: "BOARD_CONFIG_x".
   Example:
      [BOARD_CONFIG_1]
      ConfigName=<Config name, used within the select box>
      ConfigDescription=<Description, displayed after selection>
      ConfigShortDesc=<short description, displayed within a list>
      bTestValue=<0 or 1, compare register's value with the written one>
      ConfigStart=<start index of the first register entry>
*/
#define CFG_SECTION_BOARD_CONFIG_X      "BOARD_CONFIG_"
#define CFG_KEY_BOARD_CONFIG_NAME       "ConfigName"
#define CFG_KEY_BOARD_CONFIG_DESCR      "ConfigDescription"
#define CFG_KEY_BOARD_CONFIG_S_DESCR    "ConfigShortDesc"

/** If set - compare register's value with the written one */
#define CFG_KEY_BOARD_CONFIG_TEST_VALUE "bTestValue"
#define CFG_DEF_BOARD_CONFIG_TEST_VALUE IFX_DISABLE

#define CFG_KEY_BOARD_CONFIG_START      "ConfigStart"
#define CFG_DEF_BOARD_CONFIG_START       0

#define CFG_SECTION_BOARD_CONFIG_SIZE   64
#define CFG_BOARD_CONFIG_NAME_SIZE      128
#define CFG_BOARD_CONFIG_DESCR_SIZE     512
#define CFG_BOARD_CONFIG_S_DESCR_SIZE   256
#define CFG_BOARD_CONFIG_REGLINE_SIZE   256

#ifdef __cplusplus
}
#endif

#endif /* ifndef _IFXOS_LIB_BOARD_CONFIG_H */
