#ifndef GPT_PBCFG_H
#define GPT_PBCFG_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "Std_Types.h"  /* Standard AUTOSAR Types (includes Platform_Types.h, Compiler.h) */
#include "Gpt.h"        /* For Gpt_ChannelType, Gpt_ValueType, Gpt_ModeType, Gpt_NotificationFuncPtrType,
                           and the Gpt_ConfigType structure declaration. Gpt.h includes Gpt_Cfg.h */

/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/
/* Software Version Numbers for this Post-Build configuration header */
#define GPT_PBCFG_SW_MAJOR_VERSION     1U
#define GPT_PBCFG_SW_MINOR_VERSION     0U
#define GPT_PBCFG_SW_PATCH_VERSION     0U

/* AUTOSAR Version Information for this Post-Build configuration header */
#define GPT_PBCFG_AR_RELEASE_MAJOR_VERSION    4U
#define GPT_PBCFG_AR_RELEASE_MINOR_VERSION    3U
#define GPT_PBCFG_AR_RELEASE_REVISION_VERSION 1U

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/**
 * @brief Implementation-specific type to identify the underlying STM32 hardware timer peripheral.
 * @details This enumeration lists the STM32 timers that can be configured as GPT channels.
 *          The actual availability depends on the specific STM32F3 microcontroller package.
 */
typedef enum
{
    GPT_HW_STM32_TIM2,     /**< STM32 General-purpose timer TIM2 (32-bit or 16-bit) */
    GPT_HW_STM32_TIM3,     /**< STM32 General-purpose timer TIM3 (16-bit) */
    GPT_HW_STM32_TIM4,     /**< STM32 General-purpose timer TIM4 (16-bit) */
    GPT_HW_STM32_TIM5,     /**< STM32 General-purpose timer TIM5 (32-bit or 16-bit, not on all F3) - Example */
    GPT_HW_STM32_TIM6,     /**< STM32 Basic timer TIM6 (16-bit) */
    GPT_HW_STM32_TIM7,     /**< STM32 Basic timer TIM7 (16-bit) */
    /* Add other timers like TIM1, TIM15, TIM16, TIM17 as needed and supported */
    GPT_HW_CHANNEL_NOT_USED /* Placeholder if a logical channel is not mapped to HW */
} Gpt_HwChannelType;


/**
 * @brief Structure defining the post-build configuration for a single GPT channel. (GptChannelConfiguration)
 * @details This structure contains all parameters necessary to configure one GPT channel.
 *          An array of this type, representing all configured channels, will be defined
 *          in Gpt_PBcfg.c.
 */
typedef struct Gpt_ChannelConfigType /* Name matches the forward declaration in Gpt.h */
{
    /** @brief Logical GPT Channel ID. Corresponds to symbolic names in Gpt_Cfg.h (e.g., GPT_CHANNEL_0). */
    const Gpt_ChannelType GptChannelId;

    /** @brief Identifies the underlying STM32 hardware timer peripheral (e.g., GPT_HW_STM32_TIM2). */
    const Gpt_HwChannelType HwTimer;

    /** @brief Channel mode: GPT_MODE_ONESHOT or GPT_MODE_CONTINUOUS. */
    const Gpt_ModeType GptChannelMode;

    /**
     * @brief Desired tick frequency in Hz for this channel (e.g., 1000000 for 1MHz -> 1us tick).
     * @details Gpt_Init will use this, along with the timer's input clock frequency (from a Clock module
     *          or fixed definition), to calculate the hardware prescaler value.
     */
    const uint32 GptChannelTickFrequencyHz;

    /**
     * @brief Maximum tick value for the channel (corresponds to Auto-Reload Register - ARR).
     * @details This value determines the timer period in continuous mode or the maximum
     *          one-shot duration at the configured GptChannelTickFrequencyHz.
     *          Example: For a 1MHz tick frequency, a MaxTickValue of 1000 means a period of 1ms.
     *          Must be within the hardware timer's counter capability (e.g., <= 0xFFFF for 16-bit timers).
     */
    const Gpt_ValueType GptChannelTickValueMax;

    /**
     * @brief Pointer to the notification function (callback) for this channel.
     * @details This function is called when the timer period expires (in continuous mode)
     *          or when the target time is reached (in one-shot mode), if notifications are enabled.
     *          Set to NULL_PTR if no notification is needed for this channel.
     */
    const Gpt_NotificationFuncPtrType GptNotification;

#if (GPT_WAKEUP_FUNCTIONALITY_API == STD_ON) /* From Gpt_Cfg.h */
    /** @brief TRUE if this channel is configured to be wakeup capable. FALSE otherwise. */
    const boolean GptEnableWakeup;
    /* TODO: Add EcuM_WakeupSourceType GptWakeupSourceId; if GptEnableWakeup is TRUE. Requires EcuM types. */
#endif

    /* Other implementation-specific parameters could be added here, e.g.: */
    /* - Specific hardware timer channel (if a timer has multiple compare channels used for GPT) */
    /* - DMA configuration if used with timers */
    /* - Pre-calculated prescaler value (if GptChannelTickFrequencyHz is not used for calculation) */

} Gpt_ChannelConfigType;


/*
 * The `Gpt_ConfigType` structure (main configuration structure for the driver)
 * is already declared in Gpt.h. It typically includes:
 *   P2CONST(struct Gpt_ChannelConfigType, AUTOMATIC, GPT_APPL_CONST) ChannelConfigSet;
 *   const Gpt_ChannelType NumberOfChannels;
 *
 * This Gpt_PBcfg.h file provides the full definition of `Gpt_ChannelConfigType`,
 * which is referenced by the `Gpt_ConfigType` in Gpt.h.
 */


/*==================================================================================================
*                                      EXTERNAL CONSTANTS
==================================================================================================*/

/**
 * @brief Declaration of the post-build configuration set for the GPT Driver.
 * @details The actual definition (the instance of Gpt_ConfigType and its contained arrays)
 *          will be provided in Gpt_PBcfg.c. This is the structure that will be
 *          passed to Gpt_Init().
 */
#define GPT_START_SEC_CONFIG_DATA_UNSPECIFIED /* For memory mapping via MemMap.h */
/* #include "MemMap.h" This should be Bsw_MemMap.h or Gpt_MemMap.h as per AUTOSAR */

extern CONST(Gpt_ConfigType, GPT_CONFIG_CONST) Gpt_ConfigRoot[]; /* Standard is array for multiple config sets */
/* For a single config set, often just: extern CONST(Gpt_ConfigType, GPT_CONFIG_CONST) GptConfig; */
/* Let's assume a single configuration set for simplicity for now as GptConfig */
/* The name GptConfig should match the one in Gpt_PBcfg.c */
/* Using Gpt_ConfigRoot to align with potential for multiple configurations, but will define one named GptConfig */
/* For now, let's use a simpler name that implies a single config set, e.g. GptDriverConfig */
/* As per plan, `GptConfig` was used. */

extern CONST(Gpt_ConfigType, CONFIG_CONST) GptConfig; /* Using CONFIG_CONST from Compiler.h for the section */


#define GPT_STOP_SEC_CONFIG_DATA_UNSPECIFIED
/* #include "MemMap.h" */


#endif /* GPT_PBCFG_H */
