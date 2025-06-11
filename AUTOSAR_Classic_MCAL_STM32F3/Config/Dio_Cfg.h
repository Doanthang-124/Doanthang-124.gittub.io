#ifndef DIO_CFG_H
#define DIO_CFG_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
/* Include Std_Types for STD_ON/STD_OFF definitions, and basic types like uint8 */
/* This path assumes Dio_Cfg.h is in AUTOSAR_Classic_MCAL_STM32F3/Config/ */
#include "../Common/Include/Std_Types.h"
/* For Dio_ChannelType, Dio_PortType (though they are uint8, explicit include might be desired by some tools) */
/* However, Dio.h includes Dio_Cfg.h, so Dio_Cfg.h cannot include Dio.h directly. */
/* Rely on Std_Types.h for uint8, and the types will be formally defined when Dio.h is processed. */

/*==================================================================================================
*                                       PRE-COMPILE OPTIONS
==================================================================================================*/

/* Switches for Development Error Detection */
#define DIO_DEV_ERROR_DETECT        STD_ON

/* Switch for Version Information API */
#define DIO_VERSION_INFO_API        STD_ON

/* Switch for Dio_FlipChannel API */
#define DIO_FLIP_CHANNEL_API        STD_ON

/* Switch for Dio_MaskedWritePort API (AUTOSAR Extension) */
#define DIO_MASKED_WRITE_PORT_API   STD_ON

/*==================================================================================================
*                                     SYMBOLIC NAME DEFINITIONS
==================================================================================================*/

/* These symbolic names define the unique IDs for each DIO channel (pin), port, and group. */
/* The actual mapping to physical pins and ports depends on the specific MCU and board. */

/*--------------------------------------------------------------------------------------------------
 * DIO Channels (Pins)
 * A common scheme: PortA Pin0 = 0, ..., PortA Pin15 = 15, PortB Pin0 = 16, ...
 * Total channels will depend on how many ports are fully defined.
 * Max channels for STM32F3 (e.g. F303VC) could be up to Port F (6 ports * 16 pins/port = 96 channels)
 *------------------------------------------------------------------------------------------------*/

/* PORT A Channels (0-15) */
#define DIO_CHANNEL_PA0     ((uint8)0)
#define DIO_CHANNEL_PA1     ((uint8)1)
#define DIO_CHANNEL_PA2     ((uint8)2)
#define DIO_CHANNEL_PA3     ((uint8)3)
#define DIO_CHANNEL_PA4     ((uint8)4)
#define DIO_CHANNEL_PA5     ((uint8)5)
#define DIO_CHANNEL_PA6     ((uint8)6)
#define DIO_CHANNEL_PA7     ((uint8)7)
#define DIO_CHANNEL_PA8     ((uint8)8)
#define DIO_CHANNEL_PA9     ((uint8)9)
#define DIO_CHANNEL_PA10    ((uint8)10)
#define DIO_CHANNEL_PA11    ((uint8)11)
#define DIO_CHANNEL_PA12    ((uint8)12)
#define DIO_CHANNEL_PA13    ((uint8)13)  /* Typically SWDIO */
#define DIO_CHANNEL_PA14    ((uint8)14)  /* Typically SWCLK */
#define DIO_CHANNEL_PA15    ((uint8)15)

/* PORT B Channels (16-31) */
#define DIO_CHANNEL_PB0     ((uint8)16)
#define DIO_CHANNEL_PB1     ((uint8)17)
#define DIO_CHANNEL_PB2     ((uint8)18)
#define DIO_CHANNEL_PB3     ((uint8)19)
#define DIO_CHANNEL_PB4     ((uint8)20)
#define DIO_CHANNEL_PB5     ((uint8)21)
#define DIO_CHANNEL_PB6     ((uint8)22)
#define DIO_CHANNEL_PB7     ((uint8)23)
#define DIO_CHANNEL_PB8     ((uint8)24)
#define DIO_CHANNEL_PB9     ((uint8)25)
#define DIO_CHANNEL_PB10    ((uint8)26)
#define DIO_CHANNEL_PB11    ((uint8)27)
#define DIO_CHANNEL_PB12    ((uint8)28)
#define DIO_CHANNEL_PB13    ((uint8)29)
#define DIO_CHANNEL_PB14    ((uint8)30)
#define DIO_CHANNEL_PB15    ((uint8)31)

/* PORT C Channels (32-47) */
#define DIO_CHANNEL_PC0     ((uint8)32)
#define DIO_CHANNEL_PC1     ((uint8)33)
#define DIO_CHANNEL_PC2     ((uint8)34)
#define DIO_CHANNEL_PC3     ((uint8)35)
#define DIO_CHANNEL_PC4     ((uint8)36)
#define DIO_CHANNEL_PC5     ((uint8)37)
#define DIO_CHANNEL_PC6     ((uint8)38)
#define DIO_CHANNEL_PC7     ((uint8)39)
#define DIO_CHANNEL_PC8     ((uint8)40)
#define DIO_CHANNEL_PC9     ((uint8)41)
#define DIO_CHANNEL_PC10    ((uint8)42)
#define DIO_CHANNEL_PC11    ((uint8)43)
#define DIO_CHANNEL_PC12    ((uint8)44)
#define DIO_CHANNEL_PC13    ((uint8)45)
#define DIO_CHANNEL_PC14    ((uint8)46)
#define DIO_CHANNEL_PC15    ((uint8)47)

/* PORT D Channels (48-63) */
#define DIO_CHANNEL_PD0     ((uint8)48)
#define DIO_CHANNEL_PD1     ((uint8)49)
#define DIO_CHANNEL_PD2     ((uint8)50)
// ... (PD3 to PD15 defined similarly if they exist on target)
#define DIO_CHANNEL_PD15    ((uint8)63)


/* PORT E Channels (64-79) */
#define DIO_CHANNEL_PE0     ((uint8)64)
#define DIO_CHANNEL_PE1     ((uint8)65)
// ... (PE2 to PE15 defined similarly)
#define DIO_CHANNEL_PE15    ((uint8)79)

/* PORT F Channels (80-95) - Check MCU datasheet, F303 has limited Port F pins */
#define DIO_CHANNEL_PF0     ((uint8)80)
#define DIO_CHANNEL_PF1     ((uint8)81)
// ... (PF2 to PF15 defined similarly if they exist)
#define DIO_CHANNEL_PF9     ((uint8)89) // Example
#define DIO_CHANNEL_PF10    ((uint8)90) // Example


/*--------------------------------------------------------------------------------------------------
 * DIO Ports
 *------------------------------------------------------------------------------------------------*/
#define DIO_PORT_A          ((uint8)0)  /* GPIOA */
#define DIO_PORT_B          ((uint8)1)  /* GPIOB */
#define DIO_PORT_C          ((uint8)2)  /* GPIOC */
#define DIO_PORT_D          ((uint8)3)  /* GPIOD */
#define DIO_PORT_E          ((uint8)4)  /* GPIOE */
#define DIO_PORT_F          ((uint8)5)  /* GPIOF */

/*--------------------------------------------------------------------------------------------------
 * DIO Channel Groups
 * These are application specific and usually defined in a post-build configuration (Dio_PBcfg.c)
 * or by the application code. Pre-defining them here is less common unless they are fixed.
 * Example:
 * #define DIO_GROUP_LEDS       ((Dio_ChannelGroupType){DIO_PORT_A, (uint8)0, (uint16)0x00FF}) // Lower 8 bits of Port A
 * The Dio_ChannelGroupType struct itself is defined in Dio.h.
 * For now, no predefined groups in Dio_Cfg.h.
 *------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------
 * Configuration Set related defines (if applicable for Pre-Compile)
 * For Post-Build, these would typically be in Dio_PBcfg.h or generated.
 *------------------------------------------------------------------------------------------------*/
// Example: #define DIO_NUMBER_OF_CHANNELS        (uint8)96 // Max possible channels
// Example: #define DIO_NUMBER_OF_PORTS         (uint8)6
// Example: #define DIO_NUMBER_OF_CHANNEL_GROUPS  (uint8)0 // Initially no pre-defined groups

#endif /* DIO_CFG_H */
