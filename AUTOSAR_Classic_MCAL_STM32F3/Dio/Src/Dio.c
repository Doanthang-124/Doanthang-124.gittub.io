/*==================================================================================================
*   Project              : AUTOSAR 4.x MCAL
*   Platform             : STM32
*   Peripheral           : GPIO
*   Dependencies         : STM32F3xx Series
*
*   Autosar Version      : 4.3.1
*   Autosar Revision     : ASR_REL_4_3_REV_0001
*   Sw Version           : 1.0.0
*   Build Version        : S32K14X_MCAL_1_0_0_RTM_ASR_REL_4_3_REV_0001_20190426
*
*   (c) Copyright 2006-2016 Freescale Semiconductor, Inc.
*   (c) Copyright 2017-2019 NXP
*   (c) Copyright 2023 YourName
*   All Rights Reserved.
==================================================================================================*/
/*==================================================================================================
==================================================================================================*/

#include "../Include/Dio.h"         /* Header file for Dio definitions */
#include "../Include/Dio_Regs.h"    /* Header file for register definitions (includes stm32f3xx.h) */
#include "../../Common/Include/Mcal.h" /* For STATIC definition, might be in Compiler.h directly */

#if (DIO_DEV_ERROR_DETECT == STD_ON)
    /* Include Det.h only if DIO_DEV_ERROR_DETECT is STD_ON */
    /* Path is relative to Dio/Src directory */
    #include "../../Common/Include/Det.h"
#endif

/*==================================================================================================
*                                       VERSION CHECKS
==================================================================================================*/
/* Check if Dio.c source file and Dio.h header file are of the same vendor */
#if (DIO_VENDOR_ID != DIO_VENDOR_ID_H)
    #error "Dio.c and Dio.h have different vendor ids"
#endif

/* Check if Dio.c source file and Dio.h header file are of the same Autosar version */
#if ((DIO_AR_RELEASE_MAJOR_VERSION != DIO_AR_RELEASE_MAJOR_VERSION_H) || \
     (DIO_AR_RELEASE_MINOR_VERSION != DIO_AR_RELEASE_MINOR_VERSION_H) || \
     (DIO_AR_RELEASE_REVISION_VERSION != DIO_AR_RELEASE_REVISION_VERSION_H))
    #error "AutoSar Version Numbers of Dio.c and Dio.h are different"
#endif

/* Check if Dio.c source file and Dio.h header file are of the same Software version */
#if ((DIO_SW_MAJOR_VERSION != DIO_SW_MAJOR_VERSION_H) || \
     (DIO_SW_MINOR_VERSION != DIO_SW_MINOR_VERSION_H) || \
     (DIO_SW_PATCH_VERSION != DIO_SW_PATCH_VERSION_H))
    #error "Software Version Numbers of Dio.c and Dio.h are different"
#endif

/* Check if Dio.c source file and Dio_Regs.h header file are of the same vendor */
/* (Assuming Dio_Regs.h will have these macros, if not, this check might need adjustment) */
/*
#if (DIO_VENDOR_ID != DIO_REGS_VENDOR_ID)
    #error "Dio.c and Dio_Regs.h have different vendor ids"
#endif
*/

/* Check if Dio.c source file and Dio_Regs.h header file are of the same Autosar version */
/*
#if ((DIO_AR_RELEASE_MAJOR_VERSION != DIO_REGS_AR_RELEASE_MAJOR_VERSION) || \
     (DIO_AR_RELEASE_MINOR_VERSION != DIO_REGS_AR_RELEASE_MINOR_VERSION) || \
     (DIO_AR_RELEASE_REVISION_VERSION != DIO_REGS_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Dio.c and Dio_Regs.h are different"
#endif
*/

/* Check if Dio.c source file and Dio_Regs.h header file are of the same Software version */
/*
#if ((DIO_SW_MAJOR_VERSION != DIO_REGS_SW_MAJOR_VERSION) || \
     (DIO_SW_MINOR_VERSION != DIO_REGS_SW_MINOR_VERSION) || \
     (DIO_SW_PATCH_VERSION != DIO_REGS_SW_PATCH_VERSION))
    #error "Software Version Numbers of Dio.c and Dio_Regs.h are different"
#endif
*/

/*==================================================================================================
*                                          VARIABLES
==================================================================================================*/

/* Static global variable to store the configuration pointer */
/* This pointer is initialized by Dio_Init(). */
STATIC P2CONST(Dio_ConfigType, DIO_VAR, DIO_APPL_CONST) Dio_ModuleConfigPtr = NULL_PTR;

/* Variable to store the state of the DIO driver */
STATIC VAR(boolean, DIO_VAR) Dio_IsInitialized = FALSE;


/*==================================================================================================
*                                   INTERNAL HELPER FUNCTIONS
==================================================================================================*/

/**
 * @brief Returns the GPIO peripheral base address for a given Dio_PortType.
 *
 * @param[in] PortId  The ID of the DIO port.
 * @return GPIO_TypeDef* Pointer to the GPIO peripheral registers, or NULL_PTR if PortId is invalid.
 *
 * @api private
 */
STATIC FUNC(GPIO_TypeDef*, DIO_CODE) Dio_GetPortBaseAddress(Dio_PortType PortId)
{
    GPIO_TypeDef* GpioPortAddr = NULL_PTR;
    switch(PortId)
    {
        case DIO_PORT_A: GpioPortAddr = GPIOA; break;
        case DIO_PORT_B: GpioPortAddr = GPIOB; break;
        case DIO_PORT_C: GpioPortAddr = GPIOC; break;
        case DIO_PORT_D: GpioPortAddr = GPIOD; break;
        case DIO_PORT_E: GpioPortAddr = GPIOE; break;
        case DIO_PORT_F: GpioPortAddr = GPIOF; break;
        default:
            /* Optional: Report error if PortId is out of expected range, */
            /* though primary validation should be in the public API functions. */
#if (DIO_DEV_ERROR_DETECT == STD_ON)
            Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_INTERNAL_API_ID, DIO_E_PARAM_INVALID_PORT_ID_INTERNAL);
#endif
            break;
    }
    return GpioPortAddr;
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/**
 * @brief Initializes the DIO driver.
 * @details This function initializes all configured DIO channels, ports, and groups
 *          according to the provided configuration structure.
 *
 * @param[in] ConfigPtr Pointer to the post-build configuration structure.
 *
 * @implements Dio_Init_Activity
 */
FUNC(void, DIO_CODE) Dio_Init(P2CONST(Dio_ConfigType, AUTOMATIC, DIO_APPL_CONST) ConfigPtr)
{
#if (DIO_DEV_ERROR_DETECT == STD_ON)
    /* For Post-Build configurations, ConfigPtr must not be NULL. */
    /* For Pre-Compile, ConfigPtr might be NULL if configuration is fixed. */
    /* Assuming this driver primarily supports Post-Build as per Dio_PBcfg.h */
    if (NULL_PTR == ConfigPtr)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_INIT_ID, DIO_E_PARAM_CONFIG);
    }
    else
#endif /* (DIO_DEV_ERROR_DETECT == STD_ON) */
    {
        Dio_ModuleConfigPtr = ConfigPtr;
        /*
         * Additional initialization steps based on ConfigPtr can be performed here.
         * For example, iterating through configured channels/ports in Dio_ModuleConfigPtr
         * to apply any initial states if the Port driver doesn't handle all of it.
         * However, for a pure DIO driver, this is often minimal, as the Port
         * driver is responsible for pin direction, mode, pull-ups, etc.
         * The Dio_Init mainly makes the configuration available to other Dio functions.
         */
        Dio_IsInitialized = TRUE;
    }
}

/**
 * @brief Reads the level of a specified DIO channel.
 * @param[in] ChannelId The ID of the DIO channel to read.
 * @return Dio_LevelType The level of the channel (STD_HIGH or STD_LOW).
 *
 * @implements Dio_ReadChannel_Activity
 */
FUNC(Dio_LevelType, DIO_CODE) Dio_ReadChannel(Dio_ChannelType ChannelId)
{
    VAR(Dio_LevelType, AUTOMATIC) ChannelLevel = STD_LOW;
    P2VAR(GPIO_TypeDef, AUTOMATIC, DIO_APPL_DATA) GpioPortAddr;
    VAR(uint8, AUTOMATIC) PinIndex;
    VAR(Dio_PortType, AUTOMATIC) PortId;

#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (FALSE == Dio_IsInitialized)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_READCHANNEL_ID, DIO_E_UNINIT);
        return STD_LOW; /* Return a default value */
    }
    /* Basic check for ChannelId validity based on STM32F3 max possible (PortF, Pin15 -> 6*16-1 = 95) */
    /* A more robust check would use Dio_ModuleConfigPtr to see if ChannelId is configured. */
    if (ChannelId >= (uint8)96) /* Max 6 ports * 16 pins/port */
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_READCHANNEL_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);
        return STD_LOW; /* Return a default value */
    }
#endif

    PortId = (Dio_PortType)(ChannelId / 16U); /* Determine port from ChannelId */
    PinIndex = (uint8)(ChannelId % 16U);    /* Determine pin index within the port */
    GpioPortAddr = Dio_GetPortBaseAddress(PortId);

    if (NULL_PTR != GpioPortAddr)
    {
        if ((GpioPortAddr->IDR & (1UL << PinIndex)) != 0U)
        {
            ChannelLevel = STD_HIGH;
        }
        else
        {
            ChannelLevel = STD_LOW;
        }
    }
#if (DIO_DEV_ERROR_DETECT == STD_ON)
    else /* GpioPortAddr was NULL, Dio_GetPortBaseAddress might have reported error */
    {
        /* Optionally report another error here if Dio_GetPortBaseAddress doesn't, */
        /* or rely on its DET call for invalid PortId derived from ChannelId. */
        /* Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_READCHANNEL_ID, DIO_E_PARAM_INVALID_CHANNEL_ID); */
    }
#endif
    return ChannelLevel;
}

/**
 * @brief Writes the specified level to a DIO channel.
 * @param[in] ChannelId The ID of the DIO channel to write.
 * @param[in] Level The level to write (STD_HIGH or STD_LOW).
 *
 * @implements Dio_WriteChannel_Activity
 */
FUNC(void, DIO_CODE) Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level)
{
    P2VAR(GPIO_TypeDef, AUTOMATIC, DIO_APPL_DATA) GpioPortAddr;
    VAR(uint8, AUTOMATIC) PinIndex;
    VAR(Dio_PortType, AUTOMATIC) PortId;

#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (FALSE == Dio_IsInitialized)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_WRITECHANNEL_ID, DIO_E_UNINIT);
        return;
    }
    if (ChannelId >= (uint8)96)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_WRITECHANNEL_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);
        return;
    }
    if ((Level != STD_HIGH) && (Level != STD_LOW))
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_WRITECHANNEL_ID, DIO_E_PARAM_INVALID_LEVEL);
        return;
    }
#endif

    PortId = (Dio_PortType)(ChannelId / 16U);
    PinIndex = (uint8)(ChannelId % 16U);
    GpioPortAddr = Dio_GetPortBaseAddress(PortId);

    if (NULL_PTR != GpioPortAddr)
    {
        if (Level == STD_HIGH)
        {
            GpioPortAddr->BSRR = (1UL << PinIndex); /* Set bit */
        }
        else
        {
            GpioPortAddr->BSRR = (1UL << (PinIndex + 16U)); /* Reset bit (using BRR part of BSRR) */
        }
    }
#if (DIO_DEV_ERROR_DETECT == STD_ON)
    else
    {
        /* Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_WRITECHANNEL_ID, DIO_E_PARAM_INVALID_CHANNEL_ID); */
    }
#endif
}

/**
 * @brief Reads the level of all channels of a specified DIO port.
 * @param[in] PortId The ID of the DIO port to read.
 * @return Dio_PortLevelType The combined levels of all channels in the port.
 *
 * @implements Dio_ReadPort_Activity
 */
FUNC(Dio_PortLevelType, DIO_CODE) Dio_ReadPort(Dio_PortType PortId)
{
    VAR(Dio_PortLevelType, AUTOMATIC) PortLevel = 0U;
    P2VAR(GPIO_TypeDef, AUTOMATIC, DIO_APPL_DATA) GpioPortAddr;

#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (FALSE == Dio_IsInitialized)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_READPORT_ID, DIO_E_UNINIT);
        return 0U; /* Return a default value */
    }
    if (PortId >= (uint8)6) /* Assuming max Port F (0-5) */
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_READPORT_ID, DIO_E_PARAM_INVALID_PORT_ID);
        return 0U; /* Return a default value */
    }
#endif

    GpioPortAddr = Dio_GetPortBaseAddress(PortId);

    if (NULL_PTR != GpioPortAddr)
    {
        PortLevel = (Dio_PortLevelType)(GpioPortAddr->IDR & 0xFFFFU); /* Read all 16 pins of IDR */
    }
    /* No specific DET call if GpioPortAddr is NULL here, as Dio_GetPortBaseAddress would have reported it. */
    return PortLevel;
}

/**
 * @brief Writes the specified levels to all channels of a DIO port.
 * @param[in] PortId The ID of the DIO port to write.
 * @param[in] Level The levels to write to the port.
 *
 * @implements Dio_WritePort_Activity
 */
FUNC(void, DIO_CODE) Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level)
{
    P2VAR(GPIO_TypeDef, AUTOMATIC, DIO_APPL_DATA) GpioPortAddr;

#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (FALSE == Dio_IsInitialized)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_WRITEPORT_ID, DIO_E_UNINIT);
        return;
    }
    if (PortId >= (uint8)6)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_WRITEPORT_ID, DIO_E_PARAM_INVALID_PORT_ID);
        return;
    }
#endif

    GpioPortAddr = Dio_GetPortBaseAddress(PortId);

    if (NULL_PTR != GpioPortAddr)
    {
        GpioPortAddr->ODR = (uint32_t)Level; /* Write directly to ODR */
    }
    /* No specific DET call if GpioPortAddr is NULL here. */
}


#if (DIO_VERSION_INFO_API == STD_ON)
/**
 * @brief Returns the version information of the DIO module.
 * @param[out] VersionInfo Pointer to where to store the version information.
 *
 * @implements Dio_GetVersionInfo_Activity
 */
FUNC(void, DIO_CODE) Dio_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, DIO_APPL_DATA) VersionInfo)
{
#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == VersionInfo)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_GETVERSIONINFO_ID, DIO_E_PARAM_POINTER);
        return;
    }
#endif
    VersionInfo->vendorID         = DIO_VENDOR_ID;
    VersionInfo->moduleID         = DIO_MODULE_ID;
    VersionInfo->sw_major_version = DIO_SW_MAJOR_VERSION;
    VersionInfo->sw_minor_version = DIO_SW_MINOR_VERSION;
    VersionInfo->sw_patch_version = DIO_SW_PATCH_VERSION;
}
#endif /* (DIO_VERSION_INFO_API == STD_ON) */


#if (DIO_FLIP_CHANNEL_API == STD_ON)
/**
 * @brief Flips the level of a specified DIO channel.
 * @param[in] ChannelId The ID of the DIO channel to flip.
 * @return Dio_LevelType The new level of the channel after flipping.
 *
 * @implements Dio_FlipChannel_Activity
 */
FUNC(Dio_LevelType, DIO_CODE) Dio_FlipChannel(Dio_ChannelType ChannelId)
{
    VAR(Dio_LevelType, AUTOMATIC) CurrentLevel;
    VAR(Dio_LevelType, AUTOMATIC) NewLevel = STD_LOW; /* Default to STD_LOW */

#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (FALSE == Dio_IsInitialized)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_FLIPCHANNEL_ID, DIO_E_UNINIT);
        return STD_LOW; /* Return a default value */
    }
    if (ChannelId >= (uint8)96)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_FLIPCHANNEL_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);
        return STD_LOW; /* Return a default value */
    }
#endif

    /* Read current level */
    CurrentLevel = Dio_ReadChannel(ChannelId); /* Dio_ReadChannel already performs checks if DET is on */

    /* Determine new level and write it */
    if (CurrentLevel == STD_HIGH)
    {
        NewLevel = STD_LOW;
    }
    else
    {
        NewLevel = STD_HIGH;
    }
    Dio_WriteChannel(ChannelId, NewLevel); /* Dio_WriteChannel already performs checks if DET is on */

    return NewLevel;
}
#endif /* (DIO_FLIP_CHANNEL_API == STD_ON) */


/* TODO: Implement Dio_ReadChannelGroup, Dio_WriteChannelGroup, Dio_MaskedWritePort */
/* These are more complex and require careful handling of masks and offsets. */

/**
 * @brief Reads the level of a specified DIO channel group.
 * @param[in] ChannelGroupIdPtr Pointer to the channel group definition.
 * @return Dio_PortLevelType The levels of the channels in the group.
 */
FUNC(Dio_PortLevelType, DIO_CODE) Dio_ReadChannelGroup(P2CONST(Dio_ChannelGroupType, AUTOMATIC, DIO_APPL_CONST) ChannelGroupIdPtr)
{
    VAR(Dio_PortLevelType, AUTOMATIC) GroupLevel = 0U;
    P2VAR(GPIO_TypeDef, AUTOMATIC, DIO_APPL_DATA) GpioPortAddr;

#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (FALSE == Dio_IsInitialized)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_READCHANNELGROUP_ID, DIO_E_UNINIT);
        return 0U;
    }
    if (NULL_PTR == ChannelGroupIdPtr)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_READCHANNELGROUP_ID, DIO_E_PARAM_POINTER);
        return 0U;
    }
    if (ChannelGroupIdPtr->port >= (uint8)6) /* Basic check for port ID */
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_READCHANNELGROUP_ID, DIO_E_PARAM_INVALID_GROUP_ID);
        return 0U;
    }
#endif

    GpioPortAddr = Dio_GetPortBaseAddress(ChannelGroupIdPtr->port);

    if (NULL_PTR != GpioPortAddr)
    {
        GroupLevel = (Dio_PortLevelType)((GpioPortAddr->IDR & ChannelGroupIdPtr->mask) >> ChannelGroupIdPtr->offset);
    }
    return GroupLevel;
}


/**
 * @brief Writes the specified levels to a DIO channel group.
 * @param[in] ChannelGroupIdPtr Pointer to the channel group definition.
 * @param[in] Level The levels to write to the group.
 */
FUNC(void, DIO_CODE) Dio_WriteChannelGroup(P2CONST(Dio_ChannelGroupType, AUTOMATIC, DIO_APPL_CONST) ChannelGroupIdPtr, Dio_PortLevelType Level)
{
    P2VAR(GPIO_TypeDef, AUTOMATIC, DIO_APPL_DATA) GpioPortAddr;
    VAR(uint32, AUTOMATIC) PortValue;

#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (FALSE == Dio_IsInitialized)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_WRITECHANNELGROUP_ID, DIO_E_UNINIT);
        return;
    }
    if (NULL_PTR == ChannelGroupIdPtr)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_WRITECHANNELGROUP_ID, DIO_E_PARAM_POINTER);
        return;
    }
    if (ChannelGroupIdPtr->port >= (uint8)6)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_WRITECHANNELGROUP_ID, DIO_E_PARAM_INVALID_GROUP_ID);
        return;
    }
#endif

    GpioPortAddr = Dio_GetPortBaseAddress(ChannelGroupIdPtr->port);

    if (NULL_PTR != GpioPortAddr)
    {
        PortValue = GpioPortAddr->ODR; /* Read current ODR */
        PortValue &= ~( (uint32)ChannelGroupIdPtr->mask ); /* Clear bits in mask */
        PortValue |= ( ((uint32)Level << ChannelGroupIdPtr->offset) & (uint32)ChannelGroupIdPtr->mask ); /* Set new values */
        GpioPortAddr->ODR = PortValue;
    }
}


/**
 * @brief Writes a masked value to a port.
 * @param[in] PortId The ID of the DIO port.
 * @param[in] Level The value to write to the port.
 * @param[in] Mask The mask to apply to the port write.
 */
FUNC(void, DIO_CODE) Dio_MaskedWritePort(Dio_PortType PortId, Dio_PortLevelType Level, Dio_PortLevelType Mask)
{
    P2VAR(GPIO_TypeDef, AUTOMATIC, DIO_APPL_DATA) GpioPortAddr;
    VAR(uint32, AUTOMATIC) PortValue;

#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (FALSE == Dio_IsInitialized)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_MASKEDWRITEPORT_ID, DIO_E_UNINIT);
        return;
    }
    if (PortId >= (uint8)6)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_MASKEDWRITEPORT_ID, DIO_E_PARAM_INVALID_PORT_ID);
        return;
    }
#endif

    GpioPortAddr = Dio_GetPortBaseAddress(PortId);

    if (NULL_PTR != GpioPortAddr)
    {
        PortValue = GpioPortAddr->ODR;       /* Read current ODR */
        PortValue &= ~((uint32_t)Mask);      /* Clear bits specified by mask */
        PortValue |= ((uint32_t)Level & (uint32_t)Mask); /* Set bits specified by Level and Mask */
        GpioPortAddr->ODR = PortValue;
    }
}
