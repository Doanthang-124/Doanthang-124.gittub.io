/*==================================================================================================
*   Project              : AUTOSAR 4.x MCAL
*   Platform             : STM32
*   Peripheral           : GPT
*   Dependencies         : STM32F3xx Series, CMSIS Core
*
*   Autosar Version      : 4.3.1
*   Autosar Revision     : ASR_REL_4_3_REV_0001
*   Sw Version           : 1.0.0
*   Build Version        : GPT_MCAL_1_0_0_RTM_ASR_REL_4_3_REV_0001_2024xxxx
*
==================================================================================================*/

#include "../Include/Gpt.h"            /* Main GPT header (includes Gpt_Cfg.h, Gpt_PBcfg.h, Std_Types.h) */
#include "../Include/Gpt_Hw_Stm32f3.h" /* Private HW access header (includes stm32f3xx.h) */

#if (GPT_DEV_ERROR_DETECT == STD_ON) /* From Gpt_Cfg.h */
    #include "../../Det/Include/Det.h"    /* DET header */
#endif

/* Include for CMSIS NVIC functions (NVIC_EnableIRQ, NVIC_DisableIRQ) and IRQn_Type.
 * This is typically included via the device specific header (e.g., stm32f303xc.h -> core_cm4.h),
 * which itself is included by our "stm32f3xx.h".
 * If these are not found, it indicates "stm32f3xx.h" or the toolchain setup is incomplete.
 */
#if (GPT_ENABLE_DISABLE_NOTIFICATION_API == STD_ON)
    /* Conditional include for CMSIS core functions if stm32f3xx.h does not guarantee it.
       For STM32F3, stm32f3xx.h typically includes stm32f303xc.h (or similar) which includes core_cm4.h.
       If NVIC_EnableIRQ is unresolved, ensure correct CMSIS headers are part of include path or stm32f3xx.h. */
    /* #include "core_cm4.h" */ /* Usually not needed if stm32f3xx.h is correct */
#endif


/*==================================================================================================
*                                       VERSION CHECKS
==================================================================================================*/
/* Define this file's version numbers for checks against Gpt.h */
#define GPT_SW_MAJOR_VERSION_C     GPT_SW_MAJOR_VERSION
#define GPT_SW_MINOR_VERSION_C     GPT_SW_MINOR_VERSION
#define GPT_SW_PATCH_VERSION_C     GPT_SW_PATCH_VERSION

#define GPT_AR_RELEASE_MAJOR_VERSION_C  GPT_AR_RELEASE_MAJOR_VERSION
#define GPT_AR_RELEASE_MINOR_VERSION_C  GPT_AR_RELEASE_MINOR_VERSION
#define GPT_AR_RELEASE_PATCH_VERSION_C  GPT_AR_RELEASE_PATCH_VERSION

/* Check if source file and GPT header file are of the same software version */
#if ((GPT_SW_MAJOR_VERSION_C != GPT_SW_MAJOR_VERSION) || \
     (GPT_SW_MINOR_VERSION_C != GPT_SW_MINOR_VERSION) || \
     (GPT_SW_PATCH_VERSION_C != GPT_SW_PATCH_VERSION))
    #error "Software Version Numbers of Gpt.c and Gpt.h are different"
#endif

/* Check if source file and GPT header file are of the same AUTOSAR version */
#if ((GPT_AR_RELEASE_MAJOR_VERSION_C != GPT_AR_RELEASE_MAJOR_VERSION) || \
     (GPT_AR_RELEASE_MINOR_VERSION_C != GPT_AR_RELEASE_MINOR_VERSION) || \
     (GPT_AR_RELEASE_PATCH_VERSION_C != GPT_AR_RELEASE_PATCH_VERSION))
    #error "AUTOSAR Version Numbers of Gpt.c and Gpt.h are different"
#endif

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
#define GPT_MAX_TIMER_PRESCALER        (0xFFFFU) /**< Max value for 16-bit PSC register in STM32 timers. */

/*==================================================================================================
*                                            TYPES
==================================================================================================*/

/** @brief Enumeration for the runtime state of a GPT channel. */
typedef enum
{
    GPT_CHANNEL_STATE_UNINITIALIZED = 0U, /**< Channel is not initialized. */
    GPT_CHANNEL_STATE_INITIALIZED,        /**< Channel is initialized but not running. */
    GPT_CHANNEL_STATE_RUNNING,            /**< Channel is currently timing. */
    GPT_CHANNEL_STATE_STOPPED,            /**< Channel was running and is now stopped. */
    GPT_CHANNEL_STATE_EXPIRED             /**< Channel was one-shot and has completed its period. */
} Gpt_ChannelStateType;

/** @brief Structure to hold runtime information for each configured GPT channel. */
typedef struct
{
    Gpt_ChannelStateType        eState;                 /**< Current state of the channel. */
    Gpt_ValueType               u32TargetTicks;         /**< Target tick value for the current timing period. */
    Gpt_ModeType                eMode;                  /**< Operational mode (One-shot/Continuous), copied from PB config. */
    Gpt_NotificationFuncPtrType pfNotification;         /**< Notification callback function, copied from PB config. */
    TIM_GeneralPurpose_TypeDef* HwTimerRegPtr;        /**< Pointer to the STM32 TIMx peripheral registers. */
    boolean                     bIsNotificationEnabled; /**< Runtime status of notification enabled/disabled. */
    IRQn_Type                   eIrqNumber;             /**< STM32 specific IRQ number for this timer. */
    /* uint16                   u16CurrentPrescaler; */ /* Could store actual prescaler if needed elsewhere */
} Gpt_RuntimeChannelInfoType;


/*==================================================================================================
*                                          VARIABLES
==================================================================================================*/
#define GPT_START_SEC_VAR_INIT_UNSPECIFIED
/* #include "MemMap.h" */

/** @brief Pointer to the current GPT driver post-build configuration set. Initialized by Gpt_Init(). */
STATIC P2CONST(Gpt_ConfigType, GPT_VAR, GPT_APPL_CONST) Gpt_pxCurrentConfig = NULL_PTR;

#define GPT_STOP_SEC_VAR_INIT_UNSPECIFIED
/* #include "MemMap.h" */

#define GPT_START_SEC_VAR_CLEARED_UNSPECIFIED
/* #include "MemMap.h" */

/** @brief Array to store runtime data for each configured GPT channel. Sized by Gpt_Cfg.h. */
STATIC VAR(Gpt_RuntimeChannelInfoType, GPT_VAR) Gpt_axRuntimeChannelData[GPT_CONFIGURED_CHANNELS];

/** @brief Status of the GPT driver initialization. */
STATIC VAR(boolean, GPT_VAR) Gpt_bIsInitialized = FALSE;

#define GPT_STOP_SEC_VAR_CLEARED_UNSPECIFIED
/* #include "MemMap.h" */


/*==================================================================================================
*                                   INTERNAL HELPER FUNCTIONS
==================================================================================================*/
#define GPT_START_SEC_CODE
/* #include "MemMap.h" */

/**
 * @brief Retrieves the STM32 TIM peripheral register structure pointer for a given HwChannel ID.
 * @param[in] HwChannel The hardware channel ID from Gpt_PBcfg.h (e.g., STM32_TIM2).
 * @return TIM_GeneralPurpose_TypeDef* Pointer to the TIMx registers, or NULL_PTR if invalid.
 */
STATIC FUNC(P2VAR(TIM_GeneralPurpose_TypeDef, AUTOMATIC, GPT_CODE), GPT_CODE) Gpt_Internal_GetHwRegPtr(VAR(Gpt_HwChannelType, AUTOMATIC) HwChannel)
{
    P2VAR(TIM_GeneralPurpose_TypeDef, AUTOMATIC, GPT_APPL_DATA) pxTimer = NULL_PTR;
    switch(HwChannel)
    {
        case STM32_TIM2: pxTimer = TIM2; break;
        case STM32_TIM3: pxTimer = TIM3; break;
        case STM32_TIM4: pxTimer = TIM4; break;
        case STM32_TIM6: pxTimer = TIM6; break;
        case STM32_TIM7: pxTimer = TIM7; break;
        /* Add other supported timers as cases here from Gpt_HwChannelType enum */
        default:
            /* Invalid HwChannel. Error should be reported by caller if DET is ON. */
            break;
    }
    return pxTimer;
}

/**
 * @brief Placeholder function to get the input clock frequency for a specific timer peripheral.
 * @details In a complete MCAL, this information would typically come from the Mcu driver.
 * @param[in] HwChannel The hardware channel ID.
 * @return uint32 The clock frequency in Hz.
 */
STATIC FUNC(uint32, GPT_CODE) Gpt_Internal_GetTimerInputClockFrequency(VAR(Gpt_HwChannelType, AUTOMATIC) HwChannel)
{
    (void)HwChannel; /* Suppress warning if HwChannel not used to differentiate clocks */
    /* This must be accurate for correct timing.
     * Example: For STM32F3, if SystemCoreClock is 72MHz and APB1 prescaler (PPRE1) is /2 (PCLK1=36MHz),
     * then TIMxCLK for APB1 timers (TIM2,3,4,6,7) is 2*PCLK1 = 72MHz.
     * If PPRE1 is /1 (PCLK1=72MHz), then TIMxCLK is PCLK1 = 72MHz.
     * This depends on SystemInit() in system_stm32f3xx.c.
     * Using SystemCoreClock directly here might be incorrect if APB prescalers are not 1.
     * For now, assume SystemCoreClock is the TIMxCLK (e.g. 72MHz).
     */
    return SystemCoreClock; /* SystemCoreClock should be defined by CMSIS (system_stm32f3xx.c) */
}

/**
 * @brief Helper to map a hardware timer channel to its IRQn_Type for NVIC.
 * @param[in] HwChannel The hardware channel ID (e.g., STM32_TIM2).
 * @return IRQn_Type The corresponding IRQ number, or a negative/invalid value if not applicable/found.
 */
STATIC FUNC(IRQn_Type, GPT_CODE) Gpt_Internal_GetIrqNumber(VAR(Gpt_HwChannelType, AUTOMATIC) HwChannel)
{
    VAR(IRQn_Type, AUTOMATIC) eIrqNumber = (IRQn_Type)(-1); /* Default to an invalid IRQ number */

    switch(HwChannel)
    {
        case STM32_TIM2: eIrqNumber = TIM2_IRQn; break;
        case STM32_TIM3: eIrqNumber = TIM3_IRQn; break;
        case STM32_TIM4: eIrqNumber = TIM4_IRQn; break;
        case STM32_TIM6: eIrqNumber = TIM6_DAC1_IRQn; break; /* TIM6 and DAC1 share vector on STM32F303 */
        case STM32_TIM7: eIrqNumber = TIM7_DAC2_IRQn; break; /* TIM7 and DAC2 share vector on STM32F303 (check specific device RM) */
        /* Add other supported timers and their IRQn_Type from device specific CMSIS header */
        default:
            /* No valid IRQ mapping for this HwChannel or it does not support interrupts. */
            break;
    }
    return eIrqNumber;
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

FUNC(void, MCU_CODE) Gpt_Init(P2CONST(Gpt_ConfigType, AUTOMATIC, GPT_APPL_CONST) ConfigPtr)
{
    VAR(Gpt_ChannelType, AUTOMATIC) u8ChannelIdx;
    P2CONST(Gpt_ChannelConfigType, AUTOMATIC, GPT_APPL_CONST) pxChannelCfg;
    P2VAR(Gpt_RuntimeChannelInfoType, AUTOMATIC, GPT_APPL_DATA) pxRuntimeInfo; /* Changed memclass to GPT_APPL_DATA */
    P2VAR(TIM_GeneralPurpose_TypeDef, AUTOMATIC, GPT_APPL_DATA) pxTimerRegs; /* Changed memclass to GPT_APPL_DATA */
    VAR(uint32, AUTOMATIC) u32TimerBaseClock;
    VAR(uint32, AUTOMATIC) u32PrescalerValue;

#if (GPT_DEV_ERROR_DETECT == STD_ON)
    if (Gpt_bIsInitialized == TRUE) {
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_INIT_API_ID, GPT_E_ALREADY_INITIALIZED);
        return;
    }
    if (NULL_PTR == ConfigPtr) {
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_INIT_API_ID, GPT_E_PARAM_POINTER);
        return;
    }
    if ((NULL_PTR == ConfigPtr->ChannelConfigSet) && (ConfigPtr->NumberOfChannels > 0U)) {
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_INIT_API_ID, GPT_E_PARAM_POINTER); /* Invalid config content */
        return;
    }
    if (ConfigPtr->NumberOfChannels > GPT_CONFIGURED_CHANNELS) {
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_INIT_API_ID, GPT_E_PARAM_CHANNEL); /* Too many channels in PB vs PC */
        return;
    }
#endif
    Gpt_pxCurrentConfig = ConfigPtr;

    for (u8ChannelIdx = 0U; u8ChannelIdx < Gpt_pxCurrentConfig->NumberOfChannels; ++u8ChannelIdx) {
        pxChannelCfg = &(Gpt_pxCurrentConfig->ChannelConfigSet[u8ChannelIdx]);
        pxRuntimeInfo = &Gpt_axRuntimeChannelData[u8ChannelIdx];

        pxRuntimeInfo->eState = GPT_CHANNEL_STATE_UNINITIALIZED;

#if (GPT_DEV_ERROR_DETECT == STD_ON)
        if (pxChannelCfg->GptChannelId != u8ChannelIdx) {
            Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_INIT_API_ID, GPT_E_PARAM_CHANNEL);
            continue;
        }
        if (pxChannelCfg->GptChannelTickFrequencyHz == 0UL) {
            Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_INIT_API_ID, GPT_E_PARAM_VALUE);
            continue;
        }
#endif
        pxRuntimeInfo->HwTimerRegPtr = Gpt_Internal_GetHwRegPtr(pxChannelCfg->HwTimer);
        if (NULL_PTR == pxRuntimeInfo->HwTimerRegPtr) {
#if (GPT_DEV_ERROR_DETECT == STD_ON)
            Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_INIT_API_ID, GPT_E_PARAM_CHANNEL);
#endif
            continue;
        }

        /* TODO: RCC Clock Enable for the timer peripheral (e.g., using macros from Gpt_Hw_Stm32f3.h or Mcu driver)
         * This must be done BEFORE accessing timer registers. For example:
         * if (pxChannelCfg->HwTimer == STM32_TIM2) { GPT_HW_STM32_RCC_TIM2_CLK_ENABLE(); }
         */

        u32TimerBaseClock = Gpt_Internal_GetTimerInputClockFrequency(pxChannelCfg->HwTimer);
        u32PrescalerValue = (u32TimerBaseClock / pxChannelCfg->GptChannelTickFrequencyHz);
        if (u32PrescalerValue > 0UL) { u32PrescalerValue = u32PrescalerValue - 1UL; }

        if (u32PrescalerValue > GPT_MAX_TIMER_PRESCALER) {
#if (GPT_DEV_ERROR_DETECT == STD_ON)
            Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_INIT_API_ID, GPT_E_PARAM_VALUE);
#endif
            u32PrescalerValue = GPT_MAX_TIMER_PRESCALER;
        }

        pxTimerRegs = pxRuntimeInfo->HwTimerRegPtr;
        pxTimerRegs->CR1 = 0U; /* Ensure timer is stopped and reset CR1 before config */
        pxTimerRegs->PSC = (uint16)u32PrescalerValue;
        pxTimerRegs->ARR = pxChannelCfg->GptChannelTickValueMax;

        if (pxChannelCfg->GptChannelMode == GPT_MODE_ONESHOT) {
            pxTimerRegs->CR1 |= TIM_CR1_OPM;
        } /* Else OPM is 0 for continuous (already cleared) */

        pxTimerRegs->CR1 |= TIM_CR1_URS; /* Only counter overflow/underflow generates update event */
        pxTimerRegs->EGR = TIM_EGR_UG;
        pxTimerRegs->SR = (uint16)(~TIM_SR_UIF);
        pxTimerRegs->DIER &= (uint32)(~TIM_DIER_UIE); /* Disable update interrupt initially */

        pxRuntimeInfo->eMode                  = pxChannelCfg->GptChannelMode;
        pxRuntimeInfo->pfNotification         = pxChannelCfg->GptNotification;
        pxRuntimeInfo->bIsNotificationEnabled = FALSE;
        pxRuntimeInfo->u32TargetTicks         = 0U;
        pxRuntimeInfo->eIrqNumber             = Gpt_Internal_GetIrqNumber(pxChannelCfg->HwTimer);
        pxRuntimeInfo->eState                 = GPT_CHANNEL_STATE_INITIALIZED;
    }
    Gpt_bIsInitialized = TRUE;
}

#if (GPT_DEINIT_API == STD_ON)
FUNC(void, MCU_CODE) Gpt_DeInit(void) {
    VAR(Gpt_ChannelType, AUTOMATIC) u8ChannelIdx;
    P2VAR(TIM_GeneralPurpose_TypeDef, AUTOMATIC, GPT_APPL_DATA) pxTimerRegs;

#if (GPT_DEV_ERROR_DETECT == STD_ON)
    if (Gpt_bIsInitialized == FALSE) {
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_DEINIT_API_ID, GPT_E_UNINIT);
        return;
    }
#endif

    if (NULL_PTR != Gpt_pxCurrentConfig) {
        for (u8ChannelIdx = 0U; u8ChannelIdx < Gpt_pxCurrentConfig->NumberOfChannels; ++u8ChannelIdx) {
            pxTimerRegs = Gpt_axRuntimeChannelData[u8ChannelIdx].HwTimerRegPtr;
            if (NULL_PTR != pxTimerRegs) {
                pxTimerRegs->CR1 &= (uint32)(~TIM_CR1_CEN); /* Stop timer */
                pxTimerRegs->DIER &= (uint32)(~TIM_DIER_UIE);/* Disable interrupt in peripheral */
                pxTimerRegs->SR = (uint16)(~TIM_SR_UIF);   /* Clear pending flag */
#if (GPT_ENABLE_DISABLE_NOTIFICATION_API == STD_ON)
                if (Gpt_axRuntimeChannelData[u8ChannelIdx].eIrqNumber >= 0) {
                    /* NVIC_DisableIRQ(Gpt_axRuntimeChannelData[u8ChannelIdx].eIrqNumber); // CMSIS call */
                }
#endif
                /* TODO: Consider RCC Clock Disable (e.g. GPT_HW_STM32_RCC_TIMx_CLK_DISABLE()) - Mcu responsibility */
            }
            Gpt_axRuntimeChannelData[u8ChannelIdx].eState = GPT_CHANNEL_STATE_UNINITIALIZED;
            Gpt_axRuntimeChannelData[u8ChannelIdx].bIsNotificationEnabled = FALSE;
        }
    }
    Gpt_pxCurrentConfig = NULL_PTR;
    Gpt_bIsInitialized = FALSE;
}
#endif /* GPT_DEINIT_API */

#if (GPT_VERSION_INFO_API == STD_ON)
FUNC(void, MCU_CODE) Gpt_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, GPT_APPL_DATA) VersionInfoPtr) {
#if (GPT_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == VersionInfoPtr) {
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_GET_VERSION_INFO_API_ID, GPT_E_PARAM_POINTER);
        return; /* Single point of exit for valid path */
    }
#endif
    VersionInfoPtr->vendorID         = GPT_VENDOR_ID;
    VersionInfoPtr->moduleID         = GPT_MODULE_ID;
    VersionInfoPtr->sw_major_version = GPT_SW_MAJOR_VERSION;
    VersionInfoPtr->sw_minor_version = GPT_SW_MINOR_VERSION;
    VersionInfoPtr->sw_patch_version = GPT_SW_PATCH_VERSION;
}
#endif /* GPT_VERSION_INFO_API */

#if (GPT_TIME_ELAPSED_API == STD_ON)
FUNC(Gpt_ValueType, MCU_CODE) Gpt_GetTimeElapsed(Gpt_ChannelType Channel) {
    VAR(Gpt_ValueType, AUTOMATIC) u32ElapsedTime = 0U;
#if (GPT_DEV_ERROR_DETECT == STD_ON)
    if (Gpt_bIsInitialized == FALSE) {
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_GET_TIME_ELAPSED_API_ID, GPT_E_UNINIT);
    } else if (Channel >= GPT_CONFIGURED_CHANNELS) {
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_GET_TIME_ELAPSED_API_ID, GPT_E_PARAM_CHANNEL);
    } else if (Gpt_axRuntimeChannelData[Channel].eState == GPT_CHANNEL_STATE_UNINITIALIZED) {
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_GET_TIME_ELAPSED_API_ID, GPT_E_PARAM_CHANNEL);
    } else
#endif
    {
        if (NULL_PTR != Gpt_axRuntimeChannelData[Channel].HwTimerRegPtr) {
            u32ElapsedTime = Gpt_axRuntimeChannelData[Channel].HwTimerRegPtr->CNT;
        }
    }
    return u32ElapsedTime;
}
#endif /* GPT_TIME_ELAPSED_API */

#if (GPT_TIME_REMAINING_API == STD_ON)
FUNC(Gpt_ValueType, MCU_CODE) Gpt_GetTimeRemaining(Gpt_ChannelType Channel) {
    VAR(Gpt_ValueType, AUTOMATIC) u32RemainingTime = 0U;
#if (GPT_DEV_ERROR_DETECT == STD_ON)
    if (Gpt_bIsInitialized == FALSE) {
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_GET_TIME_REMAINING_API_ID, GPT_E_UNINIT);
    } else if (Channel >= GPT_CONFIGURED_CHANNELS) {
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_GET_TIME_REMAINING_API_ID, GPT_E_PARAM_CHANNEL);
    } else if (Gpt_axRuntimeChannelData[Channel].eState != GPT_CHANNEL_STATE_RUNNING) {
        /* SWS: If called for a channel that is not running, the function shall return 0. No DET error. */
        /* Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_GET_TIME_REMAINING_API_ID, GPT_E_WRONG_STATE); */
    } else
#endif
    {
        if ((Gpt_axRuntimeChannelData[Channel].eState == GPT_CHANNEL_STATE_RUNNING) &&
            (NULL_PTR != Gpt_axRuntimeChannelData[Channel].HwTimerRegPtr)) {
            VAR(Gpt_ValueType, AUTOMATIC) u32CurrentCount = Gpt_axRuntimeChannelData[Channel].HwTimerRegPtr->CNT;
            VAR(Gpt_ValueType, AUTOMATIC) u32TargetCount = Gpt_axRuntimeChannelData[Channel].u32TargetTicks; /* This is ARR for current run */
            if (u32TargetCount >= u32CurrentCount) {
                u32RemainingTime = u32TargetCount - u32CurrentCount;
            } /* Else, counter might have passed target or error, return 0 */
        }
    }
    return u32RemainingTime;
}
#endif /* GPT_TIME_REMAINING_API */

FUNC(void, MCU_CODE) Gpt_StartTimer(Gpt_ChannelType Channel, Gpt_ValueType Value) {
    P2VAR(TIM_GeneralPurpose_TypeDef, AUTOMATIC, GPT_APPL_DATA) pxTimerRegs;
#if (GPT_DEV_ERROR_DETECT == STD_ON)
    if (Gpt_bIsInitialized == FALSE) {
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_START_TIMER_API_ID, GPT_E_UNINIT); return;
    }
    if (Channel >= GPT_CONFIGURED_CHANNELS) {
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_START_TIMER_API_ID, GPT_E_PARAM_CHANNEL); return;
    }
    if (Gpt_axRuntimeChannelData[Channel].eState == GPT_CHANNEL_STATE_UNINITIALIZED) {
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_START_TIMER_API_ID, GPT_E_PARAM_CHANNEL); return;
    }
    if ((Value == 0U) || (Value > Gpt_pxCurrentConfig->ChannelConfigSet[Channel].GptChannelTickValueMax)) {
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_START_TIMER_API_ID, GPT_E_PARAM_VALUE); return;
    }
    if ((Gpt_axRuntimeChannelData[Channel].eMode == GPT_MODE_ONESHOT) && (Gpt_axRuntimeChannelData[Channel].eState == GPT_CHANNEL_STATE_RUNNING)) {
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_START_TIMER_API_ID, GPT_E_INVALID_CALL); return;
    }
#endif

    pxTimerRegs = Gpt_axRuntimeChannelData[Channel].HwTimerRegPtr;
    if (NULL_PTR != pxTimerRegs) { /* Should always be true if initialized correctly */
        pxTimerRegs->CR1 &= (uint32)(~TIM_CR1_CEN); /* Stop timer before reconfiguring */
        Gpt_axRuntimeChannelData[Channel].u32TargetTicks = Value;
        pxTimerRegs->ARR = Value;
        pxTimerRegs->CNT = 0U;
        pxTimerRegs->EGR = TIM_EGR_UG;
        pxTimerRegs->SR = (uint16)(~TIM_SR_UIF);
        if (Gpt_axRuntimeChannelData[Channel].bIsNotificationEnabled == TRUE) {
            pxTimerRegs->DIER |= TIM_DIER_UIE;
        }
        pxTimerRegs->CR1 |= TIM_CR1_CEN;
        Gpt_axRuntimeChannelData[Channel].eState = GPT_CHANNEL_STATE_RUNNING;
    }
}

FUNC(void, MCU_CODE) Gpt_StopTimer(Gpt_ChannelType Channel) {
    P2VAR(TIM_GeneralPurpose_TypeDef, AUTOMATIC, GPT_APPL_DATA) pxTimerRegs;
#if (GPT_DEV_ERROR_DETECT == STD_ON)
    if (Gpt_bIsInitialized == FALSE) {
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_STOP_TIMER_ID, GPT_E_UNINIT); return;
    }
    if (Channel >= GPT_CONFIGURED_CHANNELS) {
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_STOP_TIMER_ID, GPT_E_PARAM_CHANNEL); return;
    }
    if (Gpt_axRuntimeChannelData[Channel].eState == GPT_CHANNEL_STATE_UNINITIALIZED) {
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_STOP_TIMER_ID, GPT_E_PARAM_CHANNEL); return;
    }
    /* SWS_Gpt_00045: If Gpt_StopTimer is called on a channel which is not running, the function shall do nothing. No DET error. */
#endif
    pxTimerRegs = Gpt_axRuntimeChannelData[Channel].HwTimerRegPtr;
    if ((NULL_PTR != pxTimerRegs) && (Gpt_axRuntimeChannelData[Channel].eState == GPT_CHANNEL_STATE_RUNNING)) {
        pxTimerRegs->CR1 &= (uint32)(~TIM_CR1_CEN);
        Gpt_axRuntimeChannelData[Channel].eState = GPT_CHANNEL_STATE_STOPPED;
    }
}

#if (GPT_ENABLE_DISABLE_NOTIFICATION_API == STD_ON)
FUNC(void, MCU_CODE) Gpt_EnableNotification(Gpt_ChannelType Channel) {
    P2VAR(TIM_GeneralPurpose_TypeDef, AUTOMATIC, GPT_APPL_DATA) pxTimerRegs;
#if (GPT_DEV_ERROR_DETECT == STD_ON)
    if (Gpt_bIsInitialized == FALSE) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_ENABLE_NOTIFICATION_API_ID, GPT_E_UNINIT); return; }
    if (Channel >= GPT_CONFIGURED_CHANNELS) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_ENABLE_NOTIFICATION_API_ID, GPT_E_PARAM_CHANNEL); return; }
    if (Gpt_axRuntimeChannelData[Channel].eState == GPT_CHANNEL_STATE_UNINITIALIZED) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_ENABLE_NOTIFICATION_API_ID, GPT_E_PARAM_CHANNEL); return; }
    if (NULL_PTR == Gpt_axRuntimeChannelData[Channel].pfNotification) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_ENABLE_NOTIFICATION_API_ID, GPT_E_PARAM_CHANNEL); return; } /* SWS_Gpt_00215: No notification configured */
#endif
    pxTimerRegs = Gpt_axRuntimeChannelData[Channel].HwTimerRegPtr;
    if ((NULL_PTR != pxTimerRegs) && (NULL_PTR != Gpt_axRuntimeChannelData[Channel].pfNotification)) {
        Gpt_axRuntimeChannelData[Channel].bIsNotificationEnabled = TRUE;
        pxTimerRegs->DIER |= TIM_DIER_UIE;
        if (Gpt_axRuntimeChannelData[Channel].eIrqNumber >= 0) { /* NVIC_EnableIRQ(Gpt_axRuntimeChannelData[Channel].eIrqNumber); // CMSIS Call */ }
    }
}

FUNC(void, MCU_CODE) Gpt_DisableNotification(Gpt_ChannelType Channel) {
    P2VAR(TIM_GeneralPurpose_TypeDef, AUTOMATIC, GPT_APPL_DATA) pxTimerRegs;
#if (GPT_DEV_ERROR_DETECT == STD_ON)
    if (Gpt_bIsInitialized == FALSE) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_DISABLE_NOTIFICATION_API_ID, GPT_E_UNINIT); return; }
    if (Channel >= GPT_CONFIGURED_CHANNELS) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_DISABLE_NOTIFICATION_API_ID, GPT_E_PARAM_CHANNEL); return; }
    if (Gpt_axRuntimeChannelData[Channel].eState == GPT_CHANNEL_STATE_UNINITIALIZED) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_DISABLE_NOTIFICATION_API_ID, GPT_E_PARAM_CHANNEL); return; }
#endif
    pxTimerRegs = Gpt_axRuntimeChannelData[Channel].HwTimerRegPtr;
    if (NULL_PTR != pxTimerRegs) {
        Gpt_axRuntimeChannelData[Channel].bIsNotificationEnabled = FALSE;
        pxTimerRegs->DIER &= (uint32)(~TIM_DIER_UIE);
        /* No need to call NVIC_DisableIRQ if Gpt_DeInit handles it, or if other channels might share IRQ.
           SWS_Gpt_00049: "disables the interrupt generation in the hardware timer peripheral."
           NVIC disabling is an optimization or system-level concern. For now, only peripheral interrupt disabled.
        */
    }
}
#endif /* GPT_ENABLE_DISABLE_NOTIFICATION_API */

/*==================================================================================================
*                                       ISR HANDLERS
==================================================================================================*/
#ifndef ISR_CODE /* Define if not in Compiler.h, for ISRs */
#define ISR_CODE MCU_CODE /* Default to MCU_CODE if ISR_CODE not specifically defined */
#endif

STATIC FUNC(void, ISR_CODE) Gpt_ProcessCommonInterrupt(Gpt_ChannelType Channel)
{
    P2VAR(Gpt_RuntimeChannelInfoType, AUTOMATIC, GPT_APPL_DATA) pxRuntime = &Gpt_axRuntimeChannelData[Channel];
    P2VAR(TIM_GeneralPurpose_TypeDef, AUTOMATIC, GPT_APPL_DATA) pxTimerRegs = pxRuntime->HwTimerRegPtr;

    if ((NULL_PTR != pxTimerRegs) && ((pxTimerRegs->SR & TIM_SR_UIF) != 0U) && ((pxTimerRegs->DIER & TIM_DIER_UIE) != 0U))
    {
        pxTimerRegs->SR = (uint16)(~TIM_SR_UIF); /* Clear Update Interrupt Flag by writing 0 */

        if ((pxRuntime->bIsNotificationEnabled == TRUE) && (NULL_PTR != pxRuntime->pfNotification))
        {
            pxRuntime->pfNotification();
        }

        if (pxRuntime->eMode == GPT_MODE_ONESHOT)
        {
            pxTimerRegs->CR1 &= (uint32)(~TIM_CR1_CEN); /* Stop timer */
            pxRuntime->eState = GPT_CHANNEL_STATE_EXPIRED;
        }
        /* For continuous mode, timer automatically reloads and continues. */
    }
}

/* These ISR names must match the vector table in startup_stm32f3xx.s */
/* Example for TIM2 - check actual name for your startup file */
FUNC(void, ISR_CODE) TIM2_IRQHandler(void)
{
    VAR(Gpt_ChannelType, AUTOMATIC) u8ChannelIdx;
    if (NULL_PTR != Gpt_pxCurrentConfig) { /* Check if Gpt is initialized */
        for(u8ChannelIdx = 0U; u8ChannelIdx < Gpt_pxCurrentConfig->NumberOfChannels; ++u8ChannelIdx) {
            if(Gpt_pxCurrentConfig->ChannelConfigSet[u8ChannelIdx].HwTimer == STM32_TIM2) {
                Gpt_ProcessCommonInterrupt(Gpt_pxCurrentConfig->ChannelConfigSet[u8ChannelIdx].GptChannelId);
                break; /* Assuming one logical channel per HW Timer instance for this ISR */
            }
        }
    }
}

/* Example for TIM6_DAC / TIM6_DAC1 - check actual name for your startup file */
FUNC(void, ISR_CODE) TIM6_DAC1_IRQHandler(void) /* Common name for STM32F303 */
{
    VAR(Gpt_ChannelType, AUTOMATIC) u8ChannelIdx;
     if (NULL_PTR != Gpt_pxCurrentConfig) {
        for(u8ChannelIdx = 0U; u8ChannelIdx < Gpt_pxCurrentConfig->NumberOfChannels; ++u8ChannelIdx) {
            if(Gpt_pxCurrentConfig->ChannelConfigSet[u8ChannelIdx].HwTimer == STM32_TIM6) {
                Gpt_ProcessCommonInterrupt(Gpt_pxCurrentConfig->ChannelConfigSet[u8ChannelIdx].GptChannelId);
                break;
            }
        }
    }
    /* If DAC is also using this interrupt, its handler part would go here. */
}

#define GPT_STOP_SEC_CODE
/* #include "MemMap.h" */
