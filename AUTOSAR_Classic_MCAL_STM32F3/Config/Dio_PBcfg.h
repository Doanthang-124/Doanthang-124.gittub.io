#ifndef DIO_PBCFG_H
#define DIO_PBCFG_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "../Common/Include/Std_Types.h" /* For Std_ReturnType, uint8, uint16, P2VAR, P2CONST, CONST etc. */
                                         /* Std_Types.h includes Compiler.h */

/*
 * Note: Dio.h includes this file (Dio_PBcfg.h). Therefore, Dio_PBcfg.h must NOT include Dio.h
 * to prevent circular dependencies. All types needed by Dio_PBcfg.h that are not from
 * Std_Types.h must be defined herein or included from other base headers.
 */

/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/
/* Software Version Numbers */
#define DIO_PBCFG_SW_MAJOR_VERSION     1U
#define DIO_PBCFG_SW_MINOR_VERSION     0U
#define DIO_PBCFG_SW_PATCH_VERSION     0U

/* AUTOSAR Version Information */
#define DIO_PBCFG_AR_RELEASE_MAJOR_VERSION    4U
#define DIO_PBCFG_AR_RELEASE_MINOR_VERSION    3U
#define DIO_PBCFG_AR_RELEASE_REVISION_VERSION 1U

/*==================================================================================================
*                                             TYPES
==================================================================================================*/

/*
 * These DIO-specific types are defined here as Dio.h includes this file (Dio_PBcfg.h)
 * to get the definition of Dio_ConfigType and Dio_ChannelGroupType for post-build configurations.
 * Placing these fundamental types here makes them available to Dio.h without circular includes.
 */

/** @brief Numeric ID of a DIO channel. Range depends on MCU. Typically uint8. */
typedef uint8 Dio_ChannelType;

/** @brief Numeric ID of a DIO port. Range depends on MCU. Typically uint8. */
typedef uint8 Dio_PortType;

/**
 * @brief Represents the physical state of a DIO channel.
 * @details Based on uint8.
 */
typedef uint8 Dio_LevelType;

/**
 * @brief Standard High level for a DIO channel.
 */
#ifndef STD_HIGH
    #define STD_HIGH    ((Dio_LevelType)0x01U)
#endif

/**
 * @brief Standard Low level for a DIO channel.
 */
#ifndef STD_LOW
    #define STD_LOW     ((Dio_LevelType)0x00U)
#endif


/** @brief Represents the levels of all channels in a DIO port.
 *  @details For STM32F3, ports are 16-bit wide. uint16 is appropriate.
 */
typedef uint16 Dio_PortLevelType;


/**
 * @brief Structure for defining a channel group.
 * @details A channel group is a contiguous set of channels within a single port.
 */
typedef struct
{
    Dio_PortType      PortIndex;    /**< @brief Port ID where the group is located (e.g., DIO_PORT_A). */
    uint8             Offset;       /**< @brief Bit offset of the LSB of the group within the port (0-15). */
    Dio_PortLevelType Mask;         /**< @brief Mask defining the channels included in the group (e.g., 0x00FF for lower 8 bits). */
} Dio_ChannelGroupType;


/**
 * @brief Post-build configuration structure for a single DIO channel.
 * @details This structure is primarily for identification in post-build configurations.
 *          Most channel properties (direction, mode, pull resistors) are configured by the Port module.
 */
typedef struct
{
    Dio_ChannelType ChannelId;      /**< @brief The symbolic ID of the DIO channel (e.g., DIO_CHANNEL_PA0 from Dio_Cfg.h). */
    /* Future DIO-specific post-build parameters for a channel could be added here, e.g.,
     * - boolean IsDebounced; // If DIO supports internal debouncing for this channel
     * - Dio_LevelType InitialLevel; // If DIO needs to set an initial output level distinct from Port driver
     */
} Dio_ChannelConfigType;


/**
 * @brief Post-build configuration structure for a single DIO port.
 * @details Similar to channels, primarily for identification in post-build configurations.
 */
typedef struct
{
    Dio_PortType PortId;            /**< @brief The symbolic ID of the DIO port (e.g., DIO_PORT_A from Dio_Cfg.h). */
    /* Dio_PortLevelType DefaultPortLevel; // Example: if a default output level for the whole port was needed */
} Dio_PortConfigType;


/**
 * @brief Main Post-Build Configuration Structure for the DIO Driver.
 * @details This structure contains pointers to arrays of configured channels, ports, and groups,
 *          along with their counts. An instance of this structure (e.g., `DioConfig`) is
 *          defined in `Dio_PBcfg.c` and passed to `Dio_Init()`.
 *          The tag name `Dio_ConfigTypeTag` is used for the struct definition to allow the
 *          typedef `Dio_ConfigType` to be used as the type name, adhering to MISRA C:2012 Rule 5.6.
 */
typedef struct Dio_ConfigTypeTag
{
    /** @brief Pointer to the array of configured DIO channel configurations. */
    P2CONST(Dio_ChannelConfigType, AUTOMATIC, CONFIG_CONST) Channels; /* CONFIG_CONST from Compiler.h */
    uint8 NumChannels;                                  /**< @brief Number of channels in the Channels array. */

    /** @brief Pointer to the array of configured DIO port configurations. */
    P2CONST(Dio_PortConfigType, AUTOMATIC, CONFIG_CONST) Ports;
    uint8 NumPorts;                                     /**< @brief Number of ports in the Ports array. */

    /** @brief Pointer to the array of configured DIO channel group configurations. */
    P2CONST(Dio_ChannelGroupType, AUTOMATIC, CONFIG_CONST) Groups;
    uint8 NumGroups;                                    /**< @brief Number of groups in the Groups array. */

} Dio_ConfigType;


/*==================================================================================================
*                                      EXTERNAL CONSTANTS
==================================================================================================*/

/**
 * @brief Declaration of the post-build configuration set for the DIO Driver.
 * @details The actual definition (the instance `DioConfig`) is in `Dio_PBcfg.c`.
 *          This uses the `CONFIG_CONST` memory class from `Compiler.h`.
 */
extern CONST(Dio_ConfigType, CONFIG_CONST) DioConfig;


#endif /* DIO_PBCFG_H */
