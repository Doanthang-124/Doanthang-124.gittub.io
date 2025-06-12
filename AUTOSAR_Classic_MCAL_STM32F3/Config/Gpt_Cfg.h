#ifndef GPT_CFG_H
#define GPT_CFG_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "Std_Types.h" /* For STD_ON/STD_OFF definitions. Std_Types.h includes Compiler.h and Platform_Types.h */

/*==================================================================================================
*                                       PRE-COMPILE OPTIONS
==================================================================================================*/

/**
 * @brief Switch to enable/disable development error detection for the GPT driver.
 * @details If STD_ON, development errors are reported to DET.
 * @type {STD_ON, STD_OFF}
 */
#define GPT_DEV_ERROR_DETECT                  STD_ON

/**
 * @brief Switch to enable/disable the Gpt_GetVersionInfo API.
 * @type {STD_ON, STD_OFF}
 */
#define GPT_VERSION_INFO_API                  STD_ON

/**
 * @brief Switch to enable/disable the Gpt_DeInit API.
 * @type {STD_ON, STD_OFF}
 */
#define GPT_DEINIT_API                        STD_ON

/**
 * @brief Switch to enable/disable the Gpt_GetTimeElapsed API.
 * @type {STD_ON, STD_OFF}
 */
#define GPT_TIME_ELAPSED_API                  STD_ON

/**
 * @brief Switch to enable/disable the Gpt_GetTimeRemaining API.
 * @type {STD_ON, STD_OFF}
 */
#define GPT_TIME_REMAINING_API                STD_ON

/**
 * @brief Switch to enable/disable Gpt_EnableNotification and Gpt_DisableNotification APIs.
 * @type {STD_ON, STD_OFF}
 */
#define GPT_ENABLE_DISABLE_NOTIFICATION_API   STD_ON

/**
 * @brief Switch to enable/disable GPT wakeup functionality APIs.
 * @details Currently not supported in this basic implementation.
 * @type {STD_ON, STD_OFF}
 */
#define GPT_WAKEUP_FUNCTIONALITY_API          STD_OFF

/**
 * @brief Switch to enable/disable reporting of wakeup source by GPT.
 * @details Currently not supported. Requires GPT_WAKEUP_FUNCTIONALITY_API to be STD_ON.
 * @type {STD_ON, STD_OFF}
 */
#define GPT_REPORT_WAKEUP_SOURCE              STD_OFF


/*==================================================================================================
*                                     CHANNEL CONFIGURATION
==================================================================================================*/

/**
 * @brief Defines the number of GPT channels configured for use.
 * @details This value determines the size of channel-related arrays in the
 *          post-build configuration (Gpt_PBcfg.c).
 */
#define GPT_CONFIGURED_CHANNELS               (2U) /* Example: 2 logical GPT channels */


/**
 * @brief Symbolic names for the configured GPT channels.
 * @details These names are used as input to GPT API functions like Gpt_StartTimer, Gpt_StopTimer, etc.
 *          They serve as indices or identifiers for the logical channels defined in the
 *          post-build configuration (Gpt_PBcfg.c).
 *          The type Gpt_ChannelType is defined in Gpt.h (typically uint8).
 */
#define GPT_CHANNEL_0     ((uint8)0)  /* Logical Channel 0 (e.g., could be mapped to TIM2) */
#define GPT_CHANNEL_1     ((uint8)1)  /* Logical Channel 1 (e.g., could be mapped to TIM3) */
/*
 * If GPT_CONFIGURED_CHANNELS is increased, add more symbolic names here:
 * #define GPT_CHANNEL_2     ((uint8)2)
 * ...
 */

/*
 * Example of more descriptive symbolic names (optional, can be defined by user/integrator).
 * These would typically map to the generic ones above or be used directly if preferred.
 * The mapping to actual hardware timers (like TIM2, TIM3) is done in the
 * post-build configuration (Gpt_PBcfg.c).
 *
 * #define GptConf_GptChannel_TIM2_CH1   GPT_CHANNEL_0
 * #define GptConf_GptChannel_TIM3_CH1   GPT_CHANNEL_1
 */


/*==================================================================================================
*                                      OTHER CONFIGURATIONS
==================================================================================================*/

/*
 * Example: Pre-compile definition for a fixed prescaler or clock source if not
 * handled entirely by post-build configuration or by a clock module.
 * This is generally not recommended for flexibility if these parameters can vary.
 */
// #define GPT_DEFAULT_PRESCALER_VALUE         (71U) /* For a 72MHz clock to get 1us tick, Prescaler = (72MHz / 1MHz) - 1 = 71 */


#endif /* GPT_CFG_H */
