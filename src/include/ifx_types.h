#ifndef _IFX_TYPES_H
#define _IFX_TYPES_H
/******************************************************************************

                               Copyright  2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

/** \file
   Basic data types.
*/

/** This is the character datatype. */
typedef char            IFX_char_t;
/** This is the integer datatype. */
typedef signed int      IFX_int_t;
/** This is the unsigned integer datatype. */
typedef unsigned int    IFX_uint_t;
/** This is the unsigned 8-bit datatype. */
typedef unsigned char   IFX_uint8_t;
/** This is the signed 8-bit datatype. */
typedef signed char     IFX_int8_t;
/** This is the unsigned 16-bit datatype. */
typedef unsigned short  IFX_uint16_t;
/** This is the signed 16-bit datatype. */
typedef signed short    IFX_int16_t;
/** This is the unsigned 32-bit datatype. */
typedef unsigned int    IFX_uint32_t;
/** This is the signed 32-bit datatype. */
typedef signed int      IFX_int32_t;
/** This is the float datatype. */
typedef float           IFX_float_t;
/** This is the void datatype. */
typedef void            IFX_void_t;

#if defined(WIN64) || (defined(IFX_64) && (IFX_64==1))

/** This is the unsigned long datatype. 
On 64 bit systems it is 8 byte wide.
*/
typedef unsigned long long IFX_ulong_t;
/** This is the signed long datatype. 
On 64 bit systems it is 8 byte wide.
*/
typedef signed long long IFX_long_t;
/** This is the size data type (32 or 64 bit) */
typedef size_t     IFX_size_t;

#else

/** This is the unsigned long datatype. 
On 32bit systems it is 4 byte wide.
*/
typedef unsigned long   IFX_ulong_t;
/** This is the signed long datatype. 
On 32bit systems it is 4 byte wide.
*/
typedef signed long     IFX_long_t;
/** This is the size data type (32 or 64 bit) */
typedef IFX_ulong_t     IFX_size_t;

#endif

/** This is the time data type (32 or 64 bit) */
typedef IFX_ulong_t     IFX_time_t;
/** Conversion pointer to unsigned values (32 or 64 bit) */
typedef IFX_ulong_t     IFX_uintptr_t;
/** Conversion pointer to signed values (32 or 64 bit) */
typedef IFX_long_t      IFX_intptr_t;

/** This is the volatile unsigned 8-bit datatype. */
typedef volatile IFX_uint8_t  IFX_vuint8_t;
/** This is the volatile signed 8-bit datatype. */
typedef volatile IFX_int8_t   IFX_vint8_t;
/** This is the volatile unsigned 16-bit datatype. */
typedef volatile IFX_uint16_t IFX_vuint16_t;
/** This is the volatile signed 16-bit datatype. */
typedef volatile IFX_int16_t  IFX_vint16_t;
/** This is the volatile unsigned 32-bit datatype. */
typedef volatile IFX_uint32_t IFX_vuint32_t;
/** This is the volatile signed 32-bit datatype. */
typedef volatile IFX_int32_t  IFX_vint32_t;
/** This is the volatile float datatype. */
typedef volatile IFX_float_t  IFX_vfloat_t;


/** A type for handling boolean issues. */
typedef enum {
   /** false */
   IFX_FALSE = 0,
   /** true */
   IFX_TRUE = 1
} IFX_boolean_t;


/**
   This type is used for parameters that should enable
   and disable a dedicated feature. */
typedef enum {
   /** disable */
   IFX_DISABLE = 0,
   /** enable */
   IFX_ENABLE = 1
} IFX_enDis_t;

/**
   This type is used for parameters that should enable
   and disable a dedicated feature. */
typedef IFX_enDis_t IFX_operation_t;

/**
   This type has two states, even and odd.
*/
typedef enum {
   /** even */
   IFX_EVEN = 0,
   /** odd */
   IFX_ODD = 1
} IFX_evenOdd_t;


/**
   This type has two states, high and low.
*/
typedef enum {
   IFX_LOW = 0,
   IFX_HIGH = 1
} IFX_highLow_t;

/**
   This type has two states, success and error
*/
typedef enum {
   IFX_ERROR   = (-1),
   IFX_SUCCESS = 0
} IFX_return_t;

/** NULL pointer */
#define IFX_NULL         ((void *)0)

#endif /* _IFX_TYPES_H */

