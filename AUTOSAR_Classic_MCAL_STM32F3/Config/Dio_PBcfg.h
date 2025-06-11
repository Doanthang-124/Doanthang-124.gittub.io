#ifndef DIO_PBCFG_H
#define DIO_PBCFG_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "../Common/Include/Std_Types.h" /* For standard types */
#include "../Include/Dio.h"              /* For Dio types like Dio_ChannelType, Dio_PortType, etc. */
                                         /* and the initial/forward declaration of Dio_ConfigType */

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if source file and Dio header file are of the same vendor */
#if (DIO_VENDOR_ID != STD_TYPES_VENDOR_ID) /* Assuming STD_TYPES_VENDOR_ID, replace if different */
    #error "Dio_PBcfg.h and Std_Types.h have different vendor ids"
#endif
/* Check if source file and Dio header file are of the same AUTOSAR version */
/* TODO: Add AR version check if Std_Types.h has AR version info */

/* Check if source file and Dio header file are of the same Software version */
/* TODO: Add SW version check if Std_Types.h has SW version info */


/* Check if source file and Dio.h file are of the same vendor */
#if (DIO_VENDOR_ID != DIO_VENDOR_ID_CFG) /* Assuming DIO_VENDOR_ID_CFG is in Dio_Cfg.h or Dio.h */
    /* This check needs DIO_VENDOR_ID_CFG to be defined, typically in Dio_Cfg.h or Dio.h */
    /* For now, we assume DIO_VENDOR_ID is unique and defined in Dio.h */
#endif

/* Check if source file and Dio.h file are of the same AUTOSAR version */
#if ((DIO_AR_RELEASE_MAJOR_VERSION_PBCFG != DIO_AR_RELEASE_MAJOR_VERSION) || \
     (DIO_AR_RELEASE_MINOR_VERSION_PBCFG != DIO_AR_RELEASE_MINOR_VERSION) || \
     (DIO_AR_RELEASE_REVISION_VERSION_PBCFG != DIO_AR_RELEASE_REVISION_VERSION))
    /* These macros (DIO_AR_RELEASE_MAJOR_VERSION etc.) should be in Dio.h */
    /* #error "AutoSar Version Numbers of Dio_PBcfg.h and Dio.h are different" */
    /* NOTE: Actual AR version macros might not be in Dio.h yet, placeholder for future. */
#endif

/* Check if source file and Dio.h file are of the same Software version */
#if ((DIO_SW_MAJOR_VERSION_PBCFG != DIO_SW_MAJOR_VERSION) || \
     (DIO_SW_MINOR_VERSION_PBCFG != DIO_SW_MINOR_VERSION) || \
     (DIO_SW_PATCH_VERSION_PBCFG != DIO_SW_PATCH_VERSION))
    /* These macros (DIO_SW_MAJOR_VERSION etc.) should be in Dio.h */
    /* #error "Software Version Numbers of Dio_PBcfg.h and Dio.h are different" */
    /* NOTE: Actual SW version macros might not be in Dio.h yet, placeholder for future. */
#endif


/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*
 * Software Version Numbers for this PBcfg file.
 * These are optional and typically align with the overall driver version.
 */
#define DIO_PBCFG_SW_MAJOR_VERSION     1U
#define DIO_PBCFG_SW_MINOR_VERSION     0U
#define DIO_PBCFG_SW_PATCH_VERSION     0U

/*
 * AUTOSAR Version Information for this PBcfg file.
 * This should match the AUTOSAR release to which this configuration conforms.
 */
#define DIO_PBCFG_AR_RELEASE_MAJOR_VERSION       4U
#define DIO_PBCFG_AR_RELEASE_MINOR_VERSION       3U /* Or 9U depending on target AUTOSAR spec */
#define DIO_PBCFG_AR_RELEASE_REVISION_VERSION    1U /* Or patch version for 4.9.x */

/* Placeholder for Vendor ID and Module ID for this specific PBcfg file, if needed */
/* #define DIO_PBCFG_VENDOR_ID                 0U    */
/* #define DIO_PBCFG_MODULE_ID                 120U  */


/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                             TYPES
==================================================================================================*/

/*
 * @brief Post-build configuration for a single DIO channel.
 * @details This structure defines the configuration for a DIO channel that can be set
 *          post-build. For the Dio driver, this often just involves identifying the channel,
 *          as detailed configurations (direction, pull-up/down, speed, etc.) are typically
 *          part of the Port driver's responsibility.
 */
typedef struct
{
    Dio_ChannelType ChannelId; /* Symbolic name of the channel (e.g., DIO_CHANNEL_PA0) */
    /* Other channel-specific post-build parameters could be added here if necessary, */
    /* for example, an initial level or a flag for special handling. */
    /* boolean IsSynchronous; // Example: if some channels need different read/write behavior */
} Dio_ChannelPBConfigType;

/*
 * @brief Post-build configuration for a DIO port.
 * @details Defines post-build parameters for an entire DIO port.
 *          Similar to channels, for Dio, this might primarily be for identification.
 */
typedef struct
{
    Dio_PortType PortId;      /* Symbolic name of the port (e.g., DIO_PORT_A) */
    /* Port-specific parameters, e.g., a default level for all pins in the port */
    /* Dio_PortLevelType DefaultLevel; */
} Dio_PortPBConfigType;

/*
 * @brief Post-build configuration structure for the DIO Driver.
 * @details This is the top-level structure that holds all post-build configuration
 *          data for the Dio module. An instance of this structure (defined in Dio_PBcfg.c)
 *          is passed to the Dio_Init() function.
 * @note The Dio_ConfigType defined here provides the full definition for the
 *       type that might have been forward-declared or minimally defined in Dio.h.
 */
typedef struct
{
    /** @brief Pointer to the array of configured DIO channels. */
    const Dio_ChannelPBConfigType* ChannelsConfig;
    /** @brief Number of configured DIO channels in the array. */
    const uint8 NumConfiguredChannels;

    /** @brief Pointer to the array of configured DIO ports. */
    const Dio_PortPBConfigType* PortsConfig;
    /** @brief Number of configured DIO ports in the array. */
    const uint8 NumConfiguredPorts;

    /** @brief Pointer to the array of configured DIO channel groups. */
    const Dio_ChannelGroupType* ChannelGroupsConfig; /* Dio_ChannelGroupType is from Dio.h */
    /** @brief Number of configured DIO channel groups in the array. */
    const uint8 NumConfiguredChannelGroups;

    /* Other global post-build parameters can be added here, e.g., a general configuration flags */
    /* uint32 GlobalDioFlags; */
} Dio_ConfigType; /* This defines the actual structure for Dio_ConfigType */


/*==================================================================================================
*                                  EXTERNAL CONFIGURATION DECLARATIONS
==================================================================================================*/

/*
 * Declaration of the post-build configuration set(s).
 * The actual definition (i.e., the instance of Dio_ConfigType) will be in Dio_PBcfg.c.
 * The name 'DioConfig' is a common convention for the primary configuration structure.
 * If multiple configuration sets are supported, they would each be declared here.
 *
 * Example: extern const Dio_ConfigType MyDioConfiguration1;
 *          extern const Dio_ConfigType MyDioConfiguration2;
 *
 * For now, we declare a single, typically named configuration.
 * The MAKE_DIO_CONFIG_EXTERN macro can be used to abstract the extern keyword.
 */
#define DIO_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Dio_MemMap.h" /* Placeholder for MemMap header, if used */

/* Example: extern const Dio_ConfigType DioConfig; */
/* The specific name of the configuration structure instance (e.g., Dio_ConfigSet_Default) */
/* should be consistently used in Dio_PBcfg.c where it's defined. */
/* Let's use a common name like "Dio_Configuration" or "Dio_Config" */
extern const Dio_ConfigType Dio_Config; /* Or e.g. Dio_Config_Classic */

#define DIO_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Dio_MemMap.h" /* Placeholder for MemMap header, if used */


/*==================================================================================================
*                                      FUNCTION PROTOTYPES
==================================================================================================*/

/* Functions related to PB configuration could be declared here if any (e.g., validation functions) */

#endif /* DIO_PBCFG_H */
