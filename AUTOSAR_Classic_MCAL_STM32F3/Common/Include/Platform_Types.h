#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/* CPU Type: Represents the CPU family */
#define CPU_TYPE_8                                  8U
#define CPU_TYPE_16                                 16U
#define CPU_TYPE_32                                 32U
#define CPU_TYPE_64                                 64U

/* Bit Order: Specifies the order of bits in a byte or word */
#define LSB_FIRST                                   0U  /* Least Significant Bit first (Little Endian) */
#define MSB_FIRST                                   1U  /* Most Significant Bit first (Big Endian) */

/* Byte Order: Specifies the order of bytes in multi-byte data types */
#define LOW_BYTE_FIRST                              0U  /* Least significant byte stored at lower address (Little Endian) */
#define HIGH_BYTE_FIRST                             1U  /* Most significant byte stored at lower address (Big Endian) */

/* Target Platform Configuration (Specific to this project setup - STM32F3 Cortex-M4) */
#define CPU_TYPE                                    CPU_TYPE_32
#define CPU_BIT_ORDER                               LSB_FIRST       /* ARM Cortex-M is typically Little Endian */
#define CPU_BYTE_ORDER                              LOW_BYTE_FIRST  /* ARM Cortex-M is typically Little Endian */

/*==================================================================================================
*                                             TYPES
==================================================================================================*/

/* AUTOSAR Platform Dependent Boolean Type */
#ifndef _BOOLEAN_T_
    #define _BOOLEAN_T_ /* Guard to prevent redefinition */
    typedef unsigned char   boolean;        /**< @brief Standard boolean type (TRUE/FALSE).
                                             **         Typically maps to an 8-bit unsigned integer. */
#endif

/* AUTOSAR Platform Dependent Integer Types */
/* These types are defined to have fixed sizes for AUTOSAR compliance.
 * The C standard types (char, short, long) are mapped assuming common sizes
 * for ARM GCC on STM32F3 (32-bit Cortex-M4 architecture):
 * - char:  8-bit
 * - short: 16-bit
 * - long:  32-bit
 */

#ifndef _UINT8_T_
    #define _UINT8_T_
    typedef unsigned char   uint8;          /**< @brief Unsigned 8-bit integer. Range: 0 .. 255 */
#endif

#ifndef _UINT16_T_
    #define _UINT16_T_
    typedef unsigned short  uint16;         /**< @brief Unsigned 16-bit integer. Range: 0 .. 65535 */
#endif

#ifndef _UINT32_T_
    #define _UINT32_T_
    typedef unsigned long   uint32;         /**< @brief Unsigned 32-bit integer. Range: 0 .. 4294967295 */
#endif

#ifndef _SINT8_T_
    #define _SINT8_T_
    typedef signed char     sint8;          /**< @brief Signed 8-bit integer. Range: -128 .. +127 */
#endif

#ifndef _SINT16_T_
    #define _SINT16_T_
    typedef signed short    sint16;         /**< @brief Signed 16-bit integer. Range: -32768 .. +32767 */
#endif

#ifndef _SINT32_T_
    #define _SINT32_T_
    typedef signed long     sint32;         /**< @brief Signed 32-bit integer. Range: -2147483648 .. +2147483647 */
#endif

/* Optional: 64-bit integer types (if supported and needed) */
/*
#ifndef _UINT64_T_
    #define _UINT64_T_
    typedef unsigned long long uint64;     //**< @brief Unsigned 64-bit integer. * /
#endif

#ifndef _SINT64_T_
    #define _SINT64_T_
    typedef signed long long   sint64;     //**< @brief Signed 64-bit integer. * /
#endif
*/

/* AUTOSAR Platform Dependent Floating Point Types */
#ifndef _FLOAT32_T_
    #define _FLOAT32_T_
    typedef float           float32;        /**< @brief 32-bit floating point type (IEEE 754 single precision). */
#endif

#ifndef _FLOAT64_T_
    #define _FLOAT64_T_
    typedef double          float64;        /**< @brief 64-bit floating point type (IEEE 754 double precision). */
#endif

#endif /* PLATFORM_TYPES_H */
