#ifndef PORT_PBCFG_H
#define PORT_PBCFG_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "../Common/Include/Std_Types.h" /* For standard types like boolean, uint8, etc. */
#include "../Include/Port.h"             /* For Port specific types like Port_PinType, Port_PinModeType, and Port_ConfigType declaration */

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Define version numbers for this PBcfg header if not already in Port.h or a common version header */
#define PORT_PBCFG_SW_MAJOR_VERSION     1U
#define PORT_PBCFG_SW_MINOR_VERSION     0U
#define PORT_PBCFG_SW_PATCH_VERSION     0U

#define PORT_PBCFG_AR_RELEASE_MAJOR_VERSION    4U
#define PORT_PBCFG_AR_RELEASE_MINOR_VERSION    3U /* Or 9U for newer AUTOSAR versions */
#define PORT_PBCFG_AR_RELEASE_REVISION_VERSION 1U

/* Example checks against Port.h versions (assuming Port.h defines these) */
/*
#if ((PORT_AR_RELEASE_MAJOR_VERSION_PBCFG != PORT_AR_RELEASE_MAJOR_VERSION) || \
     (PORT_AR_RELEASE_MINOR_VERSION_PBCFG != PORT_AR_RELEASE_MINOR_VERSION) || \
     (PORT_AR_RELEASE_REVISION_VERSION_PBCFG != PORT_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Port_PBcfg.h and Port.h are different"
#endif

#if ((PORT_SW_MAJOR_VERSION_PBCFG != PORT_SW_MAJOR_VERSION) || \
     (PORT_SW_MINOR_VERSION_PBCFG != PORT_SW_MINOR_VERSION) || \
     (PORT_SW_PATCH_VERSION_PBCFG != PORT_SW_PATCH_VERSION))
    #error "Software Version Numbers of Port_PBcfg.h and Port.h are different"
#endif
*/

/*==================================================================================================
*                                             TYPES
==================================================================================================*/

/**
 * @brief Structure defining the post-build configuration for a single Port Pin.
 * @details This structure contains all parameters necessary to configure one port pin,
 *          such as its direction, mode, pull resistor, initial level, etc.
 *          An array of this type will be defined in Port_PBcfg.c.
 * @note The `struct` keyword is used here to complete the forward declaration
 *       that might be present in Port.h for Port_ConfigType.
 */
typedef struct Port_PinConfigType
{
    /** @brief The symbolic ID of the pin (e.g., PORT_PIN_PA0 from Port_Cfg.h). */
    Port_PinType PinId;

    /** @brief Initial direction of the pin (Input/Output). */
    Port_PinDirectionType PinDirection;

    /** @brief Specifies if the pin's direction can be changed at runtime via Port_SetPinDirection(). */
    boolean PinDirectionChangeable;

    /** @brief The mode of the pin (e.g., GPIO, Alternate Function, Analog). */
    Port_PinModeType PinMode;

    /** @brief Specifies if the pin's mode can be changed at runtime via Port_SetPinMode(). */
    boolean PinModeChangeable;

    /** @brief Configuration for internal pull-up/pull-down resistors. */
    Port_PinPullType PinPullCfg;

    /** @brief Configuration for the output speed of the pin (STM32 specific). */
    Port_PinOutputSpeedType PinOutputSpeedCfg;

    /** @brief Initial level for the pin if configured as an output (Low/High). */
    Port_PinInitialValueType PinInitialLevel; /* Changed from Dio_LevelType to Port_PinInitialValueType for consistency */

    /** @brief Alternate function selection (0-15 for STM32) if PinMode is an AF mode. */
    uint8 PinAlternateFunction;

} Port_PinConfigType;


/*
 * The Port_ConfigType structure is declared in Port.h.
 * It typically looks like:
 * typedef struct {
 *     const Port_PinConfigType* PinConfig; // Array of pin configurations
 *     Port_PinType NumberOfPins;           // Total number of configured pins
 * } Port_ConfigType;
 * This file (Port_PBcfg.h) provides the full definition of Port_PinConfigType,
 * which is used by Port_ConfigType.
 */


/*==================================================================================================
*                                  EXTERNAL CONFIGURATION DECLARATIONS
==================================================================================================*/

/**
 * @brief Declaration of the post-build configuration set for the Port Driver.
 * @details The actual definition of this structure (i.e., the instance) will be in Port_PBcfg.c.
 *          This is the configuration data that will be passed to Port_Init().
 *          The name 'PortConfig' is a common convention.
 */
#define PORT_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Port_MemMap.h" /* Placeholder for MemMap header, if used */

extern const Port_ConfigType Port_Config; /* Or e.g., Port_ConfigSet_Default */

#define PORT_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Port_MemMap.h" /* Placeholder for MemMap header, if used */


#endif /* PORT_PBCFG_H */
