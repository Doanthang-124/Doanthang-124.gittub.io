/*==================================================================================================
*   Project              : AUTOSAR 4.x MCAL
*   Platform             : STM32
*   Peripheral           : DIO
*   Dependencies         : STM32F3xx Series
*
*   Autosar Version      : 4.3.1
*   Autosar Revision     : ASR_REL_4_3_REV_0001
*   Sw Version           : 1.0.0
*   Build Version        : DIO_PBCFG_MCAL_1_0_0_RTM_ASR_REL_4_3_REV_0001_2024xxxx
*
==================================================================================================*/

#include "Dio_PBcfg.h"  /* For Dio_ConfigType, Dio_ChannelConfigType, etc. (Includes Std_Types.h and Compiler.h) */
                        /* Dio_PBcfg.h also defines Dio_ChannelType, Dio_PortType, etc. */
#include "Dio_Cfg.h"    /* For symbolic names like DIO_CHANNEL_PA0, DIO_PORT_A, and pre-compile switches */

/*==================================================================================================
*                                   VERSION CHECK
==================================================================================================*/
/* Check if Dio_PBcfg.c and Dio_PBcfg.h have the same SW version */
#if ((DIO_PBCFG_SW_MAJOR_VERSION != 1U) ||      \
     (DIO_PBCFG_SW_MINOR_VERSION != 0U) ||      \
     (DIO_PBCFG_SW_PATCH_VERSION != 0U))
  #error "Software Version Numbers of Dio_PBcfg.c and Dio_PBcfg.h are different"
#endif

/* Check if Dio_PBcfg.c and Dio_PBcfg.h have the same AUTOSAR version */
#if ((DIO_PBCFG_AR_RELEASE_MAJOR_VERSION != 4U) ||      \
     (DIO_PBCFG_AR_RELEASE_MINOR_VERSION != 3U) ||      \
     (DIO_PBCFG_AR_RELEASE_PATCH_VERSION != 1U))
  #error "AUTOSAR Version Numbers of Dio_PBcfg.c and Dio_PBcfg.h are different"
#endif

/* Check if Dio_PBcfg.c and Dio_Cfg.h have compatible versions (developer responsibility) */
/* Example: check DIO_MODULE_ID if defined in both, or specific config parameters. */


/*==================================================================================================
*                          POST-BUILD CONFIGURATION STRUCTURES
==================================================================================================*/
#define DIO_START_SEC_CONFIG_DATA_UNSPECIFIED
/* #include "MemMap.h" */ /* Or Dio_MemMap.h */


/**
 * @brief Example Post-Build Configuration for DIO Channels.
 * @details Each element defines a DIO channel used by the application.
 *          The `ChannelId` should correspond to a symbolic name from `Dio_Cfg.h`.
 */
STATIC CONST(Dio_ChannelConfigType, CONFIG_CONST) Dio_axChannelConfigData[] =
{
    /* Channel Configuration 0 */
    {
        .ChannelId = DIO_CHANNEL_PA0 /* Mapped to Pin PA0 */
    },
    /* Channel Configuration 1 */
    {
        .ChannelId = DIO_CHANNEL_PA5 /* Mapped to Pin PA5 (e.g., LED for sample app) */
    },
    /* Channel Configuration 2 */
    {
        .ChannelId = DIO_CHANNEL_PC13 /* Mapped to Pin PC13 (e.g., Button for sample app) */
    }
    /* Add more configured channels here based on application needs.
       The number of initializers here should ideally match a pre-compile configured value
       if such a value (e.g., DIO_CONFIGURED_CHANNELS_PB) is defined in Dio_Cfg.h.
       Alternatively, the size is derived using sizeof() when populating DioConfig.
    */
};

/**
 * @brief Example Post-Build Configuration for DIO Ports.
 * @details Each element defines a DIO port used by the application.
 */
STATIC CONST(Dio_PortConfigType, CONFIG_CONST) Dio_axPortConfigData[] =
{
    /* Port Configuration 0 */
    {
        .PortId = DIO_PORT_A /* Mapped to GPIOA */
    },
    /* Port Configuration 1 */
    {
        .PortId = DIO_PORT_C /* Mapped to GPIOC */
    }
    /* Add more configured ports if needed */
};

/**
 * @brief Example Post-Build Configuration for DIO Channel Groups.
 * @details Each element defines a group of channels within a port.
 *          The 'Mask' identifies the pins within the port.
 *          The 'Offset' is the bit position of the LSB of the group within the port.
 *          The 'Level' parameter for WriteChannelGroup is relative to the group's LSB (i.e., unshifted).
 */
STATIC CONST(Dio_ChannelGroupType, CONFIG_CONST) Dio_axChannelGroupData[] =
{
    /* Channel Group Configuration 0: Example - Lower 4 bits of Port A (PA0-PA3) */
    {
        .PortIndex = DIO_PORT_A,    /* Group is on Port A */
        .Offset    = 0U,            /* LSB of the group is Pin 0 of Port A */
        .Mask      = (Dio_PortLevelType)0x000FU  /* Mask for PA0, PA1, PA2, PA3 (bits 0-3) */
    },
    /* Channel Group Configuration 1: Example - Specific pins PB5, PB6 on Port B */
    {
        .PortIndex = DIO_PORT_B,    /* Group is on Port B */
        .Offset    = 5U,            /* LSB of the group is Pin 5 of Port B (PB5) */
        .Mask      = (Dio_PortLevelType)0x0060U /* Mask for bits 5 and 6 (PB5, PB6) of Port B */
                                    /* When writing, Level would be 2 bits (e.g., 0b01 for PB5=H, PB6=L, if LSB of Level maps to PB5) */
                                    /* Or, more typically, Level is already shifted if Offset is used for writing. */
                                    /* SWS: "The Level parameter contains the values to be written to the channels of the group, right aligned." */
                                    /* So, if group is PB5,PB6: Level 0b01 -> PB5=H, PB6=L. */
                                    /* The WriteChannelGroup implementation must shift Level by Offset before masking and writing. */
    }
    /* Add more channel groups if needed */
};


/**
 * @brief Main Post-Build Configuration Structure instance for the DIO Driver.
 * @details This structure is declared as `extern CONST(Dio_ConfigType, CONFIG_CONST) DioConfig;` in `Dio_PBcfg.h`.
 *          It is passed to `Dio_Init()` by the application or an upper layer module.
 *          The `CONFIG_CONST` memory class from `Compiler.h` ensures it's placed in a read-only section.
 */
CONST(Dio_ConfigType, CONFIG_CONST) DioConfig =
{
    .Channels    = Dio_axChannelConfigData,
    .NumChannels = (uint8)(sizeof(Dio_axChannelConfigData) / sizeof(Dio_ChannelConfigType)),

    .Ports       = Dio_axPortConfigData,
    .NumPorts    = (uint8)(sizeof(Dio_axPortConfigData) / sizeof(Dio_PortConfigType)),

    .Groups      = Dio_axChannelGroupData, /* Using the corrected name from the plan analysis */
    .NumGroups   = (uint8)(sizeof(Dio_axChannelGroupData) / sizeof(Dio_ChannelGroupType))

    /* If, for example, no groups were configured for a specific application:
    .Groups      = NULL_PTR,
    .NumGroups   = 0U
    */
};

#define DIO_STOP_SEC_CONFIG_DATA_UNSPECIFIED
/* #include "MemMap.h" */ /* Or Dio_MemMap.h */
