/*==================================================================================================
*   Project              : AUTOSAR 4.x MCAL
*   Platform             : STM32
*   Peripheral           : DIO
*   Dependencies         : STM32F3xx Series
*
*   Autosar Version      : 4.3.1
*   Autosar Revision     : ASR_REL_4_3_REV_0001
*   Sw Version           : 1.0.0
*   Build Version        : DIO_MCAL_1_0_0_RTM_ASR_REL_4_3_REV_0001_2024xxxx
*
==================================================================================================*/

#include "../Include/Dio.h"         /* Includes Dio_Cfg.h, Dio_PBcfg.h, Std_Types.h (and Compiler.h) */
#include "../Include/Dio_Regs.h"     /* Includes stm32f3xx.h for register definitions */

#if (DIO_DEV_ERROR_DETECT == STD_ON) /* From Dio_Cfg.h */
    #include "../../Det/Include/Det.h"   /* DET header */
#endif

/*==================================================================================================
*                                       VERSION CHECKS
==================================================================================================*/
/* Check if source file and DIO header file are of the same software version */
#if ((DIO_SW_MAJOR_VERSION != 1U) ||      \
     (DIO_SW_MINOR_VERSION != 0U) ||      \
     (DIO_SW_PATCH_VERSION != 0U))
    #error "Software Version Numbers of Dio.c and Dio.h are different"
#endif

/* Check if source file and DIO header file are of the same AUTOSAR version */
#if ((DIO_AR_RELEASE_MAJOR_VERSION != 4U) || \
     (DIO_AR_RELEASE_MINOR_VERSION != 3U) || \
     (DIO_AR_RELEASE_PATCH_VERSION != 1U))
    #error "AUTOSAR Version Numbers of Dio.c and Dio.h are different"
#endif

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
/** @brief Number of pins per GPIO port on this MCU (STM32 typically have 16 pins). */
#define DIO_PINS_PER_PORT              (16U)

/** @brief Maximum valid Port ID (0-5 for PortA-PortF, assuming 6 ports). */
/* This should ideally be derived from configuration or MCU capabilities. */
#define DIO_MAX_PORT_ID                (5U)

/** @brief Maximum valid Channel ID. (e.g., (5+1) ports * 16 pins/port - 1 = 95) */
#define DIO_MAX_CHANNEL_ID             (((DIO_MAX_PORT_ID + 1U) * DIO_PINS_PER_PORT) - 1U)


/*==================================================================================================
*                                          VARIABLES
==================================================================================================*/
#define DIO_START_SEC_VAR_INIT_UNSPECIFIED
/* #include "MemMap.h" */ /* Or Dio_MemMap.h */

/** @brief Pointer to the current DIO driver configuration set. Initialized by Dio_Init(). */
STATIC P2CONST(Dio_ConfigType, DIO_VAR, DIO_APPL_CONST) Dio_pxCurrentConfig = NULL_PTR;

#define DIO_STOP_SEC_VAR_INIT_UNSPECIFIED
/* #include "MemMap.h" */

#define DIO_START_SEC_VAR_CLEARED_UNSPECIFIED
/* #include "MemMap.h" */

/** @brief Status of the DIO driver initialization. */
STATIC VAR(boolean, DIO_VAR) Dio_bIsInitialized = FALSE;

#define DIO_STOP_SEC_VAR_CLEARED_UNSPECIFIED
/* #include "MemMap.h" */


/*==================================================================================================
*                                   INTERNAL HELPER FUNCTIONS
==================================================================================================*/
#define DIO_START_SEC_CODE
/* #include "MemMap.h" */

/**
 * @brief Retrieves the base address of the GPIO port for a given DIO PortType ID.
 * @param[in] PortId The ID of the DIO port (0 for PortA, 1 for PortB, etc.).
 * @return GPIO_TypeDef* Pointer to the GPIO port registers, or NULL_PTR if PortId is invalid.
 */
STATIC FUNC(P2VAR(GPIO_TypeDef, AUTOMATIC, DIO_CODE), DIO_CODE) Dio_Internal_GetPortBaseAddr(VAR(Dio_PortType, AUTOMATIC) PortId)
{
    P2VAR(GPIO_TypeDef, AUTOMATIC, DIO_APPL_DATA) pxGpioPort = NULL_PTR;

    switch(PortId) /* PortId values correspond to DIO_PORT_A, DIO_PORT_B, ... from Dio_Cfg.h */
    {
        case 0U: pxGpioPort = GPIOA; break;
        case 1U: pxGpioPort = GPIOB; break;
        case 2U: pxGpioPort = GPIOC; break;
        case 3U: pxGpioPort = GPIOD; break;
        case 4U: pxGpioPort = GPIOE; break;
        case 5U: pxGpioPort = GPIOF; break;
        /* Add cases for GPIOG, GPIOH if supported by the target MCU and configured */
        default:
            /* Invalid PortId. This case should ideally not be reached if PortId is validated by the caller. */
            /* No DET report here to avoid multiple reports if caller also reports. */
            break;
    }
    return pxGpioPort;
}


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/**
 * @see Dio.h (SWS_Dio_00099)
 */
FUNC(void, DIO_CODE) Dio_Init(P2CONST(Dio_ConfigType, AUTOMATIC, DIO_APPL_CONST) ConfigPtr)
{
#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (Dio_bIsInitialized == TRUE)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_INIT_API_ID, DIO_E_ALREADY_INITIALIZED);
    }
    else if (NULL_PTR == ConfigPtr) /* Check for NULL pointer if DET is ON */
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_INIT_API_ID, DIO_E_PARAM_POINTER);
    }
    /* Add more ConfigPtr validation if its members are used by Dio_Init, e.g.
     * else if (ConfigPtr->NumChannels > DIO_MAX_CONFIGURED_CHANNELS_IN_PB) { ... }
     */
    else
#endif /* DIO_DEV_ERROR_DETECT == STD_ON */
    {
        Dio_pxCurrentConfig = ConfigPtr; /* Store pointer to post-build configuration */
        Dio_bIsInitialized = TRUE;       /* Set state to initialized */

        /* The DIO driver itself usually does not perform pin configuration (direction, mode, etc.).
         * This is typically the responsibility of the Port driver. Dio_Init's main role is to
         * make the configuration available and mark the module as initialized.
         * If specific initializations related to DIO behavior (not pin setup) were needed,
         * they would be done here based on Dio_pxCurrentConfig.
         */
    }
}


/**
 * @see Dio.h (SWS_Dio_00053)
 */
FUNC(Dio_LevelType, DIO_CODE) Dio_ReadChannel(VAR(Dio_ChannelType, AUTOMATIC) ChannelId)
{
    VAR(Dio_LevelType, AUTOMATIC) eChannelLevel = STD_LOW; /* Default return for error cases or if pin is low */
    P2VAR(GPIO_TypeDef, AUTOMATIC, DIO_APPL_DATA) pxPortBaseAddr;
    VAR(Dio_PortType, AUTOMATIC) u8PortId;
    VAR(uint8, AUTOMATIC) u8PinIndex;

#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (Dio_bIsInitialized == FALSE)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_READ_CHANNEL_API_ID, DIO_E_UNINIT);
        /* eChannelLevel remains STD_LOW */
    }
    else if (ChannelId > DIO_MAX_CHANNEL_ID) /* Validate ChannelId against max possible */
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_READ_CHANNEL_API_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);
        /* eChannelLevel remains STD_LOW */
    }
    else
#endif /* DIO_DEV_ERROR_DETECT == STD_ON */
    {
        u8PortId = (Dio_PortType)(ChannelId / DIO_PINS_PER_PORT);
        u8PinIndex = (uint8)(ChannelId % DIO_PINS_PER_PORT);
        pxPortBaseAddr = Dio_Internal_GetPortBaseAddr(u8PortId);

        if (NULL_PTR != pxPortBaseAddr)
        {
            if ((pxPortBaseAddr->IDR & (1UL << u8PinIndex)) != 0U)
            {
                eChannelLevel = STD_HIGH;
            }
            /* else eChannelLevel remains STD_LOW (already initialized) */
        }
#if (DIO_DEV_ERROR_DETECT == STD_ON)
        else /* pxPortBaseAddr is NULL, implies invalid PortId derived from ChannelId */
        {
            /* This specific error might be redundant if ChannelId check is robust and mapping is contiguous.
               However, if Dio_Internal_GetPortBaseAddr has other reasons to return NULL for a valid-looking PortId
               (e.g., port not enabled/clocked in a more complex system), this DET could be useful.
               For now, assume ChannelId > DIO_MAX_CHANNEL_ID covers invalid port derivation.
            */
        }
#endif
    }
    return eChannelLevel;
}


/**
 * @see Dio.h (SWS_Dio_00056)
 */
FUNC(void, DIO_CODE) Dio_WriteChannel(VAR(Dio_ChannelType, AUTOMATIC) ChannelId, VAR(Dio_LevelType, AUTOMATIC) Level)
{
    P2VAR(GPIO_TypeDef, AUTOMATIC, DIO_APPL_DATA) pxPortBaseAddr;
    VAR(Dio_PortType, AUTOMATIC) u8PortId;
    VAR(uint8, AUTOMATIC) u8PinIndex;

#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (Dio_bIsInitialized == FALSE)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_WRITE_CHANNEL_API_ID, DIO_E_UNINIT);
    }
    else if (ChannelId > DIO_MAX_CHANNEL_ID)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_WRITE_CHANNEL_API_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);
    }
    else if ((Level != STD_HIGH) && (Level != STD_LOW)) /* Validate Level parameter */
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_WRITE_CHANNEL_API_ID, DIO_E_PARAM_INVALID_LEVEL);
    }
    else
#endif /* DIO_DEV_ERROR_DETECT == STD_ON */
    {
        u8PortId = (Dio_PortType)(ChannelId / DIO_PINS_PER_PORT);
        u8PinIndex = (uint8)(ChannelId % DIO_PINS_PER_PORT);
        pxPortBaseAddr = Dio_Internal_GetPortBaseAddr(u8PortId);

        if (NULL_PTR != pxPortBaseAddr)
        {
            if (Level == STD_HIGH)
            {
                pxPortBaseAddr->BSRR = (1UL << u8PinIndex); /* Atomic set */
            }
            else
            {
                pxPortBaseAddr->BSRR = (1UL << (u8PinIndex + DIO_PINS_PER_PORT)); /* Atomic reset */
            }
        }
    }
}


#if (DIO_FLIP_CHANNEL_API == STD_ON)
/**
 * @see Dio.h (SWS_Dio_00113)
 */
FUNC(Dio_LevelType, DIO_CODE) Dio_FlipChannel(VAR(Dio_ChannelType, AUTOMATIC) ChannelId)
{
    VAR(Dio_LevelType, AUTOMATIC) eCurrentLevel;
    VAR(Dio_LevelType, AUTOMATIC) eNewLevel = STD_LOW; /* Default return for error cases */

#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (Dio_bIsInitialized == FALSE)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_FLIP_CHANNEL_API_ID, DIO_E_UNINIT);
        /* eNewLevel remains STD_LOW */
    }
    else if (ChannelId > DIO_MAX_CHANNEL_ID)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_FLIP_CHANNEL_API_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);
        /* eNewLevel remains STD_LOW */
    }
    else
#endif
    {
        eCurrentLevel = Dio_ReadChannel(ChannelId); /* This call includes its own DET checks */

        /* Determine new level based on current level */
        if (eCurrentLevel == STD_LOW)
        {
            eNewLevel = STD_HIGH;
        }
        else /* Current level is STD_HIGH (or invalid read returned STD_LOW, leading to HIGH flip) */
        {
            eNewLevel = STD_LOW;
        }
        Dio_WriteChannel(ChannelId, eNewLevel); /* This call includes its own DET checks */
    }
    return eNewLevel;
}
#endif /* DIO_FLIP_CHANNEL_API */


/**
 * @see Dio.h (SWS_Dio_00050)
 */
FUNC(Dio_PortLevelType, DIO_CODE) Dio_ReadPort(VAR(Dio_PortType, AUTOMATIC) PortId)
{
    VAR(Dio_PortLevelType, AUTOMATIC) u16PortLevel = 0U;
    P2VAR(GPIO_TypeDef, AUTOMATIC, DIO_APPL_DATA) pxPortBaseAddr;

#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (Dio_bIsInitialized == FALSE)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_READ_PORT_API_ID, DIO_E_UNINIT);
    }
    else if (PortId > DIO_MAX_PORT_ID)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_READ_PORT_API_ID, DIO_E_PARAM_INVALID_PORT_ID);
    }
    else
#endif
    {
        pxPortBaseAddr = Dio_Internal_GetPortBaseAddr(PortId);
        if (NULL_PTR != pxPortBaseAddr)
        {
            u16PortLevel = (Dio_PortLevelType)(pxPortBaseAddr->IDR & 0xFFFFU);
        }
    }
    return u16PortLevel;
}


/**
 * @see Dio.h (SWS_Dio_00057)
 */
FUNC(void, DIO_CODE) Dio_WritePort(VAR(Dio_PortType, AUTOMATIC) PortId, VAR(Dio_PortLevelType, AUTOMATIC) Level)
{
    P2VAR(GPIO_TypeDef, AUTOMATIC, DIO_APPL_DATA) pxPortBaseAddr;

#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (Dio_bIsInitialized == FALSE)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_WRITE_PORT_API_ID, DIO_E_UNINIT);
    }
    else if (PortId > DIO_MAX_PORT_ID)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_WRITE_PORT_API_ID, DIO_E_PARAM_INVALID_PORT_ID);
    }
    else
#endif
    {
        pxPortBaseAddr = Dio_Internal_GetPortBaseAddr(PortId);
        if (NULL_PTR != pxPortBaseAddr)
        {
            pxPortBaseAddr->ODR = (uint32)Level; /* ODR is 32-bit, but only lower 16 bits are used for GPIO pins */
        }
    }
}


/**
 * @see Dio.h (SWS_Dio_00052)
 */
FUNC(Dio_PortLevelType, DIO_CODE) Dio_ReadChannelGroup(
    P2CONST(Dio_ChannelGroupType, AUTOMATIC, DIO_APPL_CONST) ChannelGroupIdPtr
)
{
    VAR(Dio_PortLevelType, AUTOMATIC) u16GroupLevel = 0U;
    P2VAR(GPIO_TypeDef, AUTOMATIC, DIO_APPL_DATA) pxPortBaseAddr;

#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (Dio_bIsInitialized == FALSE)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_READ_CHANNEL_GROUP_API_ID, DIO_E_UNINIT);
    }
    else if (NULL_PTR == ChannelGroupIdPtr)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_READ_CHANNEL_GROUP_API_ID, DIO_E_PARAM_POINTER);
    }
    else if (ChannelGroupIdPtr->PortIndex > DIO_MAX_PORT_ID)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_READ_CHANNEL_GROUP_API_ID, DIO_E_PARAM_INVALID_GROUP_ID);
    }
    /* Additional checks for offset and mask could be added here if necessary */
    else
#endif
    {
        pxPortBaseAddr = Dio_Internal_GetPortBaseAddr(ChannelGroupIdPtr->PortIndex);
        if (NULL_PTR != pxPortBaseAddr)
        {
            u16GroupLevel = (Dio_PortLevelType)((pxPortBaseAddr->IDR & ChannelGroupIdPtr->Mask) >> ChannelGroupIdPtr->Offset);
        }
    }
    return u16GroupLevel;
}

/**
 * @see Dio.h (SWS_Dio_00058)
 */
FUNC(void, DIO_CODE) Dio_WriteChannelGroup(
    P2CONST(Dio_ChannelGroupType, AUTOMATIC, DIO_APPL_CONST) ChannelGroupIdPtr,
    VAR(Dio_PortLevelType, AUTOMATIC) Level
)
{
    P2VAR(GPIO_TypeDef, AUTOMATIC, DIO_APPL_DATA) pxPortBaseAddr;
    VAR(uint32, AUTOMATIC) u32TempOdr;

#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (Dio_bIsInitialized == FALSE)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_WRITE_CHANNEL_GROUP_API_ID, DIO_E_UNINIT);
    }
    else if (NULL_PTR == ChannelGroupIdPtr)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_WRITE_CHANNEL_GROUP_API_ID, DIO_E_PARAM_POINTER);
    }
    else if (ChannelGroupIdPtr->PortIndex > DIO_MAX_PORT_ID)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_WRITE_CHANNEL_GROUP_API_ID, DIO_E_PARAM_INVALID_GROUP_ID);
    }
    /* Additional checks for offset, mask, and Level could be added */
    else
#endif
    {
        pxPortBaseAddr = Dio_Internal_GetPortBaseAddr(ChannelGroupIdPtr->PortIndex);
        if (NULL_PTR != pxPortBaseAddr)
        {
            u32TempOdr = pxPortBaseAddr->ODR;                                                           /* Read current ODR */
            u32TempOdr &= ~((uint32)ChannelGroupIdPtr->Mask);                                           /* Clear bits defined by mask */
            u32TempOdr |= (((uint32)Level << ChannelGroupIdPtr->Offset) & (uint32)ChannelGroupIdPtr->Mask); /* Apply new level for masked bits */
            pxPortBaseAddr->ODR = u32TempOdr;
        }
    }
}


#if (DIO_MASKED_WRITE_PORT_API == STD_ON)
/**
 * @see Dio.h (SWS_Dio_00120)
 */
FUNC(void, DIO_CODE) Dio_MaskedWritePort(
    VAR(Dio_PortType, AUTOMATIC) PortId,
    VAR(Dio_PortLevelType, AUTOMATIC) Level,
    VAR(Dio_PortLevelType, AUTOMATIC) Mask
)
{
    P2VAR(GPIO_TypeDef, AUTOMATIC, DIO_APPL_DATA) pxPortBaseAddr;
    VAR(uint32, AUTOMATIC) u32CurrentOdr;

#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (Dio_bIsInitialized == FALSE)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_MASKED_WRITE_PORT_API_ID, DIO_E_UNINIT);
    }
    else if (PortId > DIO_MAX_PORT_ID)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_MASKED_WRITE_PORT_API_ID, DIO_E_PARAM_INVALID_PORT_ID);
    }
    else
#endif
    {
        pxPortBaseAddr = Dio_Internal_GetPortBaseAddr(PortId);
        if (NULL_PTR != pxPortBaseAddr)
        {
            u32CurrentOdr = pxPortBaseAddr->ODR;
            u32CurrentOdr &= ~((uint32)Mask);                       /* Clear bits specified by Mask */
            u32CurrentOdr |= ((uint32)Level & (uint32)Mask);       /* Set new values for masked bits */
            pxPortBaseAddr->ODR = u32CurrentOdr;
        }
    }
}
#endif /* DIO_MASKED_WRITE_PORT_API */


#if (DIO_VERSION_INFO_API == STD_ON)
/**
 * @see Dio.h (SWS_Dio_00060)
 */
FUNC(void, DIO_CODE) Dio_GetVersionInfo(
    P2VAR(Std_VersionInfoType, AUTOMATIC, DIO_APPL_DATA) VersionInfoPtr
)
{
#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == VersionInfoPtr)
    {
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_GET_VERSION_INFO_API_ID, DIO_E_PARAM_POINTER);
    }
    else
#endif /* DIO_DEV_ERROR_DETECT == STD_ON */
    {
        VersionInfoPtr->vendorID         = DIO_VENDOR_ID;
        VersionInfoPtr->moduleID         = DIO_MODULE_ID;
        VersionInfoPtr->sw_major_version = DIO_SW_MAJOR_VERSION;
        VersionInfoPtr->sw_minor_version = DIO_SW_MINOR_VERSION;
        VersionInfoPtr->sw_patch_version = DIO_SW_PATCH_VERSION;
    }
}
#endif /* DIO_VERSION_INFO_API */

#define DIO_STOP_SEC_CODE
/* #include "MemMap.h" */ /* Or Dio_MemMap.h */
