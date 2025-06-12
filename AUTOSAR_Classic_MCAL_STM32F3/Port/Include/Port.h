#ifndef PORT_H
#define PORT_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "../../Common/Include/Std_Types.h" /* Standard AUTOSAR Types */
#include "../../Config/Port_Cfg.h"      /* Port Driver Pre-Compile Configuration */

/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

/* AUTOSAR Module and Vendor IDs */
#define PORT_MODULE_ID                    124U
#define PORT_VENDOR_ID                    0U   /* Replace with actual Vendor ID */

/* AUTOSAR Instance ID */
#define PORT_INSTANCE_ID                  0U

/* Development Error Codes (if PORT_DEV_ERROR_DETECT is STD_ON) */
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    #define PORT_E_PARAM_PIN                0x0AU  /* Invalid Port Pin ID requested */
    #define PORT_E_DIRECTION_UNCHANGEABLE   0x0BU  /* Port Pin direction not configured as changeable */
    #define PORT_E_PARAM_CONFIG             0x0CU  /* API service called with wrong parameter */
    #define PORT_E_PARAM_INVALID_MODE       0x0DU  /* Invalid Port Pin Mode requested */
    #define PORT_E_MODE_UNCHANGEABLE        0x0EU  /* Port Pin mode not configured as changeable */
    #define PORT_E_PARAM_POINTER            0x10U  /* API service called with a NULL pointer */
    #define PORT_E_UNINIT                   0x0FU  /* API service used without module initialization */
#endif /* (PORT_DEV_ERROR_DETECT == STD_ON) */

/* API Service IDs (for error reporting, mainly for Det_ReportError) */
#define PORT_INIT_ID                      0x00U
#define PORT_SETPINDIRECTION_ID           0x01U
#define PORT_REFRESHPORTDIRECTION_ID      0x02U
#define PORT_GETVERSIONINFO_ID            0x03U
#define PORT_SETPINMODE_ID                0x04U


/*==================================================================================================
*                                             TYPES
==================================================================================================*/

/* @brief Type for the symbolic name of a port pin. */
/* This should be able to hold all possible pin IDs defined in Port_Cfg.h (e.g., up to 95 for 6 ports) */
typedef uint8 Port_PinType;

/* @brief Possible directions of a port pin. */
typedef enum
{
    PORT_PIN_IN,  /* Sets port pin as input */
    PORT_PIN_OUT  /* Sets port pin as output */
} Port_PinDirectionType;

/* @brief Possible modes of a port pin (MCU specific). */
/* This enum covers common STM32F3 GPIO configurations. */
typedef enum
{
    PORT_PIN_MODE_GPIO_IN_ANALOG   = 0x00, /* Analog input */
    PORT_PIN_MODE_GPIO_IN_FLOATING = 0x01, /* Input Floating (reset state for many pins) */
    PORT_PIN_MODE_GPIO_IN_PULLDOWN = 0x02, /* Input with pull-down resistor */
    PORT_PIN_MODE_GPIO_IN_PULLUP   = 0x03, /* Input with pull-up resistor */

    PORT_PIN_MODE_GPIO_OUT_PUSHPULL   = 0x10, /* Output Push-Pull */
    PORT_PIN_MODE_GPIO_OUT_OPENDRAIN  = 0x11, /* Output Open-Drain */

    PORT_PIN_MODE_AF_PUSHPULL         = 0x20, /* Alternate Function Push-Pull */
    PORT_PIN_MODE_AF_OPENDRAIN        = 0x21, /* Alternate Function Open-Drain */

    /* Default DIO modes for convenience, map to specific GPIO modes */
    PORT_PIN_MODE_DIO                 = PORT_PIN_MODE_GPIO_OUT_PUSHPULL,
    PORT_PIN_MODE_DIO_INPUT           = PORT_PIN_MODE_GPIO_IN_FLOATING,

    PORT_PIN_MODE_NOT_USED            = 0xFF, /* If a pin is not used or unconfigured */
    PORT_PIN_MODE_UNCHANGEABLE        = 0xFE  /* If mode is not changeable runtime */
} Port_PinModeType;


/* @brief Initial level for an output pin. */
typedef enum
{
    PORT_PIN_LEVEL_LOW,
    PORT_PIN_LEVEL_HIGH
} Port_PinInitialValueType;

/* @brief Output speed settings for STM32F3 pins. */
typedef enum
{
    PORT_OSPEED_LOW         = 0x00U, /* Low speed */
    PORT_OSPEED_MEDIUM      = 0x01U, /* Medium speed */
    PORT_OSPEED_HIGH        = 0x02U, /* High speed */
    PORT_OSPEED_VERY_HIGH   = 0x03U  /* Very High speed (STM32F3 specific, maps to 11 in OSPEEDR) */
} Port_PinOutputSpeedType;

/* @brief Pull-up/pull-down resistor configuration for a pin. */
typedef enum
{
    PORT_PULL_NONE,    /* No pull-up or pull-down resistor */
    PORT_PULL_UP,      /* Activate pull-up resistor */
    PORT_PULL_DOWN     /* Activate pull-down resistor */
} Port_PinPullType;


/*
 * @brief Forward declaration for the detailed pin configuration structure.
 * @details The actual structure will be defined in Port_PBcfg.h for post-build configurations
 *          or could be in Port_Cfg.h for pre-compile.
 */
struct Port_PinConfigType;

/*
 * @brief Top-level configuration structure for the Port driver.
 * @details This structure contains pointers to arrays of pin configurations and other global settings.
 *          For post-build, an instance of this type (defined in Port_PBcfg.h using details from there)
 *          is passed to Port_Init().
 */
typedef struct
{
    /** @brief Pointer to the array of individual pin configurations. */
    const struct Port_PinConfigType* PinConfig;

    /** @brief Number of pins configured in the PinConfig array. */
    const Port_PinType NumberOfPins; /* Port_PinType might be uint8, ensure it's large enough for total pins */

    /* Other global Port driver settings can be added here if needed */
    /* For example, a default setting for unconfigured pins, or global clock enables if relevant */
    /* uint32 GlobalPortSettings; */
} Port_ConfigType;


/*==================================================================================================
*                                  FUNCTION PROTOTYPES
==================================================================================================*/

/**
 * @brief Initializes the Port Driver module.
 * @param[in] ConfigPtr Pointer to the configuration set.
 */
extern FUNC(void, PORT_CODE) Port_Init(P2CONST(Port_ConfigType, AUTOMATIC, PORT_APPL_CONST) ConfigPtr);

#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
/**
 * @brief Sets the port pin direction.
 * @param[in] Pin       Port Pin ID number.
 * @param[in] Direction Port Pin direction.
 */
extern FUNC(void, PORT_CODE) Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction);
#endif

/**
 * @brief Refreshes port direction.
 * @details This function is called to set the direction of all unchangeable pins
 *          to the configured direction (e.g., after a reset).
 */
extern FUNC(void, PORT_CODE) Port_RefreshPortDirection(void);

#if (PORT_SET_PIN_MODE_API == STD_ON)
/**
 * @brief Sets the port pin mode.
 * @param[in] Pin   Port Pin ID number.
 * @param[in] Mode  New Port Pin mode to be set.
 */
extern FUNC(void, PORT_CODE) Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode);
#endif

#if (PORT_VERSION_INFO_API == STD_ON)
/**
 * @brief Returns the version information of this module.
 * @param[out] VersionInfo Pointer to where to store the version information.
 */
extern FUNC(void, PORT_CODE) Port_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, PORT_APPL_DATA) VersionInfo);
#endif


#endif /* PORT_H */
