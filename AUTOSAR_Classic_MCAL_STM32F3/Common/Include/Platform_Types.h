#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

/* Platform Specific Definitions */

/* CPU Type */
#define CPU_TYPE_8                                  8U
#define CPU_TYPE_16                                 16U
#define CPU_TYPE_32                                 32U
#define CPU_TYPE_64                                 64U

/* Bit Order */
#define LSB_FIRST                                   0U  /* Little Endian */
#define MSB_FIRST                                   1U  /* Big Endian */

/* Byte Order */
#define LOW_BYTE_FIRST                              0U  /* Little Endian */
#define HIGH_BYTE_FIRST                             1U  /* Big Endian */

/* Target Platform Configuration */
#define CPU_TYPE                                    CPU_TYPE_32
#define CPU_BIT_ORDER                               LSB_FIRST
#define CPU_BYTE_ORDER                              LOW_BYTE_FIRST

/* AUTOSAR Platform Dependent Types */
#ifndef _BOOLEAN_
    #define _BOOLEAN_
    typedef unsigned char   boolean;        /* TRUE or FALSE */
#endif

#ifndef _UINT8_
    #define _UINT8_
    typedef unsigned char   uint8;          /*           0 .. 255             */
#endif

#ifndef _UINT16_
    #define _UINT16_
    typedef unsigned short  uint16;         /*           0 .. 65535           */
#endif

#ifndef _UINT32_
    #define _UINT32_
    typedef unsigned long   uint32;         /*           0 .. 4294967295      */
#endif

#ifndef _SINT8_
    #define _SINT8_
    typedef signed char     sint8;          /*        -128 .. +127            */
#endif

#ifndef _SINT16_
    #define _SINT16_
    typedef signed short    sint16;         /*      -32768 .. +32767          */
#endif

#ifndef _SINT32_
    #define _SINT32_
    typedef signed long     sint32;         /* -2147483648 .. +2147483647     */
#endif

#ifndef _FLOAT32_
    #define _FLOAT32_
    typedef float           float32;
#endif

#ifndef _FLOAT64_
    #define _FLOAT64_
    typedef double          float64;
#endif

#endif /* PLATFORM_TYPES_H */
