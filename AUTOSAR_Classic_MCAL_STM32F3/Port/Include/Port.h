#ifndef PORT_H
#define PORT_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "../../Common/Include/Std_Types.h" /* Standard AUTOSAR Types (includes Compiler.h, Platform_Types.h) */
#include "../../Config/Port_Cfg.h"      /* Port Driver Pre-Compile Configuration (PORT_DEV_ERROR_DETECT, etc.) */
#include "../../Config/Port_PBcfg.h"    /* For Port_ConfigType and Port_PinConfigType definitions (Post-Build) */

/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

/**
 * @brief Module ID for the Port Driver module.
 * @details As per AUTOSAR_SWS_StandardTypes.
 */
#define PORT_MODULE_ID                    124U /* Standard AUTOSAR Module ID for Port */

/**
 * @brief Vendor ID for this Port driver implementation.
 * @details Replace with actual registered AUTOSAR Vendor ID.
 */
#define PORT_VENDOR_ID                    0U   /* Example: To be replaced by actual Vendor ID */

/** @brief Software major version of this Port driver. */
#define PORT_SW_MAJOR_VERSION             1U
/** @brief Software minor version of this Port driver. */
#define PORT_SW_MINOR_VERSION             0U
/** @brief Software patch version of this Port driver. */
#define PORT_SW_PATCH_VERSION             0U

/** @brief AUTOSAR specification major version compatibility. */
#define PORT_AR_RELEASE_MAJOR_VERSION     4U
/** @brief AUTOSAR specification minor version compatibility. */
#define PORT_AR_RELEASE_MINOR_VERSION     3U
/** @brief AUTOSAR specification patch version compatibility. */
#define PORT_AR_RELEASE_PATCH_VERSION     1U


/* API Service IDs for Development Error Reporting (DET) */
#define PORT_INIT_API_ID                      0x00U
#define PORT_SET_PIN_DIRECTION_API_ID         0x01U
#define PORT_REFRESH_PORT_DIRECTION_API_ID    0x02U
#define PORT_GET_VERSION_INFO_API_ID          0x03U
#define PORT_SET_PIN_MODE_API_ID              0x04U


/* Development Error Codes reported to DET (if PORT_DEV_ERROR_DETECT is STD_ON from Port_Cfg.h) */
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    #define PORT_E_PARAM_PIN                  0x0AU  /**< Invalid Port Pin ID requested. (AUTOSAR SWS Port027) */
    #define PORT_E_DIRECTION_UNCHANGEABLE     0x0BU  /**< Port Pin direction cannot be changed. (AUTOSAR SWS Port028) */
    #define PORT_E_PARAM_CONFIG               0x0CU  /**< API Port_Init service called with wrong parameter. (AUTOSAR SWS Port029) */
    #define PORT_E_PARAM_INVALID_MODE         0x0DU  /**< API Port_SetPinMode service called with invalid mode. (AUTOSAR SWS Port030) */
    #define PORT_E_MODE_UNCHANGEABLE          0x0EU  /**< Port Pin mode cannot be changed. (AUTOSAR SWS Port031) */
    #define PORT_E_UNINIT                     0x0FU  /**< API service called without module initialization. (AUTOSAR SWS Port033) */
    #define PORT_E_PARAM_POINTER              0x10U  /**< API service called with a NULL pointer. (AUTOSAR SWS Port032) */
#endif /* (PORT_DEV_ERROR_DETECT == STD_ON) */


/*==================================================================================================
*                                             TYPES
==================================================================================================*/

/**
 * @brief Data type for the symbolic name of a port pin. (AUTOSAR SWS Port060)
 * @details This type shall be uint8, capable of holding all pin IDs defined in Port_Cfg.h.
 */
typedef uint8 Port_PinType;

/**
 * @brief Enumeration for the possible directions of a port pin. (AUTOSAR SWS Port061)
 */
typedef enum
{
    PORT_PIN_IN = 0U,  /**< Sets port pin direction as input. */
    PORT_PIN_OUT       /**< Sets port pin direction as output. */
} Port_PinDirectionType;


/**
 * @brief Enumeration for the possible modes of a port pin (MCU specific). (AUTOSAR SWS Port062)
 * @details This covers common STM32F3 GPIO configurations. Values are chosen to be distinct.
 */
typedef enum
{
    PORT_PIN_MODE_GPIO_IN_ANALOG   = 0x00, /**< Analog input mode. */
    PORT_PIN_MODE_GPIO_IN_FLOATING = 0x01, /**< Input Floating (reset state for many pins). */
    PORT_PIN_MODE_GPIO_IN_PULLDOWN = 0x02, /**< Input with internal pull-down resistor enabled. */
    PORT_PIN_MODE_GPIO_IN_PULLUP   = 0x03, /**< Input with internal pull-up resistor enabled. */

    PORT_PIN_MODE_GPIO_OUT_PUSHPULL   = 0x10, /**< General purpose output push-pull. */
    PORT_PIN_MODE_GPIO_OUT_OPENDRAIN  = 0x11, /**< General purpose output open-drain. */

    PORT_PIN_MODE_AF_PUSHPULL         = 0x20, /**< Alternate Function push-pull. */
    PORT_PIN_MODE_AF_OPENDRAIN        = 0x21, /**< Alternate Function open-drain. */

    PORT_PIN_MODE_NOT_USED            = 0xFF  /**< Pin is not configured or used by Port driver. */
} Port_PinModeType;


/** @brief Enumeration for initial level for an output pin. */
typedef enum
{
    PORT_PIN_LEVEL_LOW = 0U, /**< Initial logical level LOW for an output pin. */
    PORT_PIN_LEVEL_HIGH      /**< Initial logical level HIGH for an output pin. */
} Port_PinInitialValueType;


/** @brief Enumeration for output speed settings for STM32F3 pins (MCU specific). */
typedef enum
{
    PORT_OSPEED_LOW         = 0x00U, /**< Low speed. */
    PORT_OSPEED_MEDIUM      = 0x01U, /**< Medium speed. */
    PORT_OSPEED_HIGH        = 0x02U, /**< High speed. */
    PORT_OSPEED_VERY_HIGH   = 0x03U  /**< Very High speed (STM32F3 specific, maps to 11B in OSPEEDR). */
} Port_PinOutputSpeedType;


/** @brief Enumeration for pull-up/pull-down resistor configuration for a pin (MCU specific). */
typedef enum
{
    PORT_PULL_NONE = 0U, /**< No internal pull-up or pull-down resistor. */
    PORT_PULL_UP,        /**< Internal pull-up resistor activated. */
    PORT_PULL_DOWN       /**< Internal pull-down resistor activated. */
} Port_PinPullType;


/*
 * The types `Port_PinConfigType` (for individual pin configuration) and
 * `Port_ConfigType` (for the overall Port driver configuration) are now
 * fully defined in `Port_PBcfg.h` (included above).
 * This makes `Port_PBcfg.h` the source of truth for these post-build structures,
 * and `Port.h` consumes them for its API function prototypes.
 */


/*==================================================================================================
*                                  FUNCTION PROTOTYPES
==================================================================================================*/

/**
 * @brief Initializes the Port Driver module. (AUTOSAR SWS Port050)
 * @details This function sets the initial configuration (direction, mode, pull, speed, etc.)
 *          for all port pins as defined in the post-build configuration.
 *          It must be called before any other Port driver API service.
 * @param[in] ConfigPtr Pointer to the post-build configuration set (type `Port_ConfigType` from `Port_PBcfg.h`).
 */
extern FUNC(void, PORT_CODE) Port_Init(
    P2CONST(Port_ConfigType, AUTOMATIC, PORT_APPL_CONST) ConfigPtr
);

#if (PORT_SET_PIN_DIRECTION_API == STD_ON) /* From Port_Cfg.h */
/**
 * @brief Sets the direction of a configurable port pin during runtime. (AUTOSAR SWS Port052)
 * @details This function is only available if `PORT_SET_PIN_DIRECTION_API` is `STD_ON`.
 *          The pin must also be configured as direction-changeable in the post-build configuration.
 * @param[in] Pin       Port Pin ID number.
 * @param[in] Direction New Port Pin direction (PORT_PIN_IN or PORT_PIN_OUT).
 */
extern FUNC(void, PORT_CODE) Port_SetPinDirection(
    VAR(Port_PinType, AUTOMATIC) Pin,
    VAR(Port_PinDirectionType, AUTOMATIC) Direction
);
#endif /* (PORT_SET_PIN_DIRECTION_API == STD_ON) */

/**
 * @brief Refreshes the direction of all port pins that are configured as non-changeable. (AUTOSAR SWS Port053)
 * @details This function ensures that the direction of pins whose direction cannot be changed
 *          at runtime is reset to their configured state. This might be useful after
 *          certain low-power modes or error conditions.
 */
extern FUNC(void, PORT_CODE) Port_RefreshPortDirection(void);

#if (PORT_SET_PIN_MODE_API == STD_ON) /* From Port_Cfg.h */
/**
 * @brief Sets the mode of a configurable port pin during runtime. (AUTOSAR SWS Port054)
 * @details This function is only available if `PORT_SET_PIN_MODE_API` is `STD_ON`.
 *          The pin must also be configured as mode-changeable in the post-build configuration.
 * @param[in] Pin   Port Pin ID number.
 * @param[in] Mode  New Port Pin mode to be set (from `Port_PinModeType`).
 */
extern FUNC(void, PORT_CODE) Port_SetPinMode(
    VAR(Port_PinType, AUTOMATIC) Pin,
    VAR(Port_PinModeType, AUTOMATIC) Mode
);
#endif /* (PORT_SET_PIN_MODE_API == STD_ON) */

#if (PORT_VERSION_INFO_API == STD_ON) /* From Port_Cfg.h */
/**
 * @brief Returns the version information of the Port Driver module. (AUTOSAR SWS Port051)
 * @details This function is only available if `PORT_VERSION_INFO_API` is `STD_ON`.
 * @param[out] VersionInfoPtr Pointer to a `Std_VersionInfoType` structure where the version
 *                            information will be stored. This pointer must not be NULL.
 */
extern FUNC(void, PORT_CODE) Port_GetVersionInfo(
    P2VAR(Std_VersionInfoType, AUTOMATIC, PORT_APPL_DATA) VersionInfoPtr
);
#endif /* (PORT_VERSION_INFO_API == STD_ON) */


#endif /* PORT_H */
