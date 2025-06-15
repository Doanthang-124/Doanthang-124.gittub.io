#ifndef GPT_PBCFG_H
#define GPT_PBCFG_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "../Common/Include/Std_Types.h" /* For Std_ReturnType, uint8, uint16, boolean, P2CONST, CONST etc. */
                                         /* Std_Types.h includes Compiler.h */
/* Do NOT include Gpt.h here as Gpt.h includes this file (for Gpt_ConfigType definition),
   which would create a circular dependency.
   Fundamental GPT types needed by Gpt_ChannelConfigType and Gpt_ConfigType are defined below.
*/

/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/
/* Software Version Numbers for this Post-Build configuration header */
#define GPT_PBCFG_SW_MAJOR_VERSION       1U
#define GPT_PBCFG_SW_MINOR_VERSION       0U
#define GPT_PBCFG_SW_PATCH_VERSION       0U

/* AUTOSAR Version Information for this Post-Build configuration header */
#define GPT_PBCFG_AR_RELEASE_MAJOR_VERSION      4U
#define GPT_PBCFG_AR_RELEASE_MINOR_VERSION      3U
#define GPT_PBCFG_AR_RELEASE_REVISION_VERSION   1U

/*==================================================================================================
*                                             TYPES
==================================================================================================*/

/*
 * Fundamental GPT Driver Types
 * These types are defined here to be included by Gpt.h (which needs them for its API)
 * and Gpt_PBcfg.c (for defining configurations).
 * This avoids circular dependencies if Gpt.h includes Gpt_PBcfg.h for Gpt_ConfigType.
 */

/** @brief Numeric ID for a GPT channel. Typically uint8. (AUTOSAR SWS GptChannelId) */
typedef uint8 Gpt_ChannelType;

/** @brief Type for timer value (target ticks, elapsed ticks). Typically uint16 or uint32. (AUTOSAR SWS GptChannelTickValueType) */
typedef uint32 Gpt_ValueType; /* Using uint32 for compatibility with 32-bit timers like TIM2 */

/** @brief Type for defining the Gpt channel operational mode. (AUTOSAR SWS GptChannelModeType) */
typedef enum
{
    GPT_MODE_ONESHOT = 0U,   /**< Timer runs once and stops after reaching target time. */
    GPT_MODE_CONTINUOUS      /**< Timer restarts automatically after reaching target time. */
} Gpt_ModeType;

/** @brief Type for the function pointer used for channel notifications (callbacks). (AUTOSAR SWS Gpt_NotificationType) */
typedef void (*Gpt_NotificationFuncPtrType)(void);


/**
 * @brief Implementation-specific type to identify the STM32 hardware timer peripheral
 *        used for a GPT channel (e.g., TIM2, TIM3, etc.).
 */
typedef enum
{
    STM32_TIM2 = 0U, /**< STM32 General-Purpose Timer 2 (32-bit or 16-bit capable) */
    STM32_TIM3,      /**< STM32 General-Purpose Timer 3 (16-bit) */
    STM32_TIM4,      /**< STM32 General-Purpose Timer 4 (16-bit) */
    STM32_TIM6,      /**< STM32 Basic Timer 6 (16-bit) */
    STM32_TIM7,      /**< STM32 Basic Timer 7 (16-bit) */
    /* Add other timers as supported by the driver and target MCU, e.g.: */
    /* STM32_TIM1, STM32_TIM15, STM32_TIM16, STM32_TIM17 */
    STM32_GPT_MAX_HW_TIMERS /**< Helper for array sizing or validation, not a usable timer ID. */
} Gpt_Hw_ChannelType; /* Corrected name to Gpt_Hw_ChannelType as used in Gpt_PBcfg.c earlier */


/**
 * @brief Structure defining the post-build configuration for a single GPT channel. (AUTOSAR SWS GptChannelConfiguration)
 * @details This structure is instantiated in an array in `Gpt_PBcfg.c` for each configured channel,
 *          providing all necessary settings for `Gpt_Init` to configure the timer channel.
 */
typedef struct Gpt_ChannelConfigTypeTag /* Using Tag for MISRA C:2012 Rule 5.6 */
{
    const Gpt_ChannelType             GptChannelId;           /**< @brief Logical GPT Channel ID (e.g., GPT_CHANNEL_0 from Gpt_Cfg.h). */
    const Gpt_Hw_ChannelType          HwTimer;                /**< @brief The underlying STM32 hardware timer (e.g., STM32_TIM2). Renamed from HwTimerModule */
    const Gpt_ModeType                GptChannelMode;         /**< @brief Channel operational mode: GPT_MODE_ONESHOT or GPT_MODE_CONTINUOUS. */
    const uint32                      GptChannelTickFrequencyHz;/**< @brief Desired tick frequency in Hz for this channel (e.g., 1000000 for 1MHz -> 1us tick). */
    const Gpt_ValueType               GptChannelTickValueMax; /**< @brief Maximum tick value the timer channel can count to (usually ARR value). */
    const Gpt_NotificationFuncPtrType GptNotification;        /**< @brief Pointer to the notification function (callback). NULL_PTR if no notification. */

#if (defined(GPT_WAKEUP_FUNCTIONALITY_API) && (GPT_WAKEUP_FUNCTIONALITY_API == STD_ON)) /* Check Gpt_Cfg.h */
    const boolean                     GptEnableWakeup;        /**< @brief TRUE if this channel is configured as wakeup capable. */
    /* const EcuM_WakeupSourceType    GptWakeupSourceId; */     /**< @brief EcuM Wakeup Source ID if wakeup capable. (Requires EcuM_Types.h) */
#endif
} Gpt_ChannelConfigType;


/**
 * @brief Main Post-Build Configuration Structure for the GPT Driver. (AUTOSAR SWS GptConfigType)
 * @details This structure contains a pointer to an array of `Gpt_ChannelConfigType` elements,
 *          defining the configuration for all GPT channels managed by this driver instance.
 *          An instance of this structure (e.g., `GptConfig`) is defined in `Gpt_PBcfg.c`
 *          and passed to `Gpt_Init()`.
 */
typedef struct Gpt_ConfigTypeTag /* Using Tag for MISRA C:2012 Rule 5.6 */
{
    /** @brief Pointer to the array of individual GPT channel configurations. */
    P2CONST(Gpt_ChannelConfigType, AUTOMATIC, CONFIG_CONST) ChannelConfigSet; /* Changed from ChannelConfig, CONFIG_CONST from Compiler.h */

    /** @brief Total number of GPT channels configured in the `ChannelConfigSet` array.
     *  @details This should match `GPT_CONFIGURED_CHANNELS` from `Gpt_Cfg.h`.
     */
    Gpt_ChannelType NumberOfChannels;
} Gpt_ConfigType;


/*==================================================================================================
*                                      EXTERNAL CONSTANTS
==================================================================================================*/

/**
 * @brief Declaration of the post-build configuration set for the GPT Driver.
 * @details The actual definition (the instance `GptConfig`) is in `Gpt_PBcfg.c`.
 *          This uses the `CONFIG_CONST` memory class from `Compiler.h`.
 */
extern CONST(Gpt_ConfigType, CONFIG_CONST) GptConfig;


#endif /* GPT_PBCFG_H */
