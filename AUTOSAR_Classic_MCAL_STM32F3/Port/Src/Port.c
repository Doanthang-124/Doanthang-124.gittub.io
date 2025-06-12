/*==================================================================================================
*   Project              : AUTOSAR 4.x MCAL
*   Platform             : STM32
*   Peripheral           : PORT
*   Dependencies         : STM32F3xx Series
*
*   Autosar Version      : 4.3.1
*   Autosar Revision     : ASR_REL_4_3_REV_0001
*   Sw Version           : 1.0.0
*   Build Version        : PORT_MCAL_1_0_0_RTM_ASR_REL_4_3_REV_0001_2024xxxx
*
==================================================================================================*/

#include "../Include/Port.h"       /* Main Port driver header (includes Port_Cfg.h, Port_PBcfg.h, Std_Types.h) */
#include "../Include/Port_Regs.h"  /* For direct register access (includes stm32f3xx.h) */

#if (PORT_DEV_ERROR_DETECT == STD_ON) /* From Port_Cfg.h */
    #include "../../Det/Include/Det.h"   /* DET header */
#endif

/*==================================================================================================
*                                       VERSION CHECKS
==================================================================================================*/
/* Define this file's version numbers to match Port.h */
#define PORT_SW_MAJOR_VERSION_C             PORT_SW_MAJOR_VERSION
#define PORT_SW_MINOR_VERSION_C             PORT_SW_MINOR_VERSION
#define PORT_SW_PATCH_VERSION_C             PORT_SW_PATCH_VERSION

#define PORT_AR_RELEASE_MAJOR_VERSION_C     PORT_AR_RELEASE_MAJOR_VERSION
#define PORT_AR_RELEASE_MINOR_VERSION_C     PORT_AR_RELEASE_MINOR_VERSION
#define PORT_AR_RELEASE_PATCH_VERSION_C     PORT_AR_RELEASE_PATCH_VERSION

/* Check if source file and PORT header file are of the same software version */
#if ((PORT_SW_MAJOR_VERSION_C != PORT_SW_MAJOR_VERSION) || \
     (PORT_SW_MINOR_VERSION_C != PORT_SW_MINOR_VERSION) || \
     (PORT_SW_PATCH_VERSION_C != PORT_SW_PATCH_VERSION))
    #error "Software Version Numbers of Port.c and Port.h are different"
#endif

/* Check if source file and PORT header file are of the same AUTOSAR version */
#if ((PORT_AR_RELEASE_MAJOR_VERSION_C != PORT_AR_RELEASE_MAJOR_VERSION) || \
     (PORT_AR_RELEASE_MINOR_VERSION_C != PORT_AR_RELEASE_MINOR_VERSION) || \
     (PORT_AR_RELEASE_PATCH_VERSION_C != PORT_AR_RELEASE_PATCH_VERSION))
    #error "AUTOSAR Version Numbers of Port.c and Port.h are different"
#endif


/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
#define PORT_GPIO_MODE_BITS_MASK        (0x03UL) /**< Mask for 2 mode bits in MODER, OSPEEDR, PUPDR */
#define PORT_GPIO_AF_BITS_MASK          (0x0FUL) /**< Mask for 4 alternate function bits in AFR */
#define PORT_PINS_PER_GPIO_PORT         (16U)    /**< Number of pins per GPIO port for STM32 */


/*==================================================================================================
*                                          VARIABLES
==================================================================================================*/
#define PORT_START_SEC_VAR_INIT_UNSPECIFIED
/* #include "MemMap.h" */ /* Or Port_MemMap.h */

/** @brief Pointer to the current Port driver configuration set. Initialized by Port_Init(). */
STATIC P2CONST(Port_ConfigType, PORT_VAR, PORT_APPL_CONST) Port_pxCurrentConfig = NULL_PTR;

#define PORT_STOP_SEC_VAR_INIT_UNSPECIFIED
/* #include "MemMap.h" */

#define PORT_START_SEC_VAR_CLEARED_UNSPECIFIED
/* #include "MemMap.h" */

/** @brief Status of the Port driver initialization. */
STATIC VAR(boolean, PORT_VAR) Port_bIsInitialized = FALSE;

#define PORT_STOP_SEC_VAR_CLEARED_UNSPECIFIED
/* #include "MemMap.h" */


/*==================================================================================================
*                                   INTERNAL HELPER FUNCTIONS
==================================================================================================*/
#define PORT_START_SEC_CODE
/* #include "MemMap.h" */

/**
 * @brief Retrieves the base address of the GPIO port for a given Port_PinType ID.
 * @param[in] PinId The ID of the Port pin (0-95 for PA0-PF15, as per Port_Cfg.h).
 * @return GPIO_TypeDef* Pointer to the GPIO port registers, or NULL_PTR if PinId implies an invalid port index.
 */
STATIC FUNC(P2VAR(GPIO_TypeDef, AUTOMATIC, PORT_CODE), PORT_CODE) Port_Internal_GetPortBaseAddr(VAR(Port_PinType, AUTOMATIC) PinId)
{
    VAR(uint8, AUTOMATIC) u8PortIndex = (uint8)(PinId / PORT_PINS_PER_GPIO_PORT);
    P2VAR(GPIO_TypeDef, AUTOMATIC, PORT_APPL_DATA) pxGpioPort = NULL_PTR;

    switch(u8PortIndex)
    {
        case 0U: pxGpioPort = GPIOA; break; /* Corresponds to PORT_PIN_PAx */
        case 1U: pxGpioPort = GPIOB; break; /* Corresponds to PORT_PIN_PBx */
        case 2U: pxGpioPort = GPIOC; break; /* Corresponds to PORT_PIN_PCx */
        case 3U: pxGpioPort = GPIOD; break; /* Corresponds to PORT_PIN_PDx */
        case 4U: pxGpioPort = GPIOE; break; /* Corresponds to PORT_PIN_PEx */
        case 5U: pxGpioPort = GPIOF; break; /* Corresponds to PORT_PIN_PFx */
        /* Add cases for GPIOG, GPIOH if supported and configured */
        default:
            /* Invalid PortIndex derived from PinId. */
            /* This case should ideally not be reached if PinId is validated against PORT_MAX_PIN_ID by the caller. */
            break;
    }
    return pxGpioPort;
}

/**
 * @brief Returns the pin index (0-15) within a port for a given Port_PinType ID.
 * @param[in] PinId  The ID of the Port pin.
 * @return uint8 The pin index (0-15).
 */
STATIC FUNC(uint8, PORT_CODE) Port_Internal_GetPinIndex(VAR(Port_PinType, AUTOMATIC) PinId)
{
    return (uint8)(PinId % PORT_PINS_PER_GPIO_PORT);
}

/**
 * @brief Applies the configuration for a single pin to the hardware registers.
 * @param[in] pxPinCfg Pointer to the pin's configuration data from the post-build structure.
 */
STATIC FUNC(void, PORT_CODE) Port_Internal_ApplyPinConfiguration(
    P2CONST(Port_PinConfigType, AUTOMATIC, PORT_APPL_CONST) pxPinCfg
)
{
    P2VAR(GPIO_TypeDef, AUTOMATIC, PORT_APPL_DATA) pxPortBaseAddr;
    VAR(uint8, AUTOMATIC) u8PinIndex;
    VAR(uint32, AUTOMATIC) u32TempRegVal;

    pxPortBaseAddr = Port_Internal_GetPortBaseAddr(pxPinCfg->PinId);
    u8PinIndex = Port_Internal_GetPinIndex(pxPinCfg->PinId);

    if (NULL_PTR == pxPortBaseAddr)
    {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_API_ID, PORT_E_PARAM_PIN);
#endif
        return;
    }

    /* 1. Configure Pin Mode (Input, Output, AF, Analog) using MODER */
    u32TempRegVal = pxPortBaseAddr->MODER;
    u32TempRegVal &= ~(PORT_GPIO_MODE_BITS_MASK << (u8PinIndex * 2U));
    switch (pxPinCfg->PinMode)
    {
        case PORT_PIN_MODE_GPIO_IN_ANALOG:   u32TempRegVal |= (0x03UL << (u8PinIndex * 2U)); break;
        case PORT_PIN_MODE_GPIO_IN_FLOATING: /* Fall-through intended */
        case PORT_PIN_MODE_GPIO_IN_PULLDOWN: /* Fall-through intended */
        case PORT_PIN_MODE_GPIO_IN_PULLUP:   u32TempRegVal |= (0x00UL << (u8PinIndex * 2U)); break;
        case PORT_PIN_MODE_GPIO_OUT_PUSHPULL:/* Fall-through intended */
        case PORT_PIN_MODE_GPIO_OUT_OPENDRAIN:u32TempRegVal |= (0x01UL << (u8PinIndex * 2U)); break;
        case PORT_PIN_MODE_AF_PUSHPULL:      /* Fall-through intended */
        case PORT_PIN_MODE_AF_OPENDRAIN:     u32TempRegVal |= (0x02UL << (u8PinIndex * 2U)); break;
        case PORT_PIN_MODE_NOT_USED:         /* Fall-through intended */
        default:                             u32TempRegVal |= (0x03UL << (u8PinIndex * 2U)); /* Default to Analog for NOT_USED or error */
#if (PORT_DEV_ERROR_DETECT == STD_ON)
            if (pxPinCfg->PinMode != PORT_PIN_MODE_NOT_USED) { /* Report error only if mode was not intentionally NOT_USED */
                 Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_API_ID, PORT_E_PARAM_INVALID_MODE);
            }
#endif
            break;
    }
    pxPortBaseAddr->MODER = u32TempRegVal;

    /* 2. Configure Output Type (Push-pull or Open-drain) using OTYPER */
    if ((pxPinCfg->PinMode == PORT_PIN_MODE_GPIO_OUT_OPENDRAIN) || (pxPinCfg->PinMode == PORT_PIN_MODE_AF_OPENDRAIN))
    {   pxPortBaseAddr->OTYPER |= (1UL << u8PinIndex); }
    else if ((pxPinCfg->PinMode == PORT_PIN_MODE_GPIO_OUT_PUSHPULL) || (pxPinCfg->PinMode == PORT_PIN_MODE_AF_PUSHPULL))
    {   pxPortBaseAddr->OTYPER &= ~(1UL << u8PinIndex); }

    /* 3. Configure Output Speed using OSPEEDR */
    if ((pxPinCfg->PinMode == PORT_PIN_MODE_GPIO_OUT_PUSHPULL) || (pxPinCfg->PinMode == PORT_PIN_MODE_GPIO_OUT_OPENDRAIN) ||
        (pxPinCfg->PinMode == PORT_PIN_MODE_AF_PUSHPULL) || (pxPinCfg->PinMode == PORT_PIN_MODE_AF_OPENDRAIN))
    {
        u32TempRegVal = pxPortBaseAddr->OSPEEDR;
        u32TempRegVal &= ~(PORT_GPIO_MODE_BITS_MASK << (u8PinIndex * 2U));
        u32TempRegVal |= ((uint32)pxPinCfg->PinOutputSpeedCfg << (u8PinIndex * 2U));
        pxPortBaseAddr->OSPEEDR = u32TempRegVal;
    }

    /* 4. Configure Pull-up/Pull-down Resistors using PUPDR */
    u32TempRegVal = pxPortBaseAddr->PUPDR;
    u32TempRegVal &= ~(PORT_GPIO_MODE_BITS_MASK << (u8PinIndex * 2U));
    if (pxPinCfg->PinPullCfg == PORT_PULL_UP) { u32TempRegVal |= (0x01UL << (u8PinIndex * 2U)); }
    else if (pxPinCfg->PinPullCfg == PORT_PULL_DOWN) { u32TempRegVal |= (0x02UL << (u8PinIndex * 2U)); }
    pxPortBaseAddr->PUPDR = u32TempRegVal;

    /* 5. Configure Alternate Function using AFR registers */
    if ((pxPinCfg->PinMode == PORT_PIN_MODE_AF_PUSHPULL) || (pxPinCfg->PinMode == PORT_PIN_MODE_AF_OPENDRAIN))
    {
        VAR(uint8, AUTOMATIC) u8AfRegisterIndex = (u8PinIndex < 8U) ? 0U : 1U;
        VAR(uint8, AUTOMATIC) u8AfPinPosition   = (u8PinIndex < 8U) ? u8PinIndex : (u8PinIndex - 8U);
        u32TempRegVal = pxPortBaseAddr->AFR[u8AfRegisterIndex];
        u32TempRegVal &= ~(PORT_GPIO_AF_BITS_MASK << (u8AfPinPosition * 4U));
        u32TempRegVal |= ((uint32)pxPinCfg->PinAlternateFunction << (u8AfPinPosition * 4U));
        pxPortBaseAddr->AFR[u8AfRegisterIndex] = u32TempRegVal;
    }

    /* 6. Set Initial Output Level (if configured as GPIO output) */
    if ((pxPinCfg->PinMode == PORT_PIN_MODE_GPIO_OUT_PUSHPULL) || (pxPinCfg->PinMode == PORT_PIN_MODE_GPIO_OUT_OPENDRAIN))
    {
        if (pxPinCfg->PinInitialLevel == PORT_PIN_LEVEL_HIGH) { pxPortBaseAddr->BSRR = (1UL << u8PinIndex); }
        else { pxPortBaseAddr->BSRR = (1UL << (u8PinIndex + 16U)); }
    }
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

FUNC(void, PORT_CODE) Port_Init(P2CONST(Port_ConfigType, AUTOMATIC, PORT_APPL_CONST) ConfigPtr)
{
    VAR(Port_PinType, AUTOMATIC) i;

#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if (Port_bIsInitialized == TRUE) {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_API_ID, PORT_E_ALREADY_INITIALIZED); /* Custom Error */
        return; /* Added return after DET report */
    }
    if (NULL_PTR == ConfigPtr) {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_API_ID, PORT_E_PARAM_CONFIG);
        return;
    }
    if ((NULL_PTR == ConfigPtr->PinConfig) && (ConfigPtr->NumberOfPins > 0U)) {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_API_ID, PORT_E_PARAM_CONFIG);
        return;
    }
    if (ConfigPtr->NumberOfPins > PORT_NUMBER_OF_PINS) { /* PORT_NUMBER_OF_PINS from Port_Cfg.h */
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_API_ID, PORT_E_PARAM_CONFIG);
        return;
    }
#endif

    Port_pxCurrentConfig = ConfigPtr;

    for (i = 0U; i < Port_pxCurrentConfig->NumberOfPins; ++i) {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
        if (Port_pxCurrentConfig->PinConfig[i].PinId > PORT_MAX_PIN_ID) { /* PORT_MAX_PIN_ID from Port_Cfg.h */
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_API_ID, PORT_E_PARAM_PIN);
            continue;
        }
#endif
        Port_Internal_ApplyPinConfiguration(&(Port_pxCurrentConfig->PinConfig[i]));
    }
    Port_bIsInitialized = TRUE;
}


#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
FUNC(void, PORT_CODE) Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction)
{
    P2CONST(Port_PinConfigType, AUTOMATIC, PORT_APPL_CONST) pxPinCfg = NULL_PTR;
    P2VAR(GPIO_TypeDef, AUTOMATIC, PORT_APPL_DATA) pxPortBaseAddr;
    VAR(uint8, AUTOMATIC) u8PinIndex;
    VAR(Port_PinType, AUTOMATIC) i;
    VAR(boolean, AUTOMATIC) bPinFound = FALSE;

#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if (Port_bIsInitialized == FALSE) {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_API_ID, PORT_E_UNINIT);
        return;
    }
    if (Pin > PORT_MAX_PIN_ID) {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_API_ID, PORT_E_PARAM_PIN);
        return;
    }
    if (!((Direction == PORT_PIN_IN) || (Direction == PORT_PIN_OUT))) {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_API_ID, PORT_E_PARAM_INVALID_MODE); /* Reusing for invalid direction */
        return;
    }
#endif

    if (NULL_PTR != Port_pxCurrentConfig) {
        for (i = 0U; i < Port_pxCurrentConfig->NumberOfPins; ++i) {
            if (Port_pxCurrentConfig->PinConfig[i].PinId == Pin) {
                pxPinCfg = &(Port_pxCurrentConfig->PinConfig[i]);
                bPinFound = TRUE;
                break;
            }
        }
    }

#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if (bPinFound == FALSE) {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_API_ID, PORT_E_PARAM_PIN);
        return;
    }
    if (!((PORT_PIN_DIRECTION_CHANGEABLE == STD_ON) && (pxPinCfg->PinDirectionChangeable == TRUE))) { /* Check global then pin specific */
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_API_ID, PORT_E_DIRECTION_UNCHANGEABLE);
        return;
    }
#else
    if (!((bPinFound == TRUE) && (PORT_PIN_DIRECTION_CHANGEABLE == STD_ON) && (pxPinCfg->PinDirectionChangeable == TRUE))) {
        return; /* Silently return if DET is off and conditions not met */
    }
#endif

    pxPortBaseAddr = Port_Internal_GetPortBaseAddr(Pin);
    u8PinIndex = Port_Internal_GetPinIndex(Pin);

    if (NULL_PTR != pxPortBaseAddr) {
        VAR(uint32, AUTOMATIC) u32TempModer = pxPortBaseAddr->MODER;
        u32TempModer &= ~(PORT_GPIO_MODE_BITS_MASK << (u8PinIndex * 2U));
        if (Direction == PORT_PIN_OUT) { u32TempModer |= (0x01UL << (u8PinIndex * 2U)); }
        else { u32TempModer |= (0x00UL << (u8PinIndex * 2U)); }
        pxPortBaseAddr->MODER = u32TempModer;
    }
}
#endif /* (PORT_SET_PIN_DIRECTION_API == STD_ON) */


FUNC(void, PORT_CODE) Port_RefreshPortDirection(void)
{
    VAR(Port_PinType, AUTOMATIC) i;
    P2CONST(Port_PinConfigType, AUTOMATIC, PORT_APPL_CONST) pxCurrentPinConfig;

#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if (Port_bIsInitialized == FALSE) {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_REFRESH_PORT_DIRECTION_API_ID, PORT_E_UNINIT);
        return;
    }
#endif
    if (NULL_PTR == Port_pxCurrentConfig) {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_REFRESH_PORT_DIRECTION_API_ID, PORT_E_UNINIT);
#endif
        return;
    }

    for (i = 0U; i < Port_pxCurrentConfig->NumberOfPins; ++i) {
        pxCurrentPinConfig = &(Port_pxCurrentConfig->PinConfig[i]);
        if (pxCurrentPinConfig->PinDirectionChangeable == FALSE) {
            P2VAR(GPIO_TypeDef, AUTOMATIC, PORT_APPL_DATA) pxPortBaseAddr = Port_Internal_GetPortBaseAddr(pxCurrentPinConfig->PinId);
            VAR(uint8, AUTOMATIC) u8PinIndex = Port_Internal_GetPinIndex(pxCurrentPinConfig->PinId);
            VAR(uint32, AUTOMATIC) u32TempRegVal;
            if (NULL_PTR != pxPortBaseAddr) {
                u32TempRegVal = pxPortBaseAddr->MODER;
                u32TempRegVal &= ~(PORT_GPIO_MODE_BITS_MASK << (u8PinIndex * 2U));
                switch (pxCurrentPinConfig->PinMode) {
                    case PORT_PIN_MODE_GPIO_IN_ANALOG:   u32TempRegVal |= (0x03UL << (u8PinIndex * 2U)); break;
                    case PORT_PIN_MODE_GPIO_IN_FLOATING: /* Fall-through */
                    case PORT_PIN_MODE_GPIO_IN_PULLDOWN: /* Fall-through */
                    case PORT_PIN_MODE_GPIO_IN_PULLUP:   u32TempRegVal |= (0x00UL << (u8PinIndex * 2U)); break;
                    case PORT_PIN_MODE_GPIO_OUT_PUSHPULL:/* Fall-through */
                    case PORT_PIN_MODE_GPIO_OUT_OPENDRAIN:u32TempRegVal |= (0x01UL << (u8PinIndex * 2U)); break;
                    case PORT_PIN_MODE_AF_PUSHPULL:      /* Fall-through */
                    case PORT_PIN_MODE_AF_OPENDRAIN:     u32TempRegVal |= (0x02UL << (u8PinIndex * 2U)); break;
                    case PORT_PIN_MODE_NOT_USED:         /* Fall-through */
                    default:                             u32TempRegVal |= (0x03UL << (u8PinIndex * 2U)); break; /* Analog for safety */
                }
                pxPortBaseAddr->MODER = u32TempRegVal;
            }
        }
    }
}


#if (PORT_SET_PIN_MODE_API == STD_ON)
FUNC(void, PORT_CODE) Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode)
{
    P2CONST(Port_PinConfigType, AUTOMATIC, PORT_APPL_CONST) pxPinCfgOriginal = NULL_PTR;
    VAR(Port_PinConfigType, AUTOMATIC) TempPinConfig;
    VAR(Port_PinType, AUTOMATIC) i;
    VAR(boolean, AUTOMATIC) bPinFound = FALSE;

#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if (Port_bIsInitialized == FALSE) {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_API_ID, PORT_E_UNINIT);
        return;
    }
    if (Pin > PORT_MAX_PIN_ID) {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_API_ID, PORT_E_PARAM_PIN);
        return;
    }
    if (Mode >= PORT_PIN_MODE_NOT_USED ) { /* Assuming NOT_USED is largest valid enum for this check logic */
         Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_API_ID, PORT_E_PARAM_INVALID_MODE);
        return;
    }
#endif

    if (NULL_PTR != Port_pxCurrentConfig) {
        for (i = 0U; i < Port_pxCurrentConfig->NumberOfPins; ++i) {
            if (Port_pxCurrentConfig->PinConfig[i].PinId == Pin) {
                pxPinCfgOriginal = &(Port_pxCurrentConfig->PinConfig[i]);
                bPinFound = TRUE;
                break;
            }
        }
    }

#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if (bPinFound == FALSE) {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_API_ID, PORT_E_PARAM_PIN);
        return;
    }
    if (!((PORT_PIN_MODE_CHANGEABLE == STD_ON) && (pxPinCfgOriginal->PinModeChangeable == TRUE))) {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_API_ID, PORT_E_MODE_UNCHANGEABLE);
        return;
    }
#else
    if (!((bPinFound == TRUE) && (PORT_PIN_MODE_CHANGEABLE == STD_ON) && (pxPinCfgOriginal->PinModeChangeable == TRUE))) {
        return;
    }
#endif

    TempPinConfig = *pxPinCfgOriginal;
    TempPinConfig.PinMode = Mode;
    /* PinAlternateFunction is taken from original config if new mode is AF. */
    /* PinDirection is implicitly handled by PinMode in ApplyPinConfiguration. */
    Port_Internal_ApplyPinConfiguration(&TempPinConfig);
}
#endif /* (PORT_SET_PIN_MODE_API == STD_ON) */


#if (PORT_VERSION_INFO_API == STD_ON)
FUNC(void, PORT_CODE) Port_GetVersionInfo(
    P2VAR(Std_VersionInfoType, AUTOMATIC, PORT_APPL_DATA) VersionInfoPtr
)
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == VersionInfoPtr) {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_GET_VERSION_INFO_API_ID, PORT_E_PARAM_POINTER);
    } else
#endif
    {
        VersionInfoPtr->vendorID         = PORT_VENDOR_ID;
        VersionInfoPtr->moduleID         = PORT_MODULE_ID;
        VersionInfoPtr->sw_major_version = PORT_SW_MAJOR_VERSION;
        VersionInfoPtr->sw_minor_version = PORT_SW_MINOR_VERSION;
        VersionInfoPtr->sw_patch_version = PORT_SW_PATCH_VERSION;
    }
}
#endif /* (PORT_VERSION_INFO_API == STD_ON) */

#define PORT_STOP_SEC_CODE
/* #include "MemMap.h" */ /* Or Port_MemMap.h */
