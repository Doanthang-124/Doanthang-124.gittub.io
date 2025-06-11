#ifndef PORT_CFG_H
#define PORT_CFG_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
/* Include Std_Types for STD_ON/STD_OFF definitions, and basic types like uint8 */
#include "../Common/Include/Std_Types.h"

/*
 * Port.h includes Port_Cfg.h. Therefore, Port_Cfg.h should not include Port.h
 * to avoid circular dependencies. Types like Port_PinType are typically uint8,
 * so direct use of (uint8) or simple integer literals for defines is common.
 */

/*==================================================================================================
*                                       PRE-COMPILE OPTIONS
==================================================================================================*/

/**
 * @brief Switch to enable/disable development error detection for the Port driver.
 * @type {STD_ON, STD_OFF}
 */
#define PORT_DEV_ERROR_DETECT           STD_ON

/**
 * @brief Switch to enable/disable the Port_GetVersionInfo API.
 * @type {STD_ON, STD_OFF}
 */
#define PORT_VERSION_INFO_API           STD_ON

/**
 * @brief Switch to enable/disable the Port_SetPinDirection API.
 * @details If STD_ON, the direction of port pins can be changed during runtime.
 * @type {STD_ON, STD_OFF}
 */
#define PORT_SET_PIN_DIRECTION_API      STD_ON

/**
 * @brief Switch to enable/disable the Port_SetPinMode API.
 * @details If STD_ON, the mode of port pins can be changed during runtime.
 * @type {STD_ON, STD_OFF}
 */
#define PORT_SET_PIN_MODE_API           STD_ON

/**
 * @brief Defines if the direction of a port pin is changeable during runtime.
 * @details This parameter is relevant if PORT_SET_PIN_DIRECTION_API is STD_ON.
 *          It can be set globally or per pin in post-build configurations.
 *          For pre-compile, this often acts as a global master switch.
 * @type {STD_ON, STD_OFF}
 */
#define PORT_PIN_DIRECTION_CHANGEABLE   STD_ON

/**
 * @brief Defines if the mode of a port pin is changeable during runtime.
 * @details This parameter is relevant if PORT_SET_PIN_MODE_API is STD_ON.
 *          It can be set globally or per pin.
 * @type {STD_ON, STD_OFF}
 */
#define PORT_PIN_MODE_CHANGEABLE        STD_ON


/*==================================================================================================
*                                     SYMBOLIC NAME DEFINITIONS
==================================================================================================*/

/**
 * @brief Defines the total number of port pins configured or configurable by this driver.
 * @details This value can be used to size arrays in configuration structures.
 *          For STM32F303 (e.g., LQFP100 package), ports A-F are available,
 *          though not all pins might be bonded out or used.
 *          Max possible for 6 ports (A-F) * 16 pins/port = 96.
 */
#define PORT_NUMBER_OF_CONFIGURED_PINS    (96U)


/* Symbolic names for Port Pins */
/* These IDs are used to identify pins in the configuration and APIs. */
/* The type Port_PinType (typically uint8) must be able to hold these values. */

/* PORT A (Pins 0-15) */
#define PORT_PIN_PA0     (0U)
#define PORT_PIN_PA1     (1U)
#define PORT_PIN_PA2     (2U)
#define PORT_PIN_PA3     (3U)
#define PORT_PIN_PA4     (4U)
#define PORT_PIN_PA5     (5U)
#define PORT_PIN_PA6     (6U)
#define PORT_PIN_PA7     (7U)
#define PORT_PIN_PA8     (8U)
#define PORT_PIN_PA9     (9U)
#define PORT_PIN_PA10    (10U)
#define PORT_PIN_PA11    (11U)
#define PORT_PIN_PA12    (12U)
#define PORT_PIN_PA13    (13U)  /* Corresponds to SWDIO/TMS */
#define PORT_PIN_PA14    (14U)  /* Corresponds to SWCLK/TCK */
#define PORT_PIN_PA15    (15U)  /* Corresponds to JTDI */

/* PORT B (Pins 16-31) */
#define PORT_PIN_PB0     (16U)
#define PORT_PIN_PB1     (17U)
#define PORT_PIN_PB2     (18U)
#define PORT_PIN_PB3     (19U)  /* Corresponds to JTDO/TRACESWO */
#define PORT_PIN_PB4     (20U)  /* Corresponds to NJTRST */
#define PORT_PIN_PB5     (21U)
#define PORT_PIN_PB6     (22U)
#define PORT_PIN_PB7     (23U)
#define PORT_PIN_PB8     (24U)
#define PORT_PIN_PB9     (25U)
#define PORT_PIN_PB10    (26U)
#define PORT_PIN_PB11    (27U)
#define PORT_PIN_PB12    (28U)
#define PORT_PIN_PB13    (29U)
#define PORT_PIN_PB14    (30U)
#define PORT_PIN_PB15    (31U)

/* PORT C (Pins 32-47) */
#define PORT_PIN_PC0     (32U)
#define PORT_PIN_PC1     (33U)
#define PORT_PIN_PC2     (34U)
#define PORT_PIN_PC3     (35U)
#define PORT_PIN_PC4     (36U)
#define PORT_PIN_PC5     (37U)
#define PORT_PIN_PC6     (38U)
#define PORT_PIN_PC7     (39U)
#define PORT_PIN_PC8     (40U)
#define PORT_PIN_PC9     (41U)
#define PORT_PIN_PC10    (42U)
#define PORT_PIN_PC11    (43U)
#define PORT_PIN_PC12    (44U)
#define PORT_PIN_PC13    (45U)  /* Often connected to Anti-Tamper, RTC, or User Button */
#define PORT_PIN_PC14    (46U)  /* OSC32_IN */
#define PORT_PIN_PC15    (47U)  /* OSC32_OUT */

/* PORT D (Pins 48-63) */
#define PORT_PIN_PD0     (48U)  /* Can be OSC_IN on some packages */
#define PORT_PIN_PD1     (49U)  /* Can be OSC_OUT on some packages */
#define PORT_PIN_PD2     (50U)
/* ... up to PD15 */
#define PORT_PIN_PD15    (63U)

/* PORT E (Pins 64-79) */
#define PORT_PIN_PE0     (64U)
/* ... up to PE15 */
#define PORT_PIN_PE15    (79U)

/* PORT F (Pins 80-95) - Availability depends on MCU package */
#define PORT_PIN_PF0     (80U)  /* Can be OSC_IN on some packages if PD0/1 are remapped or not used */
#define PORT_PIN_PF1     (81U)  /* Can be OSC_OUT on some packages if PD0/1 are remapped or not used */
#define PORT_PIN_PF2     (82U)
/* ... up to PF15 (many STM32F3 packages have limited Port F pins, e.g. PF0, PF1, PF2, PF4, PF6, PF9, PF10) */
#define PORT_PIN_PF15    (95U)


/*==================================================================================================
*                                      DEFAULT CONFIGURATIONS
==================================================================================================*/
/*
 * Default settings for pins if certain APIs are disabled.
 * For example, if PORT_SET_PIN_DIRECTION_API is STD_OFF, all pins might default to a direction
 * specified here or in the post-build configuration.
 * These are highly dependent on specific project needs if APIs are disabled.
 */
// #define PORT_DEFAULT_PIN_DIRECTION      PORT_PIN_IN
// #define PORT_DEFAULT_PIN_MODE           PORT_PIN_MODE_GPIO_IN_FLOATING

#endif /* PORT_CFG_H */
