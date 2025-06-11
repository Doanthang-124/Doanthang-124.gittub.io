/*==================================================================================================
*   Project              : AUTOSAR 4.x MCAL
*   Platform             : STM32
*   Peripheral           : GPIO
*   Dependencies         : STM32F3xx Series
*
*   Autosar Version      : 4.3.1
*   Autosar Revision     : ASR_REL_4_3_REV_0001
*   Sw Version           : 1.0.0
*   Build Version        : S32K14X_MCAL_1_0_0_RTM_ASR_REL_4_3_REV_0001_20190426
*
*   (c) Copyright 2006-2016 Freescale Semiconductor, Inc.
*   (c) Copyright 2017-2019 NXP
*   (c) Copyright 2023 YourName
*   All Rights Reserved.
==================================================================================================*/
/*==================================================================================================
==================================================================================================*/

#include "Dio_PBcfg.h" /* Primary header for Post-Build Configuration */
                       /* This should include Dio.h, which includes Dio_Cfg.h for symbolic names */

/*==================================================================================================
*                                       VERSION CHECKS
==================================================================================================*/
/* Check if Dio_PBcfg.c source file and Dio_PBcfg.h header file are of the same vendor */
/* (Assuming DIO_PBCFG_VENDOR_ID is defined in Dio_PBcfg.h, if not, this check needs adjustment or removal) */
/*
#if (DIO_PBCFG_VENDOR_ID_C != DIO_PBCFG_VENDOR_ID)
    #error "Dio_PBcfg.c and Dio_PBcfg.h have different vendor ids"
#endif
*/

/* Check if Dio_PBcfg.c source file and Dio_PBcfg.h header file are of the same AUTOSAR version */
#if ((DIO_PBCFG_AR_RELEASE_MAJOR_VERSION_C != DIO_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (DIO_PBCFG_AR_RELEASE_MINOR_VERSION_C != DIO_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (DIO_PBCFG_AR_RELEASE_REVISION_VERSION_C != DIO_PBCFG_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Dio_PBcfg.c and Dio_PBcfg.h are different"
#endif

/* Check if Dio_PBcfg.c source file and Dio_PBcfg.h header file are of the same Software version */
#if ((DIO_PBCFG_SW_MAJOR_VERSION_C != DIO_PBCFG_SW_MAJOR_VERSION) || \
     (DIO_PBCFG_SW_MINOR_VERSION_C != DIO_PBCFG_SW_MINOR_VERSION) || \
     (DIO_PBCFG_SW_PATCH_VERSION_C != DIO_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Dio_PBcfg.c and Dio_PBcfg.h are different"
#endif

/* Define the versions for this file */
#define DIO_PBCFG_SW_MAJOR_VERSION_C            1U
#define DIO_PBCFG_SW_MINOR_VERSION_C            0U
#define DIO_PBCFG_SW_PATCH_VERSION_C            0U

#define DIO_PBCFG_AR_RELEASE_MAJOR_VERSION_C    4U
#define DIO_PBCFG_AR_RELEASE_MINOR_VERSION_C    3U
#define DIO_PBCFG_AR_RELEASE_REVISION_VERSION_C 1U


/*==================================================================================================
*                                 POST BUILD CONFIGURATION STRUCTURES
==================================================================================================*/

/*
 * Array of Configured DIO Channels
 * Each entry specifies a DIO channel that is configured for use.
 * The Dio_ChannelPBConfigType is defined in Dio_PBcfg.h.
 * Channel IDs (e.g., DIO_CHANNEL_PA0) are defined in Dio_Cfg.h.
 */
STATIC const Dio_ChannelPBConfigType Dio_ConfiguredChannels[] =
{
    { .ChannelId = DIO_CHANNEL_PA0 }, /* Example: Port A, Pin 0 */
    { .ChannelId = DIO_CHANNEL_PA1 }, /* Example: Port A, Pin 1 */
    { .ChannelId = DIO_CHANNEL_PB5 }, /* Example: Port B, Pin 5 */
    { .ChannelId = DIO_CHANNEL_PC13 } /* Example: Port C, Pin 13 (often an User LED on Nucleo boards) */
    /* Add more configured channels as needed by the application */
};

/*
 * Array of Configured DIO Ports
 * Each entry specifies a DIO port that is configured for use.
 * The Dio_PortPBConfigType is defined in Dio_PBcfg.h.
 * Port IDs (e.g., DIO_PORT_A) are defined in Dio_Cfg.h.
 */
STATIC const Dio_PortPBConfigType Dio_ConfiguredPorts[] =
{
    { .PortId = DIO_PORT_A }, /* Example: Configure Port A for access */
    { .PortId = DIO_PORT_B }, /* Example: Configure Port B for access */
    { .PortId = DIO_PORT_C }  /* Example: Configure Port C for access */
    /* Add more configured ports as needed */
};

/*
 * Array of Configured DIO Channel Groups
 * Each entry defines a channel group. Dio_ChannelGroupType is defined in Dio.h.
 * Port IDs, masks, and offsets are used to define the group.
 */
STATIC const Dio_ChannelGroupType Dio_ConfiguredChannelGroups[] =
{
    {
        /* Group 1: Example - PA1, PA2, PA3 */
        .port   = DIO_PORT_A,
        .offset = 1U,             /* Start from pin 1 of Port A */
        .mask   = (uint16)0x000E  /* Mask for PA1, PA2, PA3 (0b0000000000001110) */
    },
    {
        /* Group 2: Example - PB0, PB1 (Lower 2 bits of PortB) */
        .port   = DIO_PORT_B,
        .offset = 0U,
        .mask   = (uint16)0x0003  /* Mask for PB0, PB1 (0b0000000000000011) */
    }
    /* Add more configured channel groups as needed */
};


/*
 * The Main Post-Build Configuration Structure for the DIO Driver.
 * This structure is declared as extern in Dio_PBcfg.h and is passed to Dio_Init().
 * It contains pointers to the arrays of configured channels, ports, and groups,
 * along with the number of elements in each array.
 */
#define DIO_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Dio_MemMap.h" /* Placeholder for MemMap include */

const Dio_ConfigType Dio_Config =
{
    .ChannelsConfig          = Dio_ConfiguredChannels,
    .NumConfiguredChannels   = (uint8)(sizeof(Dio_ConfiguredChannels) / sizeof(Dio_ChannelPBConfigType)),

    .PortsConfig             = Dio_ConfiguredPorts,
    .NumConfiguredPorts      = (uint8)(sizeof(Dio_ConfiguredPorts) / sizeof(Dio_PortPBConfigType)),

    .ChannelGroupsConfig     = Dio_ConfiguredChannelGroups,
    .NumConfiguredChannelGroups = (uint8)(sizeof(Dio_ConfiguredChannelGroups) / sizeof(Dio_ChannelGroupType))
};

#define DIO_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Dio_MemMap.h" /* Placeholder for MemMap include */
