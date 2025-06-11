/*==================================================================================================
*   Project              : AUTOSAR 4.x MCAL
*   Platform             : STM32
*   Peripheral           : GPIO
*   Dependencies         : STM32F3xx Series
*
*   Autosar Version      : 4.3.1
*   Autosar Revision     : ASR_REL_4_3_REV_0001
*   Sw Version           : 1.0.0
*   Build Version        : PORT_MCAL_1_0_0_RTM_ASR_REL_4_3_REV_0001_2024xxxx
*
*   (c) Copyright 2023-2024 YourName
*   All Rights Reserved.
==================================================================================================*/

#include "../Include/Port.h"       /* Main Port driver header (includes Port_Cfg.h, Std_Types.h) */
#include "../Include/Port_Regs.h"  /* For direct register access (includes stm32f3xx.h) */
#include "../../Common/Include/Mcal.h" /* For STATIC, FUNC, VAR macros (via Compiler.h) */


#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* Include Det.h only if PORT_DEV_ERROR_DETECT is STD_ON */
    #include "../../Common/Include/Det.h"
#endif

/*==================================================================================================
*                                       VERSION CHECKS
==================================================================================================*/
/* Define this file's version numbers (example) */
#define PORT_SW_MAJOR_VERSION_C     1
#define PORT_SW_MINOR_VERSION_C     0
#define PORT_SW_PATCH_VERSION_C     0

#define PORT_AR_RELEASE_MAJOR_VERSION_C  4
#define PORT_AR_RELEASE_MINOR_VERSION_C  3
#define PORT_AR_RELEASE_PATCH_VERSION_C  1

/* Check against Port.h version numbers (assuming these are defined in Port.h) */
#if ((PORT_SW_MAJOR_VERSION_C != PORT_SW_MAJOR_VERSION) ||  \
     (PORT_SW_MINOR_VERSION_C != PORT_SW_MINOR_VERSION) ||  \
     (PORT_SW_PATCH_VERSION_C != PORT_SW_PATCH_VERSION))
  #error "Software Version Numbers of Port.c and Port.h are different"
#endif

#if ((PORT_AR_RELEASE_MAJOR_VERSION_C != PORT_AR_RELEASE_MAJOR_VERSION) || \
     (PORT_AR_RELEASE_MINOR_VERSION_C != PORT_AR_RELEASE_MINOR_VERSION) || \
     (PORT_AR_RELEASE_PATCH_VERSION_C != PORT_AR_RELEASE_PATCH_VERSION))
  #error "AUTOSAR Version Numbers of Port.c and Port.h are different"
#endif

/*==================================================================================================
*                                          VARIABLES
==================================================================================================*/

STATIC P2CONST(Port_ConfigType, PORT_VAR, PORT_APPL_CONST) Port_kConfigPtr = NULL_PTR;
STATIC VAR(boolean, PORT_VAR) Port_IsInitialized = FALSE;

/*==================================================================================================
*                                   INTERNAL HELPER FUNCTIONS
==================================================================================================*/

/**
 * @brief Returns the GPIO peripheral base address for a given Port_PinType.
 * @param[in] PinId  The ID of the Port pin.
 * @return GPIO_TypeDef* Pointer to the GPIO peripheral, or NULL_PTR if invalid.
 */
STATIC FUNC(GPIO_TypeDef*, PORT_CODE) Port_GetPortBaseAddr(Port_PinType PinId)
{
    VAR(uint8, AUTOMATIC) portIndex = (uint8)(PinId / 16U); /* PA=0, PB=1, ... PF=5 */
    P2VAR(GPIO_TypeDef, AUTOMATIC, PORT_APPL_DATA) GpioPort = NULL_PTR;

    switch(portIndex)
    {
        case 0: GpioPort = GPIOA; break;
        case 1: GpioPort = GPIOB; break;
        case 2: GpioPort = GPIOC; break;
        case 3: GpioPort = GPIOD; break;
        case 4: GpioPort = GPIOE; break;
        case 5: GpioPort = GPIOF; break;
        default:
#if (PORT_DEV_ERROR_DETECT == STD_ON)
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INTERNAL_API_ID, PORT_E_PARAM_PIN_INTERNAL);
#endif
            break;
    }
    return GpioPort;
}

/**
 * @brief Returns the pin index (0-15) within a port for a given Port_PinType.
 * @param[in] PinId  The ID of the Port pin.
 * @return uint8 The pin index.
 */
STATIC FUNC(uint8, PORT_CODE) Port_GetPinIndex(Port_PinType PinId)
{
    return (uint8)(PinId % 16U);
}


/**
 * @brief Applies the configuration for a single pin.
 * @details Helper function for Port_Init and potentially Port_SetPinMode.
 * @param[in] PinCfg Pointer to the pin's configuration data.
 */
STATIC FUNC(void, PORT_CODE) Port_ApplyPinConfiguration(P2CONST(Port_PinConfigType, AUTOMATIC, PORT_APPL_CONST) PinCfg)
{
    P2VAR(GPIO_TypeDef, AUTOMATIC, PORT_APPL_DATA) portBaseAddr;
    VAR(uint8, AUTOMATIC) pinIndex;
    VAR(uint32, AUTOMATIC) tempRegVal;

    portBaseAddr = Port_GetPortBaseAddr(PinCfg->PinId);
    pinIndex = Port_GetPinIndex(PinCfg->PinId);

    if (NULL_PTR == portBaseAddr) {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_ID, PORT_E_PARAM_PIN);
#endif
        return;
    }

    /* 1. Configure Pin Mode (Input, Output, AF, Analog) using MODER */
    tempRegVal = portBaseAddr->MODER;
    tempRegVal &= ~(3UL << (pinIndex * 2)); /* Clear current mode bits */

    if (PinCfg->PinMode == PORT_PIN_MODE_GPIO_IN_ANALOG) {
        tempRegVal |= (3UL << (pinIndex * 2)); /* Analog */
    } else if ( (PinCfg->PinMode == PORT_PIN_MODE_GPIO_IN_FLOATING) ||
                (PinCfg->PinMode == PORT_PIN_MODE_GPIO_IN_PULLUP)   ||
                (PinCfg->PinMode == PORT_PIN_MODE_GPIO_IN_PULLDOWN) ) {
        tempRegVal |= (0UL << (pinIndex * 2)); /* Input */
    } else if ( (PinCfg->PinMode == PORT_PIN_MODE_GPIO_OUT_PUSHPULL) ||
                (PinCfg->PinMode == PORT_PIN_MODE_GPIO_OUT_OPENDRAIN) ) {
        tempRegVal |= (1UL << (pinIndex * 2)); /* Output */
    } else if ( (PinCfg->PinMode == PORT_PIN_MODE_AF_PUSHPULL) ||
                (PinCfg->PinMode == PORT_PIN_MODE_AF_OPENDRAIN) ) {
        tempRegVal |= (2UL << (pinIndex * 2)); /* Alternate Function */
    } else {
        /* PORT_PIN_MODE_NOT_USED or invalid - typically leave as input floating or as per reset state */
        /* Or report error if an invalid mode was passed for configuration */
#if (PORT_DEV_ERROR_DETECT == STD_ON)
        if(PinCfg->PinMode != PORT_PIN_MODE_NOT_USED) {
             Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_ID, PORT_E_PARAM_INVALID_MODE);
        }
#endif
         /* Default to input floating if not used, or do nothing to keep reset state */
        tempRegVal |= (0UL << (pinIndex * 2)); /* Defaulting to Input */
    }
    portBaseAddr->MODER = tempRegVal;

    /* 2. Configure Output Type (Push-pull or Open-drain) using OTYPER */
    if ( (PinCfg->PinMode == PORT_PIN_MODE_GPIO_OUT_OPENDRAIN) ||
         (PinCfg->PinMode == PORT_PIN_MODE_AF_OPENDRAIN) ) {
        portBaseAddr->OTYPER |= (1UL << pinIndex); /* Open-drain */
    } else if ( (PinCfg->PinMode == PORT_PIN_MODE_GPIO_OUT_PUSHPULL) ||
                (PinCfg->PinMode == PORT_PIN_MODE_AF_PUSHPULL) ) {
        portBaseAddr->OTYPER &= ~(1UL << pinIndex); /* Push-pull */
    }
    /* For input modes, OTYPER is don't care. */

    /* 3. Configure Output Speed (Low, Medium, High, Very High) using OSPEEDR */
    if ( (PinCfg->PinMode == PORT_PIN_MODE_GPIO_OUT_PUSHPULL) ||
         (PinCfg->PinMode == PORT_PIN_MODE_GPIO_OUT_OPENDRAIN) ||
         (PinCfg->PinMode == PORT_PIN_MODE_AF_PUSHPULL)       ||
         (PinCfg->PinMode == PORT_PIN_MODE_AF_OPENDRAIN) ) {

        tempRegVal = portBaseAddr->OSPEEDR;
        tempRegVal &= ~(3UL << (pinIndex * 2)); /* Clear current speed bits */
        tempRegVal |= ((uint32)PinCfg->PinOutputSpeedCfg << (pinIndex * 2));
        portBaseAddr->OSPEEDR = tempRegVal;
    }
    /* For input modes, OSPEEDR is don't care. */

    /* 4. Configure Pull-up/Pull-down Resistors using PUPDR */
    tempRegVal = portBaseAddr->PUPDR;
    tempRegVal &= ~(3UL << (pinIndex * 2)); /* Clear current pull config bits */
    if (PinCfg->PinPullCfg == PORT_PULL_UP) {
        tempRegVal |= (1UL << (pinIndex * 2)); /* Pull-up */
    } else if (PinCfg->PinPullCfg == PORT_PULL_DOWN) {
        tempRegVal |= (2UL << (pinIndex * 2)); /* Pull-down */
    } else {
        /* PORT_PULL_NONE - No pull (00), already cleared */
    }
    portBaseAddr->PUPDR = tempRegVal;

    /* 5. Configure Alternate Function (AF0-AF15) using AFR registers */
    if ( (PinCfg->PinMode == PORT_PIN_MODE_AF_PUSHPULL) ||
         (PinCfg->PinMode == PORT_PIN_MODE_AF_OPENDRAIN) ) {
        VAR(uint8, AUTOMATIC) afRegisterIndex = (pinIndex < 8U) ? 0U : 1U; /* AFR[0] or AFR[1] */
        VAR(uint8, AUTOMATIC) afPinIndexInReg = (pinIndex < 8U) ? pinIndex : (pinIndex - 8U);

        tempRegVal = portBaseAddr->AFR[afRegisterIndex];
        tempRegVal &= ~(0xFUL << (afPinIndexInReg * 4)); /* Clear 4 bits for AF selection */
        tempRegVal |= ((uint32)PinCfg->PinAlternateFunction << (afPinIndexInReg * 4));
        portBaseAddr->AFR[afRegisterIndex] = tempRegVal;
    }

    /* 6. Set Initial Output Level using BSRR (for atomic set/reset) or ODR */
    if ( (PinCfg->PinMode == PORT_PIN_MODE_GPIO_OUT_PUSHPULL) ||
         (PinCfg->PinMode == PORT_PIN_MODE_GPIO_OUT_OPENDRAIN) ) {
        if (PinCfg->PinInitialLevel == PORT_PIN_LEVEL_HIGH) {
            portBaseAddr->BSRR = (1UL << pinIndex); /* Set pin high */
        } else {
            portBaseAddr->BSRR = (1UL << (pinIndex + 16U)); /* Set pin low (reset) */
        }
    }
}


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

FUNC(void, PORT_CODE) Port_Init(P2CONST(Port_ConfigType, AUTOMATIC, PORT_APPL_CONST) ConfigPtr)
{
    VAR(uint16, AUTOMATIC) i; /* Use uint16 for loop if NumPins can exceed 255 */

#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == ConfigPtr) {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_ID, PORT_E_PARAM_CONFIG);
        return;
    }
    if ((NULL_PTR == ConfigPtr->PinConfig) && (ConfigPtr->NumberOfPins > 0U)) {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_ID, PORT_E_PARAM_CONFIG);
        return;
    }
#endif

    Port_kConfigPtr = ConfigPtr;

    for (i = 0U; i < Port_kConfigPtr->NumberOfPins; ++i)
    {
        Port_ApplyPinConfiguration(&(Port_kConfigPtr->PinConfig[i]));
    }
    Port_IsInitialized = TRUE;
}


#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
FUNC(void, PORT_CODE) Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction)
{
    P2CONST(Port_PinConfigType, AUTOMATIC, PORT_APPL_CONST) pinCfg = NULL_PTR;
    P2VAR(GPIO_TypeDef, AUTOMATIC, PORT_APPL_DATA) portBaseAddr;
    VAR(uint8, AUTOMATIC) pinIndex;
    VAR(uint16, AUTOMATIC) i;

#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if (FALSE == Port_IsInitialized) {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SETPINDIRECTION_ID, PORT_E_UNINIT);
        return;
    }
    /* Find the pin configuration */
    for(i = 0U; i < Port_kConfigPtr->NumberOfPins; ++i) {
        if(Port_kConfigPtr->PinConfig[i].PinId == Pin) {
            pinCfg = &Port_kConfigPtr->PinConfig[i];
            break;
        }
    }
    if (NULL_PTR == pinCfg) {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SETPINDIRECTION_ID, PORT_E_PARAM_PIN);
        return;
    }
    if (FALSE == pinCfg->PinDirectionChangeable) {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SETPINDIRECTION_ID, PORT_E_DIRECTION_UNCHANGEABLE);
        return;
    }
#else /* If DET is OFF, still need to find the config if PinDirectionChangeable is a PB feature */
    for(i = 0U; i < Port_kConfigPtr->NumberOfPins; ++i) {
        if(Port_kConfigPtr->PinConfig[i].PinId == Pin) {
            pinCfg = &Port_kConfigPtr->PinConfig[i];
            break;
        }
    }
    if (NULL_PTR == pinCfg || FALSE == pinCfg->PinDirectionChangeable) {
        /* Pin not found or not changeable, silently return or handle as per non-DET behavior */
        return;
    }
#endif

    portBaseAddr = Port_GetPortBaseAddr(Pin);
    pinIndex = Port_GetPinIndex(Pin);

    if (NULL_PTR != portBaseAddr) {
        VAR(uint32, AUTOMATIC) tempModer = portBaseAddr->MODER;
        tempModer &= ~(3UL << (pinIndex * 2)); /* Clear current mode bits */
        if (Direction == PORT_PIN_OUT) {
            tempModer |= (1UL << (pinIndex * 2)); /* General purpose output mode */
        } else { /* PORT_PIN_IN */
            tempModer |= (0UL << (pinIndex * 2)); /* Input mode */
        }
        portBaseAddr->MODER = tempModer;
        /* Note: This simplified version changes mode to basic GPIO In/Out.
         * A full implementation would need to consider the existing PinMode (AF, Analog)
         * and how changing direction interacts with those. Often, changing direction
         * is only meaningful for pins already in GPIO mode.
         */
    }
}
#endif /* (PORT_SET_PIN_DIRECTION_API == STD_ON) */


FUNC(void, PORT_CODE) Port_RefreshPortDirection(void)
{
    VAR(uint16, AUTOMATIC) i;
    P2CONST(Port_PinConfigType, AUTOMATIC, PORT_APPL_CONST) currentPinConfig;

#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if (FALSE == Port_IsInitialized) {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_REFRESHPORTDIRECTION_ID, PORT_E_UNINIT);
        return;
    }
#endif

    for (i = 0U; i < Port_kConfigPtr->NumberOfPins; ++i)
    {
        currentPinConfig = &(Port_kConfigPtr->PinConfig[i]);
        if (FALSE == currentPinConfig->PinDirectionChangeable)
        {
            /* Re-apply the direction part of the mode for non-changeable pins */
            /* This re-uses parts of Port_ApplyPinConfiguration logic for MODER */
            P2VAR(GPIO_TypeDef, AUTOMATIC, PORT_APPL_DATA) portBaseAddr = Port_GetPortBaseAddr(currentPinConfig->PinId);
            VAR(uint8, AUTOMATIC) pinIndex = Port_GetPinIndex(currentPinConfig->PinId);
            VAR(uint32, AUTOMATIC) tempRegVal;

            if (NULL_PTR != portBaseAddr) {
                tempRegVal = portBaseAddr->MODER;
                tempRegVal &= ~(3UL << (pinIndex * 2));

                if (currentPinConfig->PinMode == PORT_PIN_MODE_GPIO_IN_ANALOG) tempRegVal |= (3UL << (pinIndex * 2));
                else if ((currentPinConfig->PinMode == PORT_PIN_MODE_GPIO_IN_FLOATING) || \
                         (currentPinConfig->PinMode == PORT_PIN_MODE_GPIO_IN_PULLUP)   || \
                         (currentPinConfig->PinMode == PORT_PIN_MODE_GPIO_IN_PULLDOWN) ) tempRegVal |= (0UL << (pinIndex * 2));
                else if ((currentPinConfig->PinMode == PORT_PIN_MODE_GPIO_OUT_PUSHPULL) || \
                         (currentPinConfig->PinMode == PORT_PIN_MODE_GPIO_OUT_OPENDRAIN) ) tempRegVal |= (1UL << (pinIndex * 2));
                else if ((currentPinConfig->PinMode == PORT_PIN_MODE_AF_PUSHPULL) || \
                         (currentPinConfig->PinMode == PORT_PIN_MODE_AF_OPENDRAIN) ) tempRegVal |= (2UL << (pinIndex * 2));
                else { /* Not used or invalid - default to input */ tempRegVal |= (0UL << (pinIndex * 2)); }
                portBaseAddr->MODER = tempRegVal;
            }
        }
    }
}


#if (PORT_SET_PIN_MODE_API == STD_ON)
FUNC(void, PORT_CODE) Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode)
{
    P2CONST(Port_PinConfigType, AUTOMATIC, PORT_APPL_CONST) pinCfg = NULL_PTR;
    VAR(uint16, AUTOMATIC) i;
    /* Temporary config to apply the new mode. AF number would be an issue. */
    Port_PinConfigType tempPinConfig;

#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if (FALSE == Port_IsInitialized) {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SETPINMODE_ID, PORT_E_UNINIT);
        return;
    }
    for(i = 0U; i < Port_kConfigPtr->NumberOfPins; ++i) {
        if(Port_kConfigPtr->PinConfig[i].PinId == Pin) {
            pinCfg = &Port_kConfigPtr->PinConfig[i];
            break;
        }
    }
    if (NULL_PTR == pinCfg) {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SETPINMODE_ID, PORT_E_PARAM_PIN);
        return;
    }
    if (FALSE == pinCfg->PinModeChangeable) {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SETPINMODE_ID, PORT_E_MODE_UNCHANGEABLE);
        return;
    }
    /* Basic check for mode validity. A full check would be MCU-specific (e.g. valid AFs for a pin) */
    if (Mode > PORT_PIN_MODE_AF_OPENDRAIN && Mode < PORT_PIN_MODE_NOT_USED) { /* Assuming modes are somewhat ordered */
         Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SETPINMODE_ID, PORT_E_PARAM_INVALID_MODE);
        return;
    }
#else
    for(i = 0U; i < Port_kConfigPtr->NumberOfPins; ++i) {
        if(Port_kConfigPtr->PinConfig[i].PinId == Pin) {
            pinCfg = &Port_kConfigPtr->PinConfig[i];
            break;
        }
    }
    if (NULL_PTR == pinCfg || FALSE == pinCfg->PinModeChangeable) {
        return; /* Pin not found or not changeable */
    }
#endif

    /* Create a temporary config based on the original, then change the mode */
    tempPinConfig = *pinCfg; /* Copy original config */
    tempPinConfig.PinMode = Mode;

    /* If changing to an AF mode, the AF number must be known.
     * The standard API does not pass AF number. Common solutions:
     * 1. The 'Mode' parameter itself encodes AF number (e.g. PORT_MODE_AF0_SPI, PORT_MODE_AF1_UART). (Complex enum)
     * 2. This function only changes to a default AF or the one from initial config.
     * 3. A non-standard API is used: Port_SetPinAlternateFunction(Pin, AF_Number).
     * Here, we use the AF number from the *original* configuration if changing to AF.
     * This means runtime change to a *different* AF number is not supported by this call alone.
     */
    if( (Mode == PORT_PIN_MODE_AF_PUSHPULL || Mode == PORT_PIN_MODE_AF_OPENDRAIN) &&
        !(pinCfg->PinMode == PORT_PIN_MODE_AF_PUSHPULL || pinCfg->PinMode == PORT_PIN_MODE_AF_OPENDRAIN) ) {
        /* If changing *to* AF mode from a non-AF mode, PinAlternateFunction from original config is used. */
        /* If already in AF mode and just changing type (e.g. PP to OD), AF number is preserved. */
    }
    /* If changing *from* AF mode to non-AF, PinAlternateFunction in tempPinConfig is ignored by Port_ApplyPinConfiguration. */

    Port_ApplyPinConfiguration(&tempPinConfig);
}
#endif /* (PORT_SET_PIN_MODE_API == STD_ON) */


#if (PORT_VERSION_INFO_API == STD_ON)
FUNC(void, PORT_CODE) Port_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, PORT_APPL_DATA) VersionInfo)
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == VersionInfo) {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_GETVERSIONINFO_ID, PORT_E_PARAM_POINTER);
        return;
    }
#endif
    VersionInfo->vendorID         = PORT_VENDOR_ID;         /* From Port.h */
    VersionInfo->moduleID         = PORT_MODULE_ID;         /* From Port.h */
    VersionInfo->sw_major_version = PORT_SW_MAJOR_VERSION;  /* From Port.h */
    VersionInfo->sw_minor_version = PORT_SW_MINOR_VERSION;  /* From Port.h */
    VersionInfo->sw_patch_version = PORT_SW_PATCH_VERSION;  /* From Port.h */
}
#endif /* (PORT_VERSION_INFO_API == STD_ON) */
