#ifndef STD_TYPES_H
#define STD_TYPES_H

/* Standard Return Type */
typedef uint8 Std_ReturnType;

/* Standard Return Values */
#define E_OK            ((Std_ReturnType)0x00U)
#define E_NOT_OK        ((Std_ReturnType)0x01U)

/* Boolean Type */
#ifndef TRUE
    #define TRUE            1U
#endif
#ifndef FALSE
    #define FALSE           0U
#endif

typedef unsigned char   boolean;

/* Integer Types */
typedef unsigned char   uint8;          /*           0 .. 255             */
typedef signed char     sint8;          /*        -128 .. +127            */
typedef unsigned short  uint16;         /*           0 .. 65535           */
typedef signed short    sint16;         /*      -32768 .. +32767          */
typedef unsigned long   uint32;         /*           0 .. 4294967295      */
typedef signed long     sint32;         /* -2147483648 .. +2147483647     */

/* NULL Pointer */
#ifndef NULL_PTR
    #define NULL_PTR ((void *)0)
#endif

#endif /* STD_TYPES_H */
