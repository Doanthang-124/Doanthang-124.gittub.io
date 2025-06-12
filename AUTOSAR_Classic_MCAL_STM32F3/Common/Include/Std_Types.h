#ifndef STD_TYPES_H
#define STD_TYPES_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "Platform_Types.h" /* For basic platform types (uint8, boolean, etc.) */
#include "Compiler.h"       /* For compiler abstraction macros (FUNC, VAR, CONST, STATIC, etc.) */

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/**
 * @brief Symbol to define API services as ON (enabled).
 * @details Adheres to MISRA C:2012 Rule 10.3 by using unsigned suffix U.
 */
#ifndef STD_ON
    #define STD_ON          1U
#endif

/**
 * @brief Symbol to define API services as OFF (disabled).
 * @details Adheres to MISRA C:2012 Rule 10.3 by using unsigned suffix U.
 */
#ifndef STD_OFF
    #define STD_OFF         0U
#endif

/** @brief Represents a `TRUE` boolean value. */
#ifndef TRUE
    #define TRUE            1U  /**< @brief Standard TRUE value (boolean) */
#endif

/** @brief Represents a `FALSE` boolean value. */
#ifndef FALSE
    #define FALSE           0U  /**< @brief Standard FALSE value (boolean) */
#endif


/**
 * @brief Standard return type for many AUTOSAR APIs, representing success.
 * @details Value is 0x00. Uses Std_ReturnType.
 *          Adheres to MISRA C:2012 Rule 10.1, 10.3.
 */
#define E_OK            ((Std_ReturnType)0x00U)

/**
 * @brief Standard return type for many AUTOSAR APIs, representing failure.
 * @details Value is 0x01. Uses Std_ReturnType.
 *          Adheres to MISRA C:2012 Rule 10.1, 10.3.
 */
#define E_NOT_OK        ((Std_ReturnType)0x01U)


/**
 * @brief Definition of a NULL pointer.
 * @details Cast to (void *) for type safety and to satisfy MISRA C:2012 rules
 *          (e.g., Rule 11.5 - not converting pointer to void to pointer to object,
 *           but (void*)0 is a common way to define NULL_PTR).
 *          MISRA C:2012 Rule 11.9: "The macro NULL shall be defined as a null pointer constant."
 *          A null pointer constant is an integer literal with value 0, or such an expression cast to void*.
 */
#ifndef NULL_PTR
    #define NULL_PTR ((void *)0)
#endif

/*==================================================================================================
*                                             TYPES
==================================================================================================*/

/**
 * @brief Standard AUTOSAR return type.
 * @details This type is used for functions that return a success or failure status.
 *          It is based on `uint8` from `Platform_Types.h`.
 */
typedef uint8 Std_ReturnType;


/**
 * @brief Structure for holding version information of a BSW module.
 * @details This structure is used by `Xxx_GetVersionInfo()` APIs.
 *          Members use fixed-width types from `Platform_Types.h`.
 */
typedef struct
{
    uint16  vendorID;           /**< @brief Vendor ID for the module. */
    uint16  moduleID;           /**< @brief Module ID. */
    uint8   sw_major_version;   /**< @brief Software major version. */
    uint8   sw_minor_version;   /**< @brief Software minor version. */
    uint8   sw_patch_version;   /**< @brief Software patch version. */
} Std_VersionInfoType;

#endif /* STD_TYPES_H */
