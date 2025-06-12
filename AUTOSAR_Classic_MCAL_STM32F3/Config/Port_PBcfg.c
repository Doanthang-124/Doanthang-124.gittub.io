/*==================================================================================================
*   Project              : AUTOSAR 4.x MCAL
*   Platform             : STM32
*   Peripheral           : PORT
*   Dependencies         : STM32F3xx Series
*
*   Autosar Version      : 4.3.1
*   Autosar Revision     : ASR_REL_4_3_REV_0001
*   Sw Version           : 1.0.0
*   Build Version        : PORT_PBCFG_MCAL_1_0_0_RTM_ASR_REL_4_3_REV_0001_2024xxxx
*
==================================================================================================*/

#include "Port_PBcfg.h" /* For Port_ConfigType, Port_PinConfigType, and fundamental Port types.
                           Includes Std_Types.h (and Compiler.h via Std_Types.h). */
#include "Port_Cfg.h"   /* For symbolic Pin IDs (PORT_PIN_PA0, etc.) and pre-compile switches like PORT_DEV_ERROR_DETECT */

/*==================================================================================================
*                                   VERSION CHECK
==================================================================================================*/
/* Check if Port_PBcfg.c and Port_PBcfg.h have the same SW version */
#if ((PORT_PBCFG_SW_MAJOR_VERSION != 1U) || \
     (PORT_PBCFG_SW_MINOR_VERSION != 0U) || \
     (PORT_PBCFG_SW_PATCH_VERSION != 0U))
  #error "Software Version Numbers of Port_PBcfg.c and Port_PBcfg.h are different"
#endif

/* Check if Port_PBcfg.c and Port_PBcfg.h have the same AUTOSAR version */
#if ((PORT_PBCFG_AR_RELEASE_MAJOR_VERSION != 4U) || \
     (PORT_PBCFG_AR_RELEASE_MINOR_VERSION != 3U) || \
     (PORT_PBCFG_AR_RELEASE_PATCH_VERSION != 1U))
  #error "AUTOSAR Version Numbers of Port_PBcfg.c and Port_PBcfg.h are different"
#endif

/* Check if Port_PBcfg.c and Port_Cfg.h have compatible versions (developer responsibility) */
/* Example: #if (PORT_INSTANCE_ID_CFG != PORT_INSTANCE_ID_PBCFG) ... */


/*==================================================================================================
*                          POST-BUILD CONFIGURATION STRUCTURES
==================================================================================================*/
#define PORT_START_SEC_CONFIG_DATA_UNSPECIFIED
/* #include "MemMap.h" */ /* Or Port_MemMap.h */

/**
 * @brief Array of Pin Configurations for the Port driver.
 * @details Each element configures a single port pin according to the application's requirements.
 *          This array is part of the post-build configuration set.
 *          The memory class `CONFIG_CONST` (from Compiler.h) places this in a read-only section.
 */
STATIC CONST(Port_PinConfigType, CONFIG_CONST) Port_axPinConfigurations[] =
{
    /* Pin Configuration for PA5 (Example: User LED on many STM32 boards) */
    {
        .PinId                  = PORT_PIN_PA5,
        .PinMode                = PORT_PIN_MODE_GPIO_OUT_PUSHPULL,
        .PinDirection           = PORT_PIN_OUT,                 /* Consistent with Output mode */
        .PinPullCfg             = PORT_PULL_NONE,               /* No pull resistor needed for push-pull output */
        .PinOutputSpeedCfg      = PORT_OSPEED_LOW,              /* Low speed is usually sufficient for an LED */
        .PinInitialLevel        = PORT_PIN_LEVEL_LOW,           /* LED initially OFF */
        .PinAlternateFunction   = 0U,                           /* Not an AF mode, value is don't care but initialized */
        .PinDirectionChangeable = TRUE,                          /* Allow runtime direction change if API enabled */
        .PinModeChangeable      = TRUE                           /* Allow runtime mode change if API enabled */
    },

    /* Pin Configuration for PC13 (Example: User Button on Nucleo/Discovery boards) */
    {
        .PinId                  = PORT_PIN_PC13,
        .PinMode                = PORT_PIN_MODE_GPIO_IN_PULLUP,   /* Mode: GPIO Input with Pull-up */
        .PinDirection           = PORT_PIN_IN,                    /* Consistent with Input mode */
        .PinPullCfg             = PORT_PULL_UP,                   /* Enable internal pull-up resistor */
        .PinOutputSpeedCfg      = PORT_OSPEED_LOW,                /* Not applicable for input, initialized to default */
        .PinInitialLevel        = PORT_PIN_LEVEL_HIGH,            /* Not applicable for input, initialized to default */
        .PinAlternateFunction   = 0U,                             /* Not an AF mode */
        .PinDirectionChangeable = FALSE,                          /* Typically fixed for a button input */
        .PinModeChangeable      = FALSE                           /* Typically fixed for a button input */
    },

    /* Pin Configuration for PA9 (Example: USART1_TX on STM32F303) */
    {
        .PinId                  = PORT_PIN_PA9,
        .PinMode                = PORT_PIN_MODE_AF_PUSHPULL,      /* Alternate Function Push-Pull */
        .PinDirection           = PORT_PIN_OUT,                   /* Direction for AF output (TX) */
        .PinPullCfg             = PORT_PULL_UP,                   /* Pull-up often recommended for UART TX when idle */
        .PinOutputSpeedCfg      = PORT_OSPEED_HIGH,               /* High speed for UART communication */
        .PinInitialLevel        = PORT_PIN_LEVEL_HIGH,            /* Idle state for UART TX is high */
        .PinAlternateFunction   = 7U,                             /* AF7 for USART1_TX on PA9 for many STM32F3 */
        .PinDirectionChangeable = FALSE,                          /* Fixed for peripheral function */
        .PinModeChangeable      = FALSE                           /* Fixed for peripheral function */
    },

    /* Pin Configuration for PA10 (Example: USART1_RX on STM32F303) */
    {
        .PinId                  = PORT_PIN_PA10,
        .PinMode                = PORT_PIN_MODE_AF_PUSHPULL,      /* Set to AF mode. Input path configured by peripheral. OTYPER choice less critical for input. */
        .PinDirection           = PORT_PIN_IN,                    /* Direction for AF input (RX) */
        .PinPullCfg             = PORT_PULL_UP,                   /* Or PORT_PULL_NONE/Floating, depending on system needs for UART RX */
        .PinOutputSpeedCfg      = PORT_OSPEED_LOW,                /* Not applicable for a pure AF input */
        .PinInitialLevel        = PORT_PIN_LEVEL_HIGH,            /* Not applicable for input */
        .PinAlternateFunction   = 7U,                             /* AF7 for USART1_RX on PA10 for many STM32F3 */
        .PinDirectionChangeable = FALSE,
        .PinModeChangeable      = FALSE
    },

    /* Pin Configuration for PA0 (Example: Analog Input for ADC) */
    {
        .PinId                  = PORT_PIN_PA0,
        .PinMode                = PORT_PIN_MODE_GPIO_IN_ANALOG,
        .PinDirection           = PORT_PIN_IN,
        .PinPullCfg             = PORT_PULL_NONE,               /* Analog inputs usually require no pull resistor */
        .PinOutputSpeedCfg      = PORT_OSPEED_LOW,              /* N/A for analog input */
        .PinInitialLevel        = PORT_PIN_LEVEL_LOW,           /* N/A for analog input */
        .PinAlternateFunction   = 0U,                           /* N/A for analog input */
        .PinDirectionChangeable = FALSE,
        .PinModeChangeable      = FALSE                           /* Typically fixed for ADC input */
    }
    /* Add configurations for all other pins required by the application.
     * The number of initializers here will determine PortConfig.NumberOfPins.
     */
};


/**
 * @brief Main Post-Build Configuration Structure instance for the Port Driver.
 * @details This structure is declared as `extern CONST(Port_ConfigType, CONFIG_CONST) PortConfig;`
 *          in `Port_PBcfg.h` and is passed to `Port_Init()` to configure all port pins.
 */
CONST(Port_ConfigType, CONFIG_CONST) PortConfig =
{
    .PinConfig      = Port_axPinConfigurations,
    .NumberOfPins   = (Port_PinType)(sizeof(Port_axPinConfigurations) / sizeof(Port_PinConfigType))
};

#define PORT_STOP_SEC_CONFIG_DATA_UNSPECIFIED
/* #include "MemMap.h" */ /* Or Port_MemMap.h */
