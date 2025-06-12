#ifndef PORT_CFG_H
#define PORT_CFG_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "Std_Types.h"      /* For STD_ON, STD_OFF definitions */
/* For Port_PinType - this is uint8. Port.h (where it's typedef'd) includes Port_Cfg.h,
   so Port_Cfg.h cannot include Port.h directly. Using (uint8) cast for pin ID macros is safe.
*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/**
 * @brief Instance ID for the PORT module.
 * @details Typically 0 for non-reentrant modules.
 */
#define PORT_INSTANCE_ID                      (0U)


/**
 * @brief Switch to enable or disable development error detection for the PORT module.
 */
#define PORT_DEV_ERROR_DETECT                 STD_ON

/**
 * @brief Switch to enable or disable the Port_GetVersionInfo API.
 */
#define PORT_VERSION_INFO_API                 STD_ON

/**
 * @brief Switch to enable or disable the Port_SetPinDirection API.
 * @details If STD_ON, allows changing pin direction at runtime if the pin is configured as changeable.
 */
#define PORT_SET_PIN_DIRECTION_API            STD_ON

/**
 * @brief Switch to enable or disable the Port_SetPinMode API.
 * @details If STD_ON, allows changing pin mode at runtime if the pin is configured as changeable.
 */
#define PORT_SET_PIN_MODE_API                 STD_ON


/*
 * The following macros define whether pin direction and mode can be changed AT ALL during runtime
 * at a global level for the Port driver instance.
 * If PORT_SET_PIN_DIRECTION_API is STD_ON, then PORT_PIN_DIRECTION_CHANGEABLE should also generally be STD_ON
 * to allow individual pins to be configured as changeable in Port_PBcfg.c.
 * If PORT_PIN_DIRECTION_CHANGEABLE is STD_OFF here, then no pin's direction can be changed by Port_SetPinDirection,
 * regardless of the Port_SetPinDirection API switch or individual pin configuration in Port_PBcfg.c.
 */

/**
 * @brief Global switch to allow/disallow runtime changes to any pin's direction via Port_SetPinDirection.
 * @details If STD_OFF, Port_SetPinDirection (if enabled by PORT_SET_PIN_DIRECTION_API) will report an error
 *          or have no effect, even on pins configured as changeable in post-build configuration.
 */
#define PORT_PIN_DIRECTION_CHANGEABLE         STD_ON

/**
 * @brief Global switch to allow/disallow runtime changes to any pin's mode via Port_SetPinMode.
 * @details If STD_OFF, Port_SetPinMode (if enabled by PORT_SET_PIN_MODE_API) will report an error
 *          or have no effect, even on pins configured as changeable in post-build configuration.
 */
#define PORT_PIN_MODE_CHANGEABLE              STD_ON


/**
 * @brief Defines the highest symbolic Port Pin ID configured in this file.
 * @details Used to derive the total number of potentially addressable pins.
 */
#define PORT_MAX_PIN_ID                       (95U) /* Corresponds to PF15 in the 0-95 numbering scheme */

/**
 * @brief Defines the total number of unique port pin symbolic names provided.
 * @details This value indicates the range of pin IDs (0 to PORT_MAX_PIN_ID).
 *          It can be used for array sizing for pin-related data if all pins up to MAX_PIN_ID are potentially configurable.
 *          The actual number of pins actively configured by the application is determined by the
 *          post-build configuration (e.g., size of PinConfig array in Port_PBcfg.c).
 */
#define PORT_NUMBER_OF_PINS                   (PORT_MAX_PIN_ID + 1U)


/*
 * Symbolic names for Port Pins
 * These macros define unique identifiers for each Port pin.
 * The values should be of type Port_PinType (typically uint8, as defined in Port.h).
 * Mapping: PortA Pins 0-15 are IDs 0-15, PortB Pins 0-15 are IDs 16-31, etc.
 */

/* PORT A: Pins 0 - 15 */
#define PORT_PIN_PA0                          ((uint8)0U)
#define PORT_PIN_PA1                          ((uint8)1U)
#define PORT_PIN_PA2                          ((uint8)2U)
#define PORT_PIN_PA3                          ((uint8)3U)
#define PORT_PIN_PA4                          ((uint8)4U)
#define PORT_PIN_PA5                          ((uint8)5U)  /* Used for LED in sample app */
#define PORT_PIN_PA6                          ((uint8)6U)
#define PORT_PIN_PA7                          ((uint8)7U)
#define PORT_PIN_PA8                          ((uint8)8U)
#define PORT_PIN_PA9                          ((uint8)9U)  /* Used for USART1_TX in sample app */
#define PORT_PIN_PA10                         ((uint8)10U) /* Used for USART1_RX in sample app */
#define PORT_PIN_PA11                         ((uint8)11U)
#define PORT_PIN_PA12                         ((uint8)12U)
#define PORT_PIN_PA13                         ((uint8)13U) /* SWDIO / TMS */
#define PORT_PIN_PA14                         ((uint8)14U) /* SWCLK / TCK */
#define PORT_PIN_PA15                         ((uint8)15U) /* JTDI */

/* PORT B: Pins 16 - 31 */
#define PORT_PIN_PB0                          ((uint8)16U)
#define PORT_PIN_PB1                          ((uint8)17U)
#define PORT_PIN_PB2                          ((uint8)18U)
#define PORT_PIN_PB3                          ((uint8)19U) /* JTDO / TRACESWO */
#define PORT_PIN_PB4                          ((uint8)20U) /* NJTRST */
#define PORT_PIN_PB5                          ((uint8)21U)
#define PORT_PIN_PB6                          ((uint8)22U)
#define PORT_PIN_PB7                          ((uint8)23U)
#define PORT_PIN_PB8                          ((uint8)24U)
#define PORT_PIN_PB9                          ((uint8)25U)
#define PORT_PIN_PB10                         ((uint8)26U)
#define PORT_PIN_PB11                         ((uint8)27U)
#define PORT_PIN_PB12                         ((uint8)28U)
#define PORT_PIN_PB13                         ((uint8)29U)
#define PORT_PIN_PB14                         ((uint8)30U)
#define PORT_PIN_PB15                         ((uint8)31U)

/* PORT C: Pins 32 - 47 */
#define PORT_PIN_PC0                          ((uint8)32U)
#define PORT_PIN_PC1                          ((uint8)33U)
#define PORT_PIN_PC2                          ((uint8)34U)
#define PORT_PIN_PC3                          ((uint8)35U)
#define PORT_PIN_PC4                          ((uint8)36U)
#define PORT_PIN_PC5                          ((uint8)37U)
#define PORT_PIN_PC6                          ((uint8)38U)
#define PORT_PIN_PC7                          ((uint8)39U)
#define PORT_PIN_PC8                          ((uint8)40U)
#define PORT_PIN_PC9                          ((uint8)41U)
#define PORT_PIN_PC10                         ((uint8)42U)
#define PORT_PIN_PC11                         ((uint8)43U)
#define PORT_PIN_PC12                         ((uint8)44U)
#define PORT_PIN_PC13                         ((uint8)45U) /* User Button in sample app / Tamper */
#define PORT_PIN_PC14                         ((uint8)46U) /* OSC32_IN */
#define PORT_PIN_PC15                         ((uint8)47U) /* OSC32_OUT */

/* PORT D: Pins 48 - 63 */
#define PORT_PIN_PD0                          ((uint8)48U)
/* ... Define other PD pins fully if used by application ... */
#define PORT_PIN_PD15                         ((uint8)63U)

/* PORT E: Pins 64 - 79 */
#define PORT_PIN_PE0                          ((uint8)64U)
/* ... Define other PE pins fully if used by application ... */
#define PORT_PIN_PE15                         ((uint8)79U)

/* PORT F: Pins 80 - 95 */
#define PORT_PIN_PF0                          ((uint8)80U)
/* ... Define other PF pins fully if used by application ... */
#define PORT_PIN_PF15                         ((uint8)95U)


#endif /* PORT_CFG_H */
