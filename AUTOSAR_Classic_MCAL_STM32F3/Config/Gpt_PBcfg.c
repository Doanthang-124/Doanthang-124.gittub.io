/*==================================================================================================
*   Project              : AUTOSAR 4.x MCAL
*   Platform             : STM32
*   Peripheral           : GPT
*   Dependencies         : STM32F3xx Series
*
*   Autosar Version      : 4.3.1
*   Autosar Revision     : ASR_REL_4_3_REV_0001
*   Sw Version           : 1.0.0
*   Build Version        : GPT_MCAL_1_0_0_RTM_ASR_REL_4_3_REV_0001_2024xxxx
*
*   (c) Copyright 2023-2024 YourName
*   All Rights Reserved.
==================================================================================================*/

#include "Gpt_PBcfg.h"  /* Primary header for Gpt Post-Build configurations.
                           Includes Gpt.h, which includes Gpt_Cfg.h for pre-compile options
                           and Std_Types.h (which includes Compiler.h). */

/*==================================================================================================
*                                   VERSION CHECK
==================================================================================================*/
/* Check if Gpt_PBcfg.c and Gpt_PBcfg.h file versions are compatible */
#if ((GPT_PBCFG_SW_MAJOR_VERSION != 1U) || \
     (GPT_PBCFG_SW_MINOR_VERSION != 0U) || \
     (GPT_PBCFG_SW_PATCH_VERSION != 0U))
    #error "Software Version Numbers of Gpt_PBcfg.c and Gpt_PBcfg.h are different"
#endif

#if ((GPT_PBCFG_AR_RELEASE_MAJOR_VERSION    != 4U) || \
     (GPT_PBCFG_AR_RELEASE_MINOR_VERSION    != 3U) || \
     (GPT_PBCFG_AR_RELEASE_REVISION_VERSION != 1U))
    #error "AUTOSAR Version Numbers of Gpt_PBcfg.c and Gpt_PBcfg.h are different"
#endif

/* Check Gpt_PBcfg.c against Gpt.h (included via Gpt_PBcfg.h) */
#if ((GPT_MODULE_ID != 100U)) /* Example check, assuming GPT_MODULE_ID defined in Gpt.h */
    /* Error if versions are not compatible */
#endif


/*==================================================================================================
*                          PRECOMPILE AND CONFIGURATION CONSISTENCY CHECKS
==================================================================================================*/
/* Check that the number of channels defined in this PBcfg matches Gpt_Cfg.h */
#if (GPT_CONFIGURED_CHANNELS > 0) /* Only proceed if there are channels to configure */
    /* This check is more of a static assertion for the developer. */
    /* If Gpt_ChannelConfigurations array below has a different size than GPT_CONFIGURED_CHANNELS,
       it would ideally lead to a compile error or warning.
       The array is explicitly sized with GPT_CONFIGURED_CHANNELS, so this is implicitly handled.
    */
#endif


/*==================================================================================================
*                                     FUNCTION PROTOTYPES (Callbacks)
==================================================================================================*/

#if (GPT_ENABLE_DISABLE_NOTIFICATION_API == STD_ON) /* From Gpt_Cfg.h */
/**
 * @brief Example notification function for GPT Channel 0.
 * @details This function would be defined in the application code (e.g., main.c or a callback module).
 *          It is declared here as extern to be assigned in the configuration structure.
 *          The 'APPL_CODE' memclass should be defined in Compiler.h.
 */
extern FUNC(void, APPL_CODE) Gpt_Notification_Channel0(void);

/**
 * @brief Example notification function for GPT Channel 1.
 */
extern FUNC(void, APPL_CODE) Gpt_Notification_Channel1(void);

/* Add other notification function prototypes as needed for configured channels */
#endif


/*==================================================================================================
*                            POST-BUILD CONFIGURATION STRUCTURES
==================================================================================================*/

/**
 * @brief Array of GPT Channel Configurations.
 * @details Each element configures one logical GPT channel, mapping it to a hardware timer
 *          and specifying its operational parameters.
 *          The size of this array is determined by `GPT_CONFIGURED_CHANNELS` from `Gpt_Cfg.h`.
 */
#define GPT_START_SEC_CONFIG_DATA_UNSPECIFIED
/* #include "MemMap.h" */ /* Should be Gpt_MemMap.h or Bsw_MemMap.h */

#if (GPT_CONFIGURED_CHANNELS > 0)
STATIC CONST(Gpt_ChannelConfigType, CONFIG_CONST) Gpt_ChannelConfigurations[GPT_CONFIGURED_CHANNELS] =
{
    /* Channel 0 Configuration */
    /* Example: Map logical GPT_CHANNEL_0 to STM32 TIM2 (General-purpose 32-bit timer) */
    {
        .GptChannelId                 = GPT_CHANNEL_0,         /* Logical Channel ID from Gpt_Cfg.h */
        .HwTimer                      = GPT_HW_STM32_TIM2,     /* Hardware timer selection from Gpt_PBcfg.h enum */
        .GptChannelMode               = GPT_MODE_CONTINUOUS,   /* Run timer continuously */
        .GptChannelTickFrequencyHz    = 1000000UL,             /* Target 1MHz tick frequency (1us resolution) */
        .GptChannelTickValueMax       = 0xFFFFFFFFUL,          /* Max ticks for TIM2 (32-bit) */
    #if (GPT_ENABLE_DISABLE_NOTIFICATION_API == STD_ON)
        .GptNotification              = &Gpt_Notification_Channel0 /* User-defined callback for Channel 0 */
    #else
        .GptNotification              = NULL_PTR
    #endif
    #if (GPT_WAKEUP_FUNCTIONALITY_API == STD_ON)
        // .GptEnableWakeup              = FALSE, /* Example, if wakeup was supported */
        // .GptWakeupSourceId            = 0 /* Example EcuM Wakeup Source ID */
    #endif
    },

#if (GPT_CONFIGURED_CHANNELS > 1)
    /* Channel 1 Configuration */
    /* Example: Map logical GPT_CHANNEL_1 to STM32 TIM6 (Basic 16-bit timer) */
    {
        .GptChannelId                 = GPT_CHANNEL_1,
        .HwTimer                      = GPT_HW_STM32_TIM6,
        .GptChannelMode               = GPT_MODE_ONESHOT,      /* Run timer once per Gpt_StartTimer call */
        .GptChannelTickFrequencyHz    = 1000UL,                /* Target 1kHz tick frequency (1ms resolution) */
        .GptChannelTickValueMax       = 0xFFFFU,               /* Max ticks for TIM6 (16-bit) */
    #if (GPT_ENABLE_DISABLE_NOTIFICATION_API == STD_ON)
        .GptNotification              = &Gpt_Notification_Channel1 /* User-defined callback for Channel 1 */
    #else
        .GptNotification              = NULL_PTR
    #endif
    #if (GPT_WAKEUP_FUNCTIONALITY_API == STD_ON)
        // .GptEnableWakeup              = FALSE,
    #endif
    }
#endif
    /* Add more channel configurations here if GPT_CONFIGURED_CHANNELS is greater than 2, */
    /* ensuring the array size matches GPT_CONFIGURED_CHANNELS. */
};
#endif /* (GPT_CONFIGURED_CHANNELS > 0) */


/**
 * @brief Main GPT Driver Post-Build Configuration Structure instance.
 * @details This structure is declared as `extern const Gpt_ConfigType GptConfig;` in `Gpt_PBcfg.h`.
 *          It is passed to `Gpt_Init()` by the application or an upper layer module.
 *          It contains a pointer to the channel configurations array and the total number of channels.
 *          The memclass `CONFIG_CONST` should be defined in `Compiler.h`.
 */
CONST(Gpt_ConfigType, CONFIG_CONST) GptConfig =
{
#if (GPT_CONFIGURED_CHANNELS > 0)
    .ChannelConfigSet    = Gpt_ChannelConfigurations,
#else
    .ChannelConfigSet    = NULL_PTR, /* No channels configured */
#endif
    .NumberOfChannels    = GPT_CONFIGURED_CHANNELS
};

#define GPT_STOP_SEC_CONFIG_DATA_UNSPECIFIED
/* #include "MemMap.h" */
