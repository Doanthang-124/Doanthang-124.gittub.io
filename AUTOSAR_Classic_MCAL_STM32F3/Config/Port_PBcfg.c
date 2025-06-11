/*==================================================================================================
*   Project              : AUTOSAR 4.x MCAL
*   Platform             : STM32
*   Peripheral           : GPIO
*   Dependencies         : STM32F3xx Series
*
*   Autosar Version      : 4.3.1
*   Autosar Revision     : ASR_REL_4_3_REV_0001
*   Sw Version           : 1.0.0
*   Build Version        : PORT_MCAL_1_0_0_RTM_ASR_REL_4_3_REV_0001_2024xxxx
*
*   (c) Copyright 2023-2024 YourName
*   All Rights Reserved.
==================================================================================================*/

#include "Port_PBcfg.h" /* Main header for Port Post-Build configurations.
                           Includes Port.h, which includes Port_Cfg.h for symbolic pin names
                           and Std_Types.h. */
#include "../Include/Dio.h" /* For Dio_LevelType (STD_HIGH, STD_LOW) used in PinInitialLevel.
                               Port_PinInitialValueType is defined in Port.h but might map to these.
                               Let's assume Port_PinInitialValueType is compatible with STD_HIGH/LOW.
                               Port.h defines Port_PinInitialValueType as enum {PORT_PIN_LEVEL_LOW, PORT_PIN_LEVEL_HIGH}
                               So STD_LOW/STD_HIGH from Dio.h are not strictly needed if we use PORT_PIN_LEVEL_ constants.
                               For clarity, if PinInitialLevel is meant to be 0 or 1, using PORT_PIN_LEVEL_LOW/HIGH is better.
                           */

/*==================================================================================================
*                                       VERSION CHECKS
==================================================================================================*/
/* Define this file's version numbers to match Port_PBcfg.h (example) */
#define PORT_PBCFG_SW_MAJOR_VERSION_C            1U
#define PORT_PBCFG_SW_MINOR_VERSION_C            0U
#define PORT_PBCFG_SW_PATCH_VERSION_C            0U

#define PORT_PBCFG_AR_RELEASE_MAJOR_VERSION_C    4U
#define PORT_PBCFG_AR_RELEASE_MINOR_VERSION_C    3U
#define PORT_PBCFG_AR_RELEASE_REVISION_VERSION_C 1U

/* Check against Port_PBcfg.h version numbers */
#if ((PORT_PBCFG_SW_MAJOR_VERSION_C != PORT_PBCFG_SW_MAJOR_VERSION) ||  \
     (PORT_PBCFG_SW_MINOR_VERSION_C != PORT_PBCFG_SW_MINOR_VERSION) ||  \
     (PORT_PBCFG_SW_PATCH_VERSION_C != PORT_PBCFG_SW_PATCH_VERSION))
  #error "Software Version Numbers of Port_PBcfg.c and Port_PBcfg.h are different"
#endif

#if ((PORT_PBCFG_AR_RELEASE_MAJOR_VERSION_C != PORT_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (PORT_PBCFG_AR_RELEASE_MINOR_VERSION_C != PORT_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (PORT_PBCFG_AR_RELEASE_REVISION_VERSION_C != PORT_PBCFG_AR_RELEASE_REVISION_VERSION))
  #error "AUTOSAR Version Numbers of Port_PBcfg.c and Port_PBcfg.h are different"
#endif


/*==================================================================================================
*                                 POST BUILD CONFIGURATION ARRAY
==================================================================================================*/

/**
 * @brief Array of Pin Configurations.
 * @details This array contains the detailed configuration for each port pin
 *          managed by the Port driver. The settings here are applied during Port_Init().
 *          Symbolic pin names (e.g., PORT_PIN_PA5) are defined in Port_Cfg.h.
 *          The type Port_PinConfigType is defined in Port_PBcfg.h.
 */
STATIC const Port_PinConfigType Port_PinConfigurations[] =
{
    /* Example: Configure PA5 as a GPIO Output for an LED */
    {
        .PinId                  = PORT_PIN_PA5,
        .PinDirection           = PORT_PIN_OUT,
        .PinDirectionChangeable = TRUE,
        .PinMode                = PORT_PIN_MODE_GPIO_OUT_PUSHPULL,
        .PinModeChangeable      = TRUE,
        .PinPullCfg             = PORT_PULL_NONE,
        .PinOutputSpeedCfg      = PORT_OSPEED_LOW,
        .PinInitialLevel        = PORT_PIN_LEVEL_LOW, /* Use Port_PinInitialValueType */
        .PinAlternateFunction   = 0U /* Not used for GPIO mode */
    },

    /* Example: Configure PC13 as a GPIO Input for a User Button (common on Nucleo boards) */
    {
        .PinId                  = PORT_PIN_PC13,
        .PinDirection           = PORT_PIN_IN,
        .PinDirectionChangeable = TRUE, /* Typically fixed as input */
        .PinMode                = PORT_PIN_MODE_GPIO_IN_PULLUP, /* Input with Pull-up */
        .PinModeChangeable      = TRUE, /* Or FALSE if always an input button */
        .PinPullCfg             = PORT_PULL_UP, /* Pull-up is common for buttons */
        .PinOutputSpeedCfg      = PORT_OSPEED_LOW, /* Not applicable for input */
        .PinInitialLevel        = PORT_PIN_LEVEL_HIGH, /* Not applicable for input */
        .PinAlternateFunction   = 0U /* Not used for GPIO mode */
    },

    /* Example: Configure PA9 (USART1_TX) for UART communication */
    {
        .PinId                  = PORT_PIN_PA9,    /* STM32F303: USART1_TX is often AF7 on PA9 */
        .PinDirection           = PORT_PIN_OUT,    /* Direction is implicitly OUT for TX */
        .PinDirectionChangeable = FALSE,           /* Fixed direction for TX */
        .PinMode                = PORT_PIN_MODE_AF_PUSHPULL,
        .PinModeChangeable      = FALSE,           /* Fixed mode for TX */
        .PinPullCfg             = PORT_PULL_UP,    /* Pull-up on TX line is common */
        .PinOutputSpeedCfg      = PORT_OSPEED_HIGH, /* High speed for UART */
        .PinInitialLevel        = PORT_PIN_LEVEL_HIGH, /* Default idle state for UART TX */
        .PinAlternateFunction   = 7U               /* AF7 for USART1 on PA9 (verify for specific STM32F3xx) */
    },

    /* Example: Configure PA10 (USART1_RX) for UART communication */
    {
        .PinId                  = PORT_PIN_PA10,   /* STM32F303: USART1_RX is often AF7 on PA10 */
        .PinDirection           = PORT_PIN_IN,     /* Direction is implicitly IN for RX */
        .PinDirectionChangeable = FALSE,           /* Fixed direction for RX */
        .PinMode                = PORT_PIN_MODE_AF_PUSHPULL, /* Input mode for AF. OTYPER is PushPull to match common settings. */
                                                   /* Could also be IN_FLOATING if peripheral guarantees input path. */
        .PinModeChangeable      = FALSE,           /* Fixed mode for RX */
        .PinPullCfg             = PORT_PULL_UP,    /* Pull-up or floating, depending on system */
        .PinOutputSpeedCfg      = PORT_OSPEED_LOW, /* Not applicable for AF input */
        .PinInitialLevel        = PORT_PIN_LEVEL_HIGH, /* Not applicable for input */
        .PinAlternateFunction   = 7U               /* AF7 for USART1 on PA10 (verify for specific STM32F3xx) */
    },

    /* Example: Configure PA0 as Analog input (e.g. for ADC) */
    {
        .PinId                  = PORT_PIN_PA0,
        .PinDirection           = PORT_PIN_IN,
        .PinDirectionChangeable = FALSE,
        .PinMode                = PORT_PIN_MODE_GPIO_IN_ANALOG,
        .PinModeChangeable      = FALSE,
        .PinPullCfg             = PORT_PULL_NONE,
        .PinOutputSpeedCfg      = PORT_OSPEED_LOW,      /* N/A */
        .PinInitialLevel        = PORT_PIN_LEVEL_LOW,   /* N/A */
        .PinAlternateFunction   = 0U                    /* N/A */
    }

    /* Add configurations for all other pins used by the application here. */
    /* For pins not used, they could be configured to a safe default state, */
    /* e.g., Analog Input with no pull resistors, to minimize power consumption. */
};


/*==================================================================================================
*                                 POST BUILD CONFIGURATION STRUCTURE
==================================================================================================*/

/**
 * @brief The main Post-Build Configuration Structure for the Port Driver.
 * @details This structure is declared as extern in Port_PBcfg.h and is passed to Port_Init().
 *          It contains a pointer to the array of pin configurations and the total number
 *          of configured pins.
 */
#define PORT_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Port_MemMap.h" /* Placeholder for MemMap include */

const Port_ConfigType Port_Config =
{
    .PinConfig        = Port_PinConfigurations,
    .NumberOfPins     = (Port_PinType)(sizeof(Port_PinConfigurations) / sizeof(Port_PinConfigType))
};

#define PORT_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Port_MemMap.h" /* Placeholder for MemMap include */
