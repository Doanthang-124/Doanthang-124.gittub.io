#ifndef GPT_CFG_H
#define GPT_CFG_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "Std_Types.h"      /* For STD_ON, STD_OFF definitions */
/* For Gpt_ChannelType - this is uint8. Gpt.h (where it's typedef'd) includes Gpt_Cfg.h,
   so Gpt_Cfg.h cannot include Gpt.h directly. Using (uint8) cast for channel ID macros is safe.
*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/**
 * @brief Instance ID for the GPT module.
 * @details Typically 0 for non-reentrant modules.
 */
#define GPT_INSTANCE_ID                       (0U)


/**
 * @brief Switch to enable or disable development error detection for the GPT module.
 */
#define GPT_DEV_ERROR_DETECT                  STD_ON

/**
 * @brief Switch to enable or disable the Gpt_GetVersionInfo API.
 */
#define GPT_VERSION_INFO_API                  STD_ON

/**
 * @brief Switch to enable or disable the Gpt_DeInit API.
 */
#define GPT_DEINIT_API                        STD_ON

/**
 * @brief Switch to enable or disable the Gpt_GetTimeElapsed API.
 */
#define GPT_TIME_ELAPSED_API                  STD_ON

/**
 * @brief Switch to enable or disable the Gpt_GetTimeRemaining API.
 */
#define GPT_TIME_REMAINING_API                STD_ON

/**
 * @brief Switch to enable or disable the Gpt_EnableNotification and Gpt_DisableNotification APIs.
 */
#define GPT_ENABLE_DISABLE_NOTIFICATION_API   STD_ON

/**
 * @brief Switch to enable or disable wakeup functionality related APIs.
 * @details Currently set to STD_OFF as this feature is not implemented in the basic Gpt driver.
 */
#define GPT_WAKEUP_FUNCTIONALITY_API          STD_OFF

/**
 * @brief Switch to enable or disable reporting of wakeup source by Gpt.
 * @details Currently set to STD_OFF. Requires GPT_WAKEUP_FUNCTIONALITY_API to be STD_ON.
 */
#define GPT_REPORT_WAKEUP_SOURCE              STD_OFF


/**
 * @brief Defines the number of GPT channels configured for this driver instance.
 * @details This value must match the number of channel configurations provided in Gpt_PBcfg.c.
 *          It is used for sizing internal arrays and loop bounds.
 */
#define GPT_CONFIGURED_CHANNELS               (2U)  /* Example: Configuring 2 GPT channels */


/*
 * Symbolic names for GPT Channels
 * These macros define unique identifiers for each logical GPT channel.
 * The values should be of type Gpt_ChannelType (typically uint8) and correspond
 * to the indices of the channel configurations in the Gpt_PBcfg.c array.
 */
#define GPT_CHANNEL_0                         ((uint8)0U)  /**< @brief Symbolic name for logical GPT Channel 0. */
#define GPT_CHANNEL_1                         ((uint8)1U)  /**< @brief Symbolic name for logical GPT Channel 1. */
/* Add more symbolic names if GPT_CONFIGURED_CHANNELS is increased, e.g.: */
/* #define GPT_CHANNEL_2                      ((uint8)2U) */


/* Example of more descriptive names (optional, if preferred over generic GPT_CHANNEL_x)
   These would still map to the 0-indexed logical channel IDs.
   The actual hardware timer (e.g., TIM2) mapping is done in Gpt_PBcfg.c.
*/
// #define GptConf_GptChannel_LOGICAL_TIMER_A   GPT_CHANNEL_0
// #define GptConf_GptChannel_LOGICAL_TIMER_B   GPT_CHANNEL_1


#endif /* GPT_CFG_H */
