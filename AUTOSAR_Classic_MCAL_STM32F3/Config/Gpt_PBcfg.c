/*==================================================================================================
*   Project              : AUTOSAR 4.x MCAL
*   Platform             : STM32
*   Peripheral           : GPT
*   Dependencies         : STM32F3xx Series
*
*   Autosar Version      : 4.3.1
*   Autosar Revision     : ASR_REL_4_3_REV_0001
*   Sw Version           : 1.0.0
*   Build Version        : GPT_PBCFG_MCAL_1_0_0_RTM_ASR_REL_4_3_REV_0001_2024xxxx
*
==================================================================================================*/

#include "Gpt_PBcfg.h"  /* For Gpt_ConfigType, Gpt_ChannelConfigType, and fundamental Gpt types */
                        /* Includes Std_Types.h (and Compiler.h) */
#include "Gpt_Cfg.h"    /* For symbolic channel names (GPT_CHANNEL_0, etc.) and GPT_CONFIGURED_CHANNELS, GPT_ENABLE_DISABLE_NOTIFICATION_API */

/*==================================================================================================
*                                   VERSION CHECK
==================================================================================================*/
/* Check if Gpt_PBcfg.c and Gpt_PBcfg.h have the same SW version */
#if ((GPT_PBCFG_SW_MAJOR_VERSION != 1U) || \
     (GPT_PBCFG_SW_MINOR_VERSION != 0U) || \
     (GPT_PBCFG_SW_PATCH_VERSION != 0U))
  #error "Software Version Numbers of Gpt_PBcfg.c and Gpt_PBcfg.h are different"
#endif

/* Check if Gpt_PBcfg.c and Gpt_PBcfg.h have the same AUTOSAR version */
#if ((GPT_PBCFG_AR_RELEASE_MAJOR_VERSION != 4U) || \
     (GPT_PBCFG_AR_RELEASE_MINOR_VERSION != 3U) || \
     (GPT_PBCFG_AR_RELEASE_PATCH_VERSION != 1U))
  #error "AUTOSAR Version Numbers of Gpt_PBcfg.c and Gpt_PBcfg.h are different"
#endif

/* Check if Gpt_PBcfg.c and Gpt_Cfg.h have compatible versions (developer responsibility) */
/* Example: #if (GPT_INSTANCE_ID_CFG != GPT_INSTANCE_ID_PBCFG) ... */


/*==================================================================================================
*                                     FUNCTION PROTOTYPES (Callbacks)
==================================================================================================*/

/*
 * Extern declarations for user-defined notification functions.
 * These functions must be defined in application code if configured for any channel.
 * They are declared here to be assigned to Gpt_ChannelConfigType.GptNotification members.
 */
#if (GPT_ENABLE_DISABLE_NOTIFICATION_API == STD_ON) /* From Gpt_Cfg.h */
    /** @brief Example notification callback for GPT Channel 0. */
    extern FUNC(void, APPL_CODE) Gpt_Notification_Channel0(void);

    /** @brief Example notification callback for GPT Channel 1. */
    extern FUNC(void, APPL_CODE) Gpt_Notification_Channel1(void);

    /* Add extern declarations for other notification functions if more channels use them. */
#endif /* GPT_ENABLE_DISABLE_NOTIFICATION_API */


/*==================================================================================================
*                            POST-BUILD CONFIGURATION STRUCTURES
==================================================================================================*/
#define GPT_START_SEC_CONFIG_DATA_UNSPECIFIED
/* #include "MemMap.h" */ /* Or Gpt_MemMap.h */

/**
 * @brief Array of GPT Channel Configurations.
 * @details Each element configures a single GPT channel. The number of elements
 *          is determined by `GPT_CONFIGURED_CHANNELS` from `Gpt_Cfg.h`.
 *          Memory class `CONFIG_CONST` places this in a read-only section.
 */
STATIC CONST(Gpt_ChannelConfigType, CONFIG_CONST) Gpt_axChannelConfigurations[GPT_CONFIGURED_CHANNELS] =
{
#if (GPT_CONFIGURED_CHANNELS > 0U)
    /* Channel 0 Configuration - Example using TIM2 (32-bit general purpose timer on STM32F303) */
    {
        .GptChannelId                 = GPT_CHANNEL_0,        /* Logical Channel ID from Gpt_Cfg.h */
        .HwTimer                      = STM32_TIM2,           /* Maps to TIM2 peripheral (enum from Gpt_PBcfg.h) */
        .GptChannelMode               = GPT_MODE_CONTINUOUS,  /* Run continuously */
        .GptChannelTickFrequencyHz    = 1000000UL,            /* Target 1 MHz tick frequency (1 microsecond resolution) */
        .GptChannelTickValueMax       = 0xFFFFFFFFUL,         /* TIM2 is used as a 32-bit timer */
#if (GPT_ENABLE_DISABLE_NOTIFICATION_API == STD_ON)
        .GptNotification              = &Gpt_Notification_Channel0 /* Pointer to user callback */
#else
        .GptNotification              = NULL_PTR
#endif
#if (defined(GPT_WAKEUP_FUNCTIONALITY_API) && (GPT_WAKEUP_FUNCTIONALITY_API == STD_ON))
        /* .GptEnableWakeup        = FALSE, */ /* Initialize if field exists */
#endif
    },
#endif /* GPT_CONFIGURED_CHANNELS > 0U */

#if (GPT_CONFIGURED_CHANNELS > 1U)
    /* Channel 1 Configuration - Example using TIM6 (16-bit basic timer) */
    {
        .GptChannelId                 = GPT_CHANNEL_1,        /* Logical Channel ID from Gpt_Cfg.h */
        .HwTimer                      = STM32_TIM6,           /* Maps to TIM6 peripheral */
        .GptChannelMode               = GPT_MODE_ONESHOT,     /* Run once per Gpt_StartTimer call */
        .GptChannelTickFrequencyHz    = 1000UL,               /* Target 1 kHz tick frequency (1 millisecond resolution) */
        .GptChannelTickValueMax       = 0xFFFFU,              /* TIM6 is a 16-bit timer */
#if (GPT_ENABLE_DISABLE_NOTIFICATION_API == STD_ON)
        .GptNotification              = &Gpt_Notification_Channel1 /* Pointer to user callback */
#else
        .GptNotification              = NULL_PTR
#endif
#if (defined(GPT_WAKEUP_FUNCTIONALITY_API) && (GPT_WAKEUP_FUNCTIONALITY_API == STD_ON))
        /* .GptEnableWakeup        = FALSE, */
#endif
    }
#endif /* GPT_CONFIGURED_CHANNELS > 1U */

    /* Add more channel configurations here if GPT_CONFIGURED_CHANNELS is greater than 2,
       ensuring each is guarded by `#if (GPT_CONFIGURED_CHANNELS > N)`.
    */
};


/**
 * @brief Main Post-Build Configuration Structure instance for the GPT Driver.
 * @details This structure is passed to `Gpt_Init()` to configure the GPT module.
 *          It points to the `Gpt_axChannelConfigurations` array and uses `GPT_CONFIGURED_CHANNELS`
 *          from `Gpt_Cfg.h` for the number of channels.
 */
CONST(Gpt_ConfigType, CONFIG_CONST) GptConfig =
{
#if (GPT_CONFIGURED_CHANNELS > 0U)
    .ChannelConfigSet    = Gpt_axChannelConfigurations, /* Pointer to channel configurations array */
#else
    .ChannelConfigSet    = NULL_PTR, /* No channels configured, provide NULL */
#endif
    .NumberOfChannels    = GPT_CONFIGURED_CHANNELS      /* Total number of configured channels from Gpt_Cfg.h */
};

#define GPT_STOP_SEC_CONFIG_DATA_UNSPECIFIED
/* #include "MemMap.h" */ /* Or Gpt_MemMap.h */
