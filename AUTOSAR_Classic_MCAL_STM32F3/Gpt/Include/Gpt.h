#ifndef GPT_H
#define GPT_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "../../Common/Include/Std_Types.h" /* Standard AUTOSAR Types (includes Compiler.h, Platform_Types.h) */
#include "../../Config/Gpt_Cfg.h"       /* GPT Driver Pre-Compile Configuration (GPT_DEV_ERROR_DETECT, etc.) */
#include "../../Config/Gpt_PBcfg.h"     /* For Gpt_ConfigType, Gpt_ChannelConfigType, and fundamental GPT types
                                           (Gpt_ChannelType, Gpt_ValueType, Gpt_ModeType, Gpt_NotificationFuncPtrType) */

/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

/**
 * @brief Module ID for the General Purpose Timer (GPT) module.
 * @details As per AUTOSAR_SWS_StandardTypes. GPT SWS usually assigns 100.
 */
#define GPT_MODULE_ID                     100U

/**
 * @brief Vendor ID for this GPT driver implementation.
 * @details Replace with actual registered AUTOSAR Vendor ID.
 */
#define GPT_VENDOR_ID                     0U   /* Example: To be replaced by actual Vendor ID */

/** @brief Software major version of this GPT driver. */
#define GPT_SW_MAJOR_VERSION              1U
/** @brief Software minor version of this GPT driver. */
#define GPT_SW_MINOR_VERSION              0U
/** @brief Software patch version of this GPT driver. */
#define GPT_SW_PATCH_VERSION              0U

/** @brief AUTOSAR specification major version compatibility. */
#define GPT_AR_RELEASE_MAJOR_VERSION      4U
/** @brief AUTOSAR specification minor version compatibility. */
#define GPT_AR_RELEASE_MINOR_VERSION      3U
/** @brief AUTOSAR specification patch version compatibility. */
#define GPT_AR_RELEASE_PATCH_VERSION      1U


/* API Service IDs for Development Error Reporting (DET) */
#define GPT_INIT_API_ID                       0x01U
#define GPT_DEINIT_API_ID                     0x02U
#define GPT_GET_TIME_ELAPSED_API_ID           0x03U
#define GPT_GET_TIME_REMAINING_API_ID         0x04U
#define GPT_START_TIMER_API_ID                0x05U
#define GPT_STOP_TIMER_API_ID                 0x06U
#define GPT_ENABLE_NOTIFICATION_API_ID        0x07U
#define GPT_DISABLE_NOTIFICATION_API_ID       0x08U
/* #define GPT_SET_MODE_API_ID                0x09U  // Not implemented in this basic version */
#define GPT_GET_VERSION_INFO_API_ID           0x0AU
/* #define GPT_WAKEUP_API_ID (if specific API for wakeup) */


/* Development Error Codes reported to DET (if GPT_DEV_ERROR_DETECT is STD_ON from Gpt_Cfg.h) */
#if (GPT_DEV_ERROR_DETECT == STD_ON)
    #define GPT_E_UNINIT                    0x0AU  /**< API service used without module initialization. */
    #define GPT_E_ALREADY_INITIALIZED       0x0BU  /**< Gpt_Init function called when the driver is already initialized. */
    #define GPT_E_PARAM_CHANNEL             0x0CU  /**< API service called with an invalid channel ID. */
    #define GPT_E_PARAM_VALUE               0x0DU  /**< API service called with an invalid value (e.g., 0 for target time). */
    #define GPT_E_PARAM_POINTER             0x0EU  /**< API service called with a NULL pointer parameter. */
    #define GPT_E_PARAM_MODE                0x0FU  /**< API Gpt_SetMode called with an invalid mode (Not used in basic version). */
    #define GPT_E_INVALID_CALL              0x10U  /**< API service called in an invalid channel state (e.g., Gpt_StartTimer on an already running timer). */
    #define GPT_E_WRONG_STATE               GPT_E_INVALID_CALL /* Alias for more general invalid state */
#endif /* (GPT_DEV_ERROR_DETECT == STD_ON) */


/*==================================================================================================
*                                             TYPES
==================================================================================================*/

/*
 * Core GPT types like Gpt_ChannelType, Gpt_ValueType, Gpt_ModeType, Gpt_NotificationFuncPtrType,
 * and the main configuration structure Gpt_ConfigType (which uses Gpt_ChannelConfigType)
 * are now defined in Gpt_PBcfg.h (included above).
 * This makes Gpt_PBcfg.h the source of truth for these types and ensures consistency,
 * especially for post-build configurations.
 */


/*==================================================================================================
*                                  FUNCTION PROTOTYPES
==================================================================================================*/

/**
 * @brief Initializes the GPT Driver module. (AUTOSAR SWS Gpt_00006)
 * @param[in] ConfigPtr Pointer to the post-build configuration set.
 *                      (Type `Gpt_ConfigType` is defined in `Gpt_PBcfg.h`)
 */
extern FUNC(void, MCU_CODE) Gpt_Init( /* Changed from GPT_CODE to MCU_CODE for consistency */
    P2CONST(Gpt_ConfigType, AUTOMATIC, GPT_APPL_CONST) ConfigPtr
);

#if (GPT_DEINIT_API == STD_ON) /* From Gpt_Cfg.h */
/**
 * @brief De-initializes the GPT Driver module. (AUTOSAR SWS Gpt_00007)
 */
extern FUNC(void, MCU_CODE) Gpt_DeInit(void);
#endif /* (GPT_DEINIT_API == STD_ON) */

#if (GPT_VERSION_INFO_API == STD_ON) /* From Gpt_Cfg.h */
/**
 * @brief Returns the version information of this module. (AUTOSAR SWS Gpt_00008)
 * @param[out] VersionInfoPtr Pointer to where to store the version information. Must not be NULL.
 */
extern FUNC(void, MCU_CODE) Gpt_GetVersionInfo(
    P2VAR(Std_VersionInfoType, AUTOMATIC, GPT_APPL_DATA) VersionInfoPtr
);
#endif /* (GPT_VERSION_INFO_API == STD_ON) */

#if (GPT_TIME_ELAPSED_API == STD_ON) /* From Gpt_Cfg.h */
/**
 * @brief Returns the time already elapsed for a specified GPT channel. (AUTOSAR SWS Gpt_00009)
 * @param[in] Channel GPT Channel ID (Type Gpt_ChannelType is from Gpt_PBcfg.h).
 * @return Gpt_ValueType Elapsed time in ticks. Returns 0 if channel is invalid or not started.
 *                     (Type Gpt_ValueType is from Gpt_PBcfg.h).
 */
extern FUNC(Gpt_ValueType, MCU_CODE) Gpt_GetTimeElapsed(
    VAR(Gpt_ChannelType, AUTOMATIC) Channel
);
#endif /* (GPT_TIME_ELAPSED_API == STD_ON) */

#if (GPT_TIME_REMAINING_API == STD_ON) /* From Gpt_Cfg.h */
/**
 * @brief Returns the time remaining until the target time is reached for a specified GPT channel. (AUTOSAR SWS Gpt_00010)
 * @param[in] Channel GPT Channel ID.
 * @return Gpt_ValueType Remaining time in ticks. Returns 0 if channel is invalid, not running, or target reached.
 */
extern FUNC(Gpt_ValueType, MCU_CODE) Gpt_GetTimeRemaining(
    VAR(Gpt_ChannelType, AUTOMATIC) Channel
);
#endif /* (GPT_TIME_REMAINING_API == STD_ON) */

/**
 * @brief Starts a specified GPT channel with a given target time (value in ticks). (AUTOSAR SWS Gpt_00011)
 * @param[in] Channel GPT Channel ID.
 * @param[in] Value   Target time in ticks. Timer will count from 0 up to this value.
 *                    Must be > 0 and <= channel's MaxTickValue from configuration.
 */
extern FUNC(void, MCU_CODE) Gpt_StartTimer(
    VAR(Gpt_ChannelType, AUTOMATIC) Channel,
    VAR(Gpt_ValueType, AUTOMATIC) Value
);

/**
 * @brief Stops a specified GPT channel. (AUTOSAR SWS Gpt_00012)
 * @param[in] Channel GPT Channel ID.
 */
extern FUNC(void, MCU_CODE) Gpt_StopTimer(
    VAR(Gpt_ChannelType, AUTOMATIC) Channel
);

#if (GPT_ENABLE_DISABLE_NOTIFICATION_API == STD_ON) /* From Gpt_Cfg.h */
/**
 * @brief Enables the interrupt notification for a specified GPT channel. (AUTOSAR SWS Gpt_00013)
 * @details The notification function (callback) is configured in `Gpt_PBcfg.c`.
 * @param[in] Channel GPT Channel ID.
 */
extern FUNC(void, MCU_CODE) Gpt_EnableNotification(
    VAR(Gpt_ChannelType, AUTOMATIC) Channel
);

/**
 * @brief Disables the interrupt notification for a specified GPT channel. (AUTOSAR SWS Gpt_00014)
 * @param[in] Channel GPT Channel ID.
 */
extern FUNC(void, MCU_CODE) Gpt_DisableNotification(
    VAR(Gpt_ChannelType, AUTOMATIC) Channel
);
#endif /* GPT_ENABLE_DISABLE_NOTIFICATION_API */


#endif /* GPT_H */
