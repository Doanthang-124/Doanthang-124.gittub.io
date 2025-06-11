#ifndef GPT_H
#define GPT_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "Std_Types.h"      /* Standard AUTOSAR Types (includes Compiler.h, Platform_Types.h) */
#include "Gpt_Cfg.h"        /* GPT Driver Pre-Compile Configuration (to be created) */

/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

/* AUTOSAR Module and Vendor IDs */
#define GPT_MODULE_ID                     100U
#define GPT_VENDOR_ID                     0U   /* Example Vendor ID, replace with actual */

/* AUTOSAR Instance ID */
#define GPT_INSTANCE_ID                   0U

/* Development Error Codes (if GPT_DEV_ERROR_DETECT from Gpt_Cfg.h is STD_ON) */
/* The check for GPT_DEV_ERROR_DETECT will be handled by Gpt_Cfg.h */
#define GPT_E_UNINIT                    0x0AU  /* API service used without module initialization */
#define GPT_E_ALREADY_INITIALIZED       0x0BU  /* Gpt_Init called when already initialized */
#define GPT_E_PARAM_CHANNEL             0x0CU  /* Invalid GPT channel ID requested */
#define GPT_E_PARAM_VALUE               0x0DU  /* Invalid timer value (e.g., 0 for target time) */
#define GPT_E_PARAM_POINTER             0x0EU  /* API service called with a NULL pointer */
#define GPT_E_PARAM_MODE                0x0FU  /* Mode parameter is invalid (if Gpt_SetMode is implemented) */
#define GPT_E_INVALID_CALL              0x10U  /* Function called in an invalid state */
#define GPT_E_CHANNEL_UNINITIALIZED     0x11U  /* Channel not configured or Gpt_Init not called */
#define GPT_E_WRONG_STATE               0x12U  /* API called in wrong channel state (e.g. StopTimer on non-running timer) */


/* API Service IDs (for error reporting via Det_ReportError) */
#define GPT_INIT_ID                       0x01U
#define GPT_DEINIT_ID                     0x02U
#define GPT_GET_TIME_ELAPSED_ID           0x03U
#define GPT_GET_TIME_REMAINING_ID         0x04U
#define GPT_START_TIMER_ID                0x05U
#define GPT_STOP_TIMER_ID                 0x06U
#define GPT_ENABLE_NOTIFICATION_ID        0x07U
#define GPT_DISABLE_NOTIFICATION_ID       0x08U
#define GPT_SET_MODE_ID                   0x09U  /* Example SID, even if not implemented */
#define GPT_GET_VERSION_INFO_ID           0x0AU


/*==================================================================================================
*                                             TYPES
==================================================================================================*/

/** @brief Numeric ID for a GPT channel. Based on GptChannelId in SWS. */
typedef uint8 Gpt_ChannelType;

/** @brief Type for timer value (target ticks, elapsed ticks). Based on GptChannelTickValueType in SWS. */
typedef uint32 Gpt_ValueType;

/** @brief Type for defining the Gpt channel mode. Based on GptChannelModeType in SWS. */
typedef enum
{
    GPT_MODE_ONESHOT,   /**< Timer runs once and stops after reaching target time. */
    GPT_MODE_CONTINUOUS /**< Timer restarts automatically after reaching target time. */
} Gpt_ModeType;

/** @brief Type for the function pointer used for channel notifications (callbacks). Based on Gpt_NotificationType in SWS. */
typedef void (*Gpt_NotificationFuncPtrType)(void);


/**
 * @brief Forward declaration for the GPT driver's channel-specific configuration structure.
 * @details This structure will be fully defined in Gpt_PBcfg.h for post-build configurations.
 */
struct Gpt_ChannelConfigType; /* AUTOSAR: GptChannelConfiguration */

/**
 * @brief Main configuration structure for the GPT Driver. Based on Gpt_ConfigType in SWS.
 * @details This structure is used to pass configuration data to the Gpt_Init function.
 *          For post-build configurations, an instance of this type (defined in Gpt_PBcfg.h)
 *          is passed to Gpt_Init.
 */
typedef struct
{
    /** @brief Pointer to the array of individual GPT channel configurations. */
    P2CONST(struct Gpt_ChannelConfigType, AUTOMATIC, GPT_APPL_CONST) ChannelConfigSet; /* Changed name for clarity */

    /** @brief Number of GPT channels configured in the ChannelConfigSet. */
    const Gpt_ChannelType NumberOfChannels;

    /* Other global GPT driver settings can be added here if needed, */
    /* e.g., a pointer to a clock configuration structure if GPT driver handles clock setup. */
    /* uint32 GptClockFrequency; // Example, if needed */
} Gpt_ConfigType;


/*==================================================================================================
*                                  FUNCTION PROTOTYPES
==================================================================================================*/

/**
 * @brief Initializes the GPT Driver module. (SWS_Gpt_00006)
 * @param[in] ConfigPtr Pointer to the configuration set (post-build).
 */
extern FUNC(void, MCU_CODE) Gpt_Init(P2CONST(Gpt_ConfigType, AUTOMATIC, GPT_APPL_CONST) ConfigPtr);

/**
 * @brief De-initializes the GPT Driver module. (SWS_Gpt_00007)
 */
extern FUNC(void, MCU_CODE) Gpt_DeInit(void);

#if (GPT_VERSION_INFO_API == STD_ON) /* Controlled by Gpt_Cfg.h */
/**
 * @brief Returns the version information of this module. (SWS_Gpt_00008)
 * @param[out] VersionInfoPtr Pointer to where to store the version information.
 */
extern FUNC(void, MCU_CODE) Gpt_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, GPT_APPL_DATA) VersionInfoPtr);
#endif

/**
 * @brief Returns the time already elapsed for a specified GPT channel. (SWS_Gpt_00009)
 * @param[in] Channel GPT Channel ID.
 * @return Gpt_ValueType Elapsed time in ticks.
 */
extern FUNC(Gpt_ValueType, MCU_CODE) Gpt_GetTimeElapsed(Gpt_ChannelType Channel);

/**
 * @brief Returns the time remaining until the target time is reached for a specified GPT channel. (SWS_Gpt_00010)
 * @param[in] Channel GPT Channel ID.
 * @return Gpt_ValueType Remaining time in ticks.
 */
extern FUNC(Gpt_ValueType, MCU_CODE) Gpt_GetTimeRemaining(Gpt_ChannelType Channel);

/**
 * @brief Starts a specified GPT channel with a given target time. (SWS_Gpt_00011)
 * @param[in] Channel GPT Channel ID.
 * @param[in] Value Target time in ticks.
 */
extern FUNC(void, MCU_CODE) Gpt_StartTimer(Gpt_ChannelType Channel, Gpt_ValueType Value);

/**
 * @brief Stops a specified GPT channel. (SWS_Gpt_00012)
 * @param[in] Channel GPT Channel ID.
 */
extern FUNC(void, MCU_CODE) Gpt_StopTimer(Gpt_ChannelType Channel);

#if (GPT_ENABLE_DISABLE_NOTIFICATION_API == STD_ON) /* Controlled by Gpt_Cfg.h */
/**
 * @brief Enables the interrupt notification for a specified GPT channel. (SWS_Gpt_00013)
 * @param[in] Channel GPT Channel ID.
 */
extern FUNC(void, MCU_CODE) Gpt_EnableNotification(Gpt_ChannelType Channel);

/**
 * @brief Disables the interrupt notification for a specified GPT channel. (SWS_Gpt_00014)
 * @param[in] Channel GPT Channel ID.
 */
extern FUNC(void, MCU_CODE) Gpt_DisableNotification(Gpt_ChannelType Channel);
#endif /* GPT_ENABLE_DISABLE_NOTIFICATION_API */


#endif /* GPT_H */
