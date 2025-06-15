#ifndef DIO_H
#define DIO_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "../../Common/Include/Std_Types.h"  /* Standard AUTOSAR Types */
#include "../../Config/Dio_Cfg.h"        /* DIO Pre-Compile Configuration and Symbolic Names */
#include "../../Config/Dio_PBcfg.h"      /* For Dio_ConfigType, Dio_ChannelGroupType (Post-Build Config) */
                                         /* This makes Dio_ConfigType and Dio_ChannelGroupType fully visible */

/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

/**
 * @brief Module ID for the Digital Input/Output module.
 * @details As per AUTOSAR_SWS_StandardTypes.
 */
#define DIO_MODULE_ID                     120U /* Standard AUTOSAR Module ID for DIO */

/**
 * @brief Vendor ID for this DIO driver implementation.
 * @details Replace with actual registered AUTOSAR Vendor ID.
 */
#define DIO_VENDOR_ID                     0U   /* Example: To be replaced by actual Vendor ID */
/* #define DIO_VENDOR_ID_MYCOMPANY         99U // Example */

/** @brief Software major version of this DIO driver. */
#define DIO_SW_MAJOR_VERSION              1U
/** @brief Software minor version of this DIO driver. */
#define DIO_SW_MINOR_VERSION              0U
/** @brief Software patch version of this DIO driver. */
#define DIO_SW_PATCH_VERSION              0U

/** @brief AUTOSAR specification major version compatibility. */
#define DIO_AR_RELEASE_MAJOR_VERSION      4U
/** @brief AUTOSAR specification minor version compatibility. */
#define DIO_AR_RELEASE_MINOR_VERSION      3U
/** @brief AUTOSAR specification patch version compatibility. */
#define DIO_AR_RELEASE_PATCH_VERSION      1U


/* API Service IDs for Development Error Reporting (DET) */
#define DIO_INIT_API_ID                       0x10U
#define DIO_READ_CHANNEL_API_ID               0x01U
#define DIO_WRITE_CHANNEL_API_ID              0x02U
#define DIO_READ_PORT_API_ID                  0x03U
#define DIO_WRITE_PORT_API_ID                 0x04U
#define DIO_READ_CHANNEL_GROUP_API_ID         0x05U
#define DIO_WRITE_CHANNEL_GROUP_API_ID        0x06U
#define DIO_GET_VERSION_INFO_API_ID           0x12U
#define DIO_FLIP_CHANNEL_API_ID               0x11U
#define DIO_MASKED_WRITE_PORT_API_ID          0x13U /* If API is supported */


/* Development Error Codes reported to DET */
#if (DIO_DEV_ERROR_DETECT == STD_ON) /* From Dio_Cfg.h */
    #define DIO_E_PARAM_INVALID_CHANNEL_ID    0x0AU  /**< Invalid channel ID requested (AUTOSAR SWS Ref: DIO050) */
    #define DIO_E_PARAM_INVALID_PORT_ID       0x14U  /**< Invalid port ID requested (AUTOSAR SWS Ref: DIO051) */
    #define DIO_E_PARAM_INVALID_GROUP_ID      0x1FU  /**< Invalid channel group requested (AUTOSAR SWS Ref: DIO052) */
    #define DIO_E_PARAM_POINTER               0x20U  /**< API service called with a NULL pointer (AUTOSAR SWS Ref: DIO112) */
    #define DIO_E_UNINIT                      0x30U  /**< API service called before module initialization (Custom error, common practice) */
    #define DIO_E_ALREADY_INITIALIZED         0x40U  /**< Dio_Init called when already initialized (Custom error, common practice) */
    #define DIO_E_PARAM_INVALID_LEVEL         0x50U  /**< Invalid level parameter (Custom error, for Dio_WriteChannel) */
#endif /* (DIO_DEV_ERROR_DETECT == STD_ON) */


/*==================================================================================================
*                                             TYPES
==================================================================================================*/

/*
 * Fundamental DIO types such as:
 * - Dio_ChannelType (uint8)
 * - Dio_PortType (uint8)
 * - Dio_LevelType (uint8, for STD_HIGH/STD_LOW)
 * - Dio_PortLevelType (uint16)
 * are now defined in "Dio_PBcfg.h" (which is included by this file via its own include of Dio_PBcfg.h).
 * This makes Dio_PBcfg.h the master source for these types and also for the
 * post-build configuration structures like Dio_ChannelGroupType and Dio_ConfigType,
 * resolving potential circular dependencies and ensuring type consistency.
 *
 * Dio.h itself includes Dio_PBcfg.h, making these types available.
 */


/*==================================================================================================
*                                  FUNCTION PROTOTYPES
==================================================================================================*/

/**
 * @brief Initializes the DIO module. (AUTOSAR SWS Ref: DIO099)
 * @details Sets up the DIO module based on the provided configuration.
 *          This function must be called before any other DIO API service.
 * @param[in] ConfigPtr Pointer to the post-build configuration structure for the DIO module.
 *                      (Type Dio_ConfigType is defined in Dio_PBcfg.h)
 */
extern FUNC(void, DIO_CODE) Dio_Init(
    P2CONST(Dio_ConfigType, AUTOMATIC, DIO_APPL_CONST) ConfigPtr
);

#if (DIO_FLIP_CHANNEL_API == STD_ON) /* From Dio_Cfg.h */
/**
 * @brief Returns the inverted level of a specified DIO channel and writes it to the channel. (AUTOSAR SWS Ref: DIO113)
 * @param[in] ChannelId ID of DIO channel to be flipped.
 * @return Dio_LevelType The level of the channel after flipping (STD_HIGH or STD_LOW).
 */
extern FUNC(Dio_LevelType, DIO_CODE) Dio_FlipChannel(
    VAR(Dio_ChannelType, AUTOMATIC) ChannelId
);
#endif /* DIO_FLIP_CHANNEL_API */

/**
 * @brief Reads the level of a specified DIO channel. (AUTOSAR SWS Ref: DIO049)
 * @param[in] ChannelId ID of DIO channel to read.
 * @return Dio_LevelType Level of the channel (STD_HIGH or STD_LOW).
 */
extern FUNC(Dio_LevelType, DIO_CODE) Dio_ReadChannel(
    VAR(Dio_ChannelType, AUTOMATIC) ChannelId
);

/**
 * @brief Writes a specified level to a DIO channel. (AUTOSAR SWS Ref: DIO056)
 * @param[in] ChannelId ID of DIO channel to write.
 * @param[in] Level     Level to be written (STD_HIGH or STD_LOW).
 */
extern FUNC(void, DIO_CODE) Dio_WriteChannel(
    VAR(Dio_ChannelType, AUTOMATIC) ChannelId,
    VAR(Dio_LevelType, AUTOMATIC) Level
);

/**
 * @brief Reads the levels of all channels in a specified DIO port. (AUTOSAR SWS Ref: DIO050)
 * @param[in] PortId ID of DIO port to read.
 * @return Dio_PortLevelType Levels of all channels in the port.
 */
extern FUNC(Dio_PortLevelType, DIO_CODE) Dio_ReadPort(
    VAR(Dio_PortType, AUTOMATIC) PortId
);

/**
 * @brief Writes a specified level to all channels in a DIO port. (AUTOSAR SWS Ref: DIO057)
 * @param[in] PortId ID of DIO port to write.
 * @param[in] Level  Levels to be written to the port.
 */
extern FUNC(void, DIO_CODE) Dio_WritePort(
    VAR(Dio_PortType, AUTOMATIC) PortId,
    VAR(Dio_PortLevelType, AUTOMATIC) Level
);

/**
 * @brief Reads the levels of a specified group of DIO channels. (AUTOSAR SWS Ref: DIO052)
 * @param[in] ChannelGroupIdPtr Pointer to the channel group definition.
 *                              (Type Dio_ChannelGroupType is defined in Dio_PBcfg.h)
 * @return Dio_PortLevelType Levels of the channels in the specified group.
 */
extern FUNC(Dio_PortLevelType, DIO_CODE) Dio_ReadChannelGroup(
    P2CONST(Dio_ChannelGroupType, AUTOMATIC, DIO_APPL_CONST) ChannelGroupIdPtr
);

/**
 * @brief Writes specified levels to a group of DIO channels. (AUTOSAR SWS Ref: DIO058)
 * @param[in] ChannelGroupIdPtr Pointer to the channel group definition.
 * @param[in] Level             Levels to be written to the channel group.
 */
extern FUNC(void, DIO_CODE) Dio_WriteChannelGroup(
    P2CONST(Dio_ChannelGroupType, AUTOMATIC, DIO_APPL_CONST) ChannelGroupIdPtr,
    VAR(Dio_PortLevelType, AUTOMATIC) Level
);

#if (DIO_MASKED_WRITE_PORT_API == STD_ON) /* From Dio_Cfg.h */
/**
 * @brief Writes a port with masked bits. (AUTOSAR SWS Ref: DIO120)
 * @details Only bits that are set in the Mask will be affected in the port.
 * @param[in] PortId ID of DIO port.
 * @param[in] Level  Value to be written to the port.
 * @param[in] Mask   Mask indicating which bits to write.
 */
extern FUNC(void, DIO_CODE) Dio_MaskedWritePort(
    VAR(Dio_PortType, AUTOMATIC) PortId,
    VAR(Dio_PortLevelType, AUTOMATIC) Level,
    VAR(Dio_PortLevelType, AUTOMATIC) Mask
);
#endif /* DIO_MASKED_WRITE_PORT_API */

#if (DIO_VERSION_INFO_API == STD_ON) /* From Dio_Cfg.h */
/**
 * @brief Returns the version information of the DIO module. (AUTOSAR SWS Ref: DIO060)
 * @param[out] VersionInfoPtr Pointer to where to store the version information.
 *                         This pointer must not be NULL.
 */
extern FUNC(void, DIO_CODE) Dio_GetVersionInfo(
    P2VAR(Std_VersionInfoType, AUTOMATIC, DIO_APPL_DATA) VersionInfoPtr
);
#endif /* DIO_VERSION_INFO_API */

#endif /* DIO_H */
