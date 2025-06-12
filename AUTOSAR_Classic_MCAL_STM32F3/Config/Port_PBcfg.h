#ifndef PORT_PBCFG_H
#define PORT_PBCFG_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "../Common/Include/Std_Types.h" /* For Std_ReturnType, uint8, uint16, boolean, P2CONST, CONST etc. */
                                         /* Std_Types.h includes Compiler.h */
/*
 * Note: Port.h includes this file (Port_PBcfg.h). Therefore, Port_PBcfg.h must NOT include Port.h
 * to prevent circular dependencies. All types needed by Port_PBcfg.h that were previously in Port.h
 * (like Port_PinType and various enums) must be defined herein or included from other base headers.
 */

/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/
/* Software Version Numbers for this Post-Build configuration header */
#define PORT_PBCFG_SW_MAJOR_VERSION       1U
#define PORT_PBCFG_SW_MINOR_VERSION       0U
#define PORT_PBCFG_SW_PATCH_VERSION       0U

/* AUTOSAR Version Information for this Post-Build configuration header */
#define PORT_PBCFG_AR_RELEASE_MAJOR_VERSION      4U
#define PORT_PBCFG_AR_RELEASE_MINOR_VERSION      3U
#define PORT_PBCFG_AR_RELEASE_REVISION_VERSION   1U

/*==================================================================================================
*                                             TYPES
==================================================================================================*/

/*
 * Fundamental Port Driver Types
 * These types are defined here because Port.h includes this file (Port_PBcfg.h)
 * to get the full definitions of Port_PinConfigType and Port_ConfigType for its API.
 * This makes Port_PBcfg.h the source for these fundamental Port types.
 */

/** @brief Data type for the symbolic name of a port pin. (AUTOSAR SWS Port060) */
typedef uint8 Port_PinType;

/** @brief Enumeration for the possible directions of a port pin. (AUTOSAR SWS Port061) */
typedef enum
{
    PORT_PIN_IN = 0U,  /**< Sets port pin direction as input. */
    PORT_PIN_OUT       /**< Sets port pin direction as output. */
} Port_PinDirectionType;

/** @brief Enumeration for the possible modes of a port pin (MCU specific). (AUTOSAR SWS Port062) */
typedef enum
{
    PORT_PIN_MODE_GPIO_IN_ANALOG   = 0x00, /**< Analog input mode. */
    PORT_PIN_MODE_GPIO_IN_FLOATING = 0x01, /**< Input Floating. */
    PORT_PIN_MODE_GPIO_IN_PULLDOWN = 0x02, /**< Input with internal pull-down resistor. */
    PORT_PIN_MODE_GPIO_IN_PULLUP   = 0x03, /**< Input with internal pull-up resistor. */
    PORT_PIN_MODE_GPIO_OUT_PUSHPULL   = 0x10, /**< General purpose output push-pull. */
    PORT_PIN_MODE_GPIO_OUT_OPENDRAIN  = 0x11, /**< General purpose output open-drain. */
    PORT_PIN_MODE_AF_PUSHPULL         = 0x20, /**< Alternate Function push-pull. */
    PORT_PIN_MODE_AF_OPENDRAIN        = 0x21, /**< Alternate Function open-drain. */
    PORT_PIN_MODE_NOT_USED            = 0xFF  /**< Pin is not configured or used. */
} Port_PinModeType;

/** @brief Enumeration for initial level for an output pin. */
typedef enum
{
    PORT_PIN_LEVEL_LOW = 0U, /**< Initial logical level LOW. */
    PORT_PIN_LEVEL_HIGH      /**< Initial logical level HIGH. */
} Port_PinInitialValueType;

/** @brief Enumeration for output speed settings for STM32F3 pins. */
typedef enum
{
    PORT_OSPEED_LOW         = 0x00U, /**< Low speed. */
    PORT_OSPEED_MEDIUM      = 0x01U, /**< Medium speed. */
    PORT_OSPEED_HIGH        = 0x02U, /**< High speed. */
    PORT_OSPEED_VERY_HIGH   = 0x03U  /**< Very High speed. */
} Port_PinOutputSpeedType;

/** @brief Enumeration for pull-up/pull-down resistor configuration for a pin. */
typedef enum
{
    PORT_PULL_NONE = 0U, /**< No internal pull-up or pull-down resistor. */
    PORT_PULL_UP,        /**< Internal pull-up resistor activated. */
    PORT_PULL_DOWN       /**< Internal pull-down resistor activated. */
} Port_PinPullType;


/**
 * @brief Structure defining the post-build configuration for a single Port Pin.
 * @details This structure is instantiated in an array in `Port_PBcfg.c` for each configured pin,
 *          providing all necessary settings for `Port_Init` to configure the pin.
 *          The tag name `Port_PinConfigType_Tag` can be used if `Port_PinConfigType` itself is preferred for the typedef.
 *          AUTOSAR often uses `typedef struct { ... } Module_Type;` directly.
 */
typedef struct Port_PinConfigType_Tag /* Using a tag to avoid potential typedef/struct name collision issues in some MISRA checkers */
{
    Port_PinType                PinId;                  /**< @brief Symbolic ID of the pin (e.g., PORT_PIN_PA0 from Port_Cfg.h). */
    Port_PinModeType            PinMode;                /**< @brief Initial mode of the pin (GPIO, AF, Analog). */
    Port_PinDirectionType       PinDirection;           /**< @brief Initial direction (IN/OUT). Relevant for GPIO modes. */
    Port_PinPullType            PinPullCfg;             /**< @brief Pull-up, Pull-down, or No Pull configuration. */
    Port_PinOutputSpeedType     PinOutputSpeedCfg;      /**< @brief Output speed. Relevant for Output and AF modes. */
    Port_PinInitialValueType    PinInitialLevel;        /**< @brief Initial logical level for output pins (LOW/HIGH). */
    uint8                       PinAlternateFunction;   /**< @brief Alternate function selection (0-15 for STM32) if mode is AF. Ignored otherwise. */
    boolean                     PinDirectionChangeable; /**< @brief TRUE if pin direction can be changed at runtime via Port_SetPinDirection. */
    boolean                     PinModeChangeable;      /**< @brief TRUE if pin mode can be changed at runtime via Port_SetPinMode. */
} Port_PinConfigType;


/**
 * @brief Main Post-Build Configuration Structure for the Port Driver.
 * @details This structure contains a pointer to an array of `Port_PinConfigType` elements,
 *          defining the configuration for all port pins managed by this driver instance.
 *          An instance of this structure (e.g., `PortConfig`) is defined in `Port_PBcfg.c`
 *          and passed to `Port_Init()`.
 */
typedef struct Port_ConfigType_Tag /* Using a tag for consistency */
{
    /** @brief Pointer to the array of individual pin configurations. */
    P2CONST(Port_PinConfigType, AUTOMATIC, CONFIG_CONST) PinConfig; /* CONFIG_CONST from Compiler.h */

    /** @brief Total number of pins configured in the `PinConfig` array.
     *  @details This should match the actual number of elements in the array defined in `Port_PBcfg.c`.
     */
    Port_PinType NumberOfPins;
} Port_ConfigType;


/*==================================================================================================
*                                      EXTERNAL CONSTANTS
==================================================================================================*/

/**
 * @brief Declaration of the post-build configuration set for the Port Driver.
 * @details The actual definition (the instance `PortConfig`) is in `Port_PBcfg.c`.
 *          This uses the `CONFIG_CONST` memory class from `Compiler.h`.
 */
extern CONST(Port_ConfigType, CONFIG_CONST) PortConfig;


#endif /* PORT_PBCFG_H */
