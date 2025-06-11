#ifndef STD_TYPES_H
#define STD_TYPES_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
/* Include Platform_Types.h for platform-specific type definitions (boolean, uint8, etc.) */
/* This file is expected to be in the same directory or an include path must be provided. */
/* Assuming it's in the same "Common/Include" directory for this project structure. */
#include "Compiler.h"       /* For FUNC, VAR, CONST macros and memory classes */
#include "Platform_Types.h"

/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

/* Standard Return Values for Std_ReturnType */
#define E_OK            ((Std_ReturnType)0x00U)
#define E_NOT_OK        ((Std_ReturnType)0x01U)

/* Boolean Values */
/* These are standard AUTOSAR values for boolean type. Platform_Types.h defines 'boolean' type. */
#ifndef TRUE
    #define TRUE            1U
#endif
#ifndef FALSE
    #define FALSE           0U
#endif

/* NULL Pointer Definition */
#ifndef NULL_PTR
    #define NULL_PTR ((void *)0)
#endif

/*==================================================================================================
*                                             TYPES
==================================================================================================*/

/**
 * @brief Standard AUTOSAR return type.
 * @details This type shall be used as the return type of AUTOSAR functions that return a standard error status.
 *          It relies on `uint8` which is defined in `Platform_Types.h`.
 */
typedef uint8 Std_ReturnType;

/**
 * @brief Structure for version information.
 * @details This type shall be used to store the version information of a module.
 *          The version information consists of:
 *          - vendorID: The ID of the vendor.
 *          - moduleID: The ID of the module.
 *          - sw_major_version: The major software version number.
 *          - sw_minor_version: The minor software version number.
 *          - sw_patch_version: The patch software version number.
 *          All version numbers are BCD coded.
 */
typedef struct
{
  uint16 vendorID;
  uint16 moduleID;
  uint8  sw_major_version;
  uint8  sw_minor_version;
  uint8  sw_patch_version;
} Std_VersionInfoType;

/*==================================================================================================
*                                         MACROS
==================================================================================================*/

/*
 * Standard ON/OFF Definitions.
 * These are typically used for configuration switches.
 * Platform_Types.h or another common header might also define these.
 * Ensure they are defined once. Std_Types.h is a common place.
 */
#ifndef STD_ON
    #define STD_ON          0x01U
#endif

#ifndef STD_OFF
    #define STD_OFF         0x00U
#endif

#ifndef STD_HIGH
    #define STD_HIGH        0x01U   /* Physical state 5V or 3.3V */
#endif

#ifndef STD_LOW
    #define STD_LOW         0x00U   /* Physical state 0V */
#endif

#endif /* STD_TYPES_H */
