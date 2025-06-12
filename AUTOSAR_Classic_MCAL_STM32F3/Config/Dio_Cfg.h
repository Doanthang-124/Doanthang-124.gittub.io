#ifndef DIO_CFG_H
#define DIO_CFG_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "Std_Types.h"      /* For STD_ON, STD_OFF definitions */
/* For Dio_ChannelType, Dio_PortType - these are uint8, so direct use or casting to uint8 is fine.
   If Dio.h (where they are typedef'd) were included here, it could create a circular dependency
   as Dio.h includes Dio_Cfg.h. It's common for _Cfg.h to define IDs as raw numbers or cast to base types.
*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/**
 * @brief Instance ID for the DIO module.
 * @details Typically 0 for non-reentrant modules.
 */
#define DIO_INSTANCE_ID                       (0U)


/**
 * @brief Switch to enable or disable development error detection for the DIO module.
 * @details STD_ON: Enable development error detection.
 *          STD_OFF: Disable development error detection.
 */
#define DIO_DEV_ERROR_DETECT                  STD_ON

/**
 * @brief Switch to enable or disable the Dio_GetVersionInfo API.
 * @details STD_ON: Dio_GetVersionInfo API is available.
 *          STD_OFF: Dio_GetVersionInfo API is not available.
 */
#define DIO_VERSION_INFO_API                  STD_ON

/**
 * @brief Switch to enable or disable the Dio_FlipChannel API.
 * @details STD_ON: Dio_FlipChannel API is available.
 *          STD_OFF: Dio_FlipChannel API is not available.
 */
#define DIO_FLIP_CHANNEL_API                  STD_ON

/**
 * @brief Switch to enable or disable the Dio_MaskedWritePort API.
 * @details STD_ON: Dio_MaskedWritePort API is available.
 *          STD_OFF: Dio_MaskedWritePort API is not available.
 *          (Note: This API was implemented in the basic Dio.c)
 */
#define DIO_MASKED_WRITE_PORT_API             STD_ON /* Set back to STD_ON as it is implemented */


/*
 * Symbolic names for DIO Channels (Pins)
 * These macros define unique identifiers for each DIO channel.
 * The values should be of type Dio_ChannelType (typically uint8).
 * Mapping: PortA Pins 0-15 are Channels 0-15, PortB Pins 0-15 are Channels 16-31, etc.
 * Ensure these values are cast to (uint8) or use U suffix for literals if not directly using Dio_ChannelType.
 */

/* PORT A: Channels 0 - 15 */
#define DIO_CHANNEL_PA0                       ((uint8)0U)
#define DIO_CHANNEL_PA1                       ((uint8)1U)
#define DIO_CHANNEL_PA2                       ((uint8)2U)
#define DIO_CHANNEL_PA3                       ((uint8)3U)
#define DIO_CHANNEL_PA4                       ((uint8)4U)
#define DIO_CHANNEL_PA5                       ((uint8)5U)  /* Used for LED in sample app */
#define DIO_CHANNEL_PA6                       ((uint8)6U)
#define DIO_CHANNEL_PA7                       ((uint8)7U)
#define DIO_CHANNEL_PA8                       ((uint8)8U)
#define DIO_CHANNEL_PA9                       ((uint8)9U)
#define DIO_CHANNEL_PA10                      ((uint8)10U)
#define DIO_CHANNEL_PA11                      ((uint8)11U)
#define DIO_CHANNEL_PA12                      ((uint8)12U)
#define DIO_CHANNEL_PA13                      ((uint8)13U) /* SWDIO */
#define DIO_CHANNEL_PA14                      ((uint8)14U) /* SWCLK */
#define DIO_CHANNEL_PA15                      ((uint8)15U)

/* PORT B: Channels 16 - 31 */
#define DIO_CHANNEL_PB0                       ((uint8)16U)
#define DIO_CHANNEL_PB1                       ((uint8)17U)
#define DIO_CHANNEL_PB2                       ((uint8)18U)
#define DIO_CHANNEL_PB3                       ((uint8)19U)
#define DIO_CHANNEL_PB4                       ((uint8)20U)
#define DIO_CHANNEL_PB5                       ((uint8)21U)
#define DIO_CHANNEL_PB6                       ((uint8)22U)
#define DIO_CHANNEL_PB7                       ((uint8)23U)
#define DIO_CHANNEL_PB8                       ((uint8)24U)
#define DIO_CHANNEL_PB9                       ((uint8)25U)
#define DIO_CHANNEL_PB10                      ((uint8)26U)
#define DIO_CHANNEL_PB11                      ((uint8)27U)
#define DIO_CHANNEL_PB12                      ((uint8)28U)
#define DIO_CHANNEL_PB13                      ((uint8)29U)
#define DIO_CHANNEL_PB14                      ((uint8)30U)
#define DIO_CHANNEL_PB15                      ((uint8)31U)

/* PORT C: Channels 32 - 47 */
#define DIO_CHANNEL_PC0                       ((uint8)32U)
#define DIO_CHANNEL_PC1                       ((uint8)33U)
#define DIO_CHANNEL_PC2                       ((uint8)34U)
#define DIO_CHANNEL_PC3                       ((uint8)35U)
#define DIO_CHANNEL_PC4                       ((uint8)36U)
#define DIO_CHANNEL_PC5                       ((uint8)37U)
#define DIO_CHANNEL_PC6                       ((uint8)38U)
#define DIO_CHANNEL_PC7                       ((uint8)39U)
#define DIO_CHANNEL_PC8                       ((uint8)40U)
#define DIO_CHANNEL_PC9                       ((uint8)41U)
#define DIO_CHANNEL_PC10                      ((uint8)42U)
#define DIO_CHANNEL_PC11                      ((uint8)43U)
#define DIO_CHANNEL_PC12                      ((uint8)44U)
#define DIO_CHANNEL_PC13                      ((uint8)45U) /* Used for Button in sample app */
#define DIO_CHANNEL_PC14                      ((uint8)46U)
#define DIO_CHANNEL_PC15                      ((uint8)47U)

/* PORT D: Channels 48 - 63 (Example, actual pins depend on STM32 package) */
#define DIO_CHANNEL_PD0                       ((uint8)48U)
/* ... Define other PD pins fully if used by application ... */
#define DIO_CHANNEL_PD15                      ((uint8)63U)

/* PORT E: Channels 64 - 79 */
#define DIO_CHANNEL_PE0                       ((uint8)64U)
/* ... Define other PE pins fully if used by application ... */
#define DIO_CHANNEL_PE15                      ((uint8)79U)

/* PORT F: Channels 80 - 95 */
#define DIO_CHANNEL_PF0                       ((uint8)80U)
/* ... Define other PF pins fully if used by application ... */
#define DIO_CHANNEL_PF15                      ((uint8)95U)


/*
 * Symbolic names for DIO Ports
 * These macros define unique identifiers for each DIO port.
 * The values should be of type Dio_PortType (typically uint8).
 */
#define DIO_PORT_A                            ((uint8)0U)   /* GPIOA */
#define DIO_PORT_B                            ((uint8)1U)   /* GPIOB */
#define DIO_PORT_C                            ((uint8)2U)   /* GPIOC */
#define DIO_PORT_D                            ((uint8)3U)   /* GPIOD */
#define DIO_PORT_E                            ((uint8)4U)   /* GPIOE */
#define DIO_PORT_F                            ((uint8)5U)   /* GPIOF */
/* Add Port G, H if applicable for the target MCU */


/* Configuration for number of channels/ports/groups if needed by other pre-compile checks
   or static array sizing in other configuration files (though PBcfg usually handles this).
   Example:
*/
// #define DIO_MAX_CHANNELS    (96U) /* Maximum possible channels with this naming scheme for 6 Ports A-F */
// #define DIO_MAX_PORTS       (6U)  /* Maximum ports A-F */

#endif /* DIO_CFG_H */
