/*==================================================================================================
*   Project              : AUTOSAR 4.x MCAL
*   Platform             : STM32
*   Peripheral           : GPT
*   Dependencies         : STM32F3xx Series
*
*   Autosar Version      : 4.3.1
*   Autosar Revision     : ASR_REL_4_3_REV_0001
*   Sw Version           : 1.0.0
*   Build Version        : GPT_MCAL_1_0_0_RTM_ASR_REL_4_3_REV_0001_2024xxxx
*
*   (c) Copyright 2023-2024 YourName
*   All Rights Reserved.
==================================================================================================*/

#include "../Include/Gpt.h"            /* Main Gpt header (includes Gpt_Cfg.h, Std_Types.h) */
#include "../Include/Gpt_Hw_Stm32f3.h" /* Private HW access header (includes stm32f3xx.h) */

#if (GPT_DEV_ERROR_DETECT == STD_ON) /* From Gpt_Cfg.h */
    #include "Det.h" /* Development Error Tracer */
#endif

/* Include CMSIS core for NVIC functions if not transitively included by stm32f3xx.h */
/* For STM32F3 series, stm32f3xx.h usually includes stm32f303xc.h (or similar) which includes core_cm4.h */
/* #include "core_cm4.h" */


/*==================================================================================================
*                                       VERSION CHECKS
==================================================================================================*/
/* Define this file's version numbers */
#define GPT_SW_MAJOR_VERSION_C     1
#define GPT_SW_MINOR_VERSION_C     0
#define GPT_SW_PATCH_VERSION_C     0

#define GPT_AR_RELEASE_MAJOR_VERSION_C  4
#define GPT_AR_RELEASE_MINOR_VERSION_C  3
#define GPT_AR_RELEASE_PATCH_VERSION_C  1

/* Check against Gpt.h version numbers (assuming these are defined in Gpt.h) */
#if ((GPT_SW_MAJOR_VERSION_C != GPT_SW_MAJOR_VERSION) ||  \
     (GPT_SW_MINOR_VERSION_C != GPT_SW_MINOR_VERSION) ||  \
     (GPT_SW_PATCH_VERSION_C != GPT_SW_PATCH_VERSION))
  #error "Software Version Numbers of Gpt.c and Gpt.h are different"
#endif

#if ((GPT_AR_RELEASE_MAJOR_VERSION_C != GPT_AR_RELEASE_MAJOR_VERSION) || \
     (GPT_AR_RELEASE_MINOR_VERSION_C != GPT_AR_RELEASE_MINOR_VERSION) || \
     (GPT_AR_RELEASE_PATCH_VERSION_C != GPT_AR_RELEASE_PATCH_VERSION))
  #error "AUTOSAR Version Numbers of Gpt.c and Gpt.h are different"
#endif

/*==================================================================================================
*                                          VARIABLES
==================================================================================================*/
#define GPT_START_SEC_VAR_INIT_UNSPECIFIED
/* #include "MemMap.h" */

STATIC P2CONST(Gpt_ConfigType, GPT_VAR, GPT_APPL_CONST) Gpt_GlobalConfigPtr = NULL_PTR;

#define GPT_STOP_SEC_VAR_INIT_UNSPECIFIED
/* #include "MemMap.h" */

#define GPT_START_SEC_VAR_CLEARED_UNSPECIFIED
/* #include "MemMap.h" */

typedef enum {
    GPT_CHANNEL_STATE_UNINITIALIZED,
    GPT_CHANNEL_STATE_INITIALIZED,
    GPT_CHANNEL_STATE_RUNNING,
    GPT_CHANNEL_STATE_STOPPED,
    GPT_CHANNEL_STATE_EXPIRED /* For one-shot mode after expiry if not stopped explicitly */
} Gpt_ChannelStateType;

typedef struct {
    Gpt_ChannelStateType        State;
    Gpt_ValueType               TargetTicks;
    Gpt_ModeType                Mode;           /* Copied from PB config for quick access */
    Gpt_NotificationFuncPtrType Notification;   /* Copied from PB config */
    P2VAR(TIM_TypeDef, AUTOMATIC, GPT_VAR) HwTimerRegPtr; /* Pointer to TIMx registers */
    boolean                     IsNotificationEnabled;
    uint16                      CurrentPrescaler; /* Actual prescaler value written to HW */
    IRQn_Type                   IrqNumber;        /* STM32 specific IRQ number for this timer */
} Gpt_RuntimeChannelInfoType;

STATIC VAR(Gpt_RuntimeChannelInfoType, GPT_VAR) Gpt_RuntimeChannelData[GPT_CONFIGURED_CHANNELS];

#define GPT_STOP_SEC_VAR_CLEARED_UNSPECIFIED
/* #include "MemMap.h" */


/*==================================================================================================
*                                   INTERNAL HELPER FUNCTIONS
==================================================================================================*/
#define GPT_START_SEC_CODE
/* #include "MemMap.h" */

STATIC FUNC(P2VAR(TIM_TypeDef, AUTOMATIC, GPT_CODE), GPT_CODE) Gpt_GetHwRegPtr(Gpt_HwChannelType HwChannelId)
{
    P2VAR(TIM_TypeDef, AUTOMATIC, GPT_CODE) timer = NULL_PTR;
    switch(HwChannelId) {
        case GPT_HW_STM32_TIM2: timer = TIM2; break;
        case GPT_HW_STM32_TIM3: timer = TIM3; break;
        case GPT_HW_STM32_TIM4: timer = TIM4; break;
        /* TIM5 is not on all F3, example: case GPT_HW_STM32_TIM5: timer = TIM5; break; */
        case GPT_HW_STM32_TIM6: timer = TIM6; break;
        case GPT_HW_STM32_TIM7: timer = TIM7; break;
        /* Add other supported timers like TIM1, TIM15, TIM16, TIM17 */
        default:
#if (GPT_DEV_ERROR_DETECT == STD_ON)
            Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_INIT_ID, GPT_E_PARAM_CHANNEL); /* Using GPT_INIT_ID as this is part of init */
#endif
            break;
    }
    return timer;
}

STATIC FUNC(IRQn_Type, GPT_CODE) Gpt_GetIrqNumber(Gpt_HwChannelType HwChannelId)
{
    IRQn_Type irq = NonMaskableInt_IRQn; /* Default to a non-GPT IRQ, error condition */
    switch(HwChannelId) {
        case GPT_HW_STM32_TIM2: irq = TIM2_IRQn; break;
        case GPT_HW_STM32_TIM3: irq = TIM3_IRQn; break;
        case GPT_HW_STM32_TIM4: irq = TIM4_IRQn; break;
        case GPT_HW_STM32_TIM6: irq = TIM6_DAC_IRQn; break; /* TIM6 and DAC share IRQ vector */
        case GPT_HW_STM32_TIM7: irq = TIM7_IRQn; break;     /* TIM7 has its own IRQ vector on some F3, shares with DAC2 on others */
        /* Add others */
        default:
#if (GPT_DEV_ERROR_DETECT == STD_ON)
            Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_INIT_ID, GPT_E_PARAM_CHANNEL);
#endif
            break;
    }
    return irq;
}

// Placeholder for getting timer clock frequency.
// In a real system, Mcu driver would provide this.
STATIC FUNC(uint32, GPT_CODE) Gpt_GetTimerInputClockFrequency(Gpt_HwChannelType HwChannelId) {
    (void)HwChannelId; // Suppress unused warning for now
    /* Example: Assume APB1 timers (TIM2,3,4,6,7 for F303) run at SystemCoreClock.
     * (APB1 prescaler is often /1 or /2 from HCLK, SystemCoreClock usually refers to HCLK).
     * This should be correctly obtained from an MCU module or a fixed system configuration header.
     * For STM32F303, if PCLK1 is 72MHz (max), then timers on APB1 get 72MHz if APB1 prescaler is 1.
     * If APB1 prescaler is >1, TIMxCLK is 2*PCLK1. Let's assume it's 72MHz for simplicity.
     */
    return SystemCoreClock; // SystemCoreClock should be defined by CMSIS system_stm32f3xx.c
}


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

FUNC(void, MCU_CODE) Gpt_Init(P2CONST(Gpt_ConfigType, AUTOMATIC, APPL_CONST) ConfigPtr)
{
    VAR(uint8, AUTOMATIC) i;
    P2CONST(Gpt_ChannelConfigType, AUTOMATIC, APPL_CONST) channelCfg;
    P2VAR(Gpt_RuntimeChannelInfoType, AUTOMATIC, GPT_VAR) runtimeInfo;

#if (GPT_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == ConfigPtr) {
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_INIT_ID, GPT_E_PARAM_POINTER);
        return;
    }
    if (Gpt_GlobalConfigPtr != NULL_PTR) {
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_INIT_ID, GPT_E_ALREADY_INITIALIZED);
        return;
    }
    if (ConfigPtr->NumberOfChannels > GPT_CONFIGURED_CHANNELS) { /* Check against Gpt_Cfg.h */
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_INIT_ID, GPT_E_PARAM_CHANNEL);
        return;
    }
    if ((NULL_PTR == ConfigPtr->ChannelConfigSet) && (ConfigPtr->NumberOfChannels > 0U)) {
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_INIT_ID, GPT_E_PARAM_CONFIG);
        return;
    }
#endif
    Gpt_GlobalConfigPtr = ConfigPtr;

    for (i = 0U; i < Gpt_GlobalConfigPtr->NumberOfChannels; ++i) {
        channelCfg = &(Gpt_GlobalConfigPtr->ChannelConfigSet[i]);
        runtimeInfo = &Gpt_RuntimeChannelData[i];

        runtimeInfo->State = GPT_CHANNEL_STATE_UNINITIALIZED; /* Mark as uninitialized first */

        if (channelCfg->GptChannelId != i) { /* Basic check for config consistency */
#if (GPT_DEV_ERROR_DETECT == STD_ON)
            Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_INIT_ID, GPT_E_PARAM_CHANNEL);
#endif
            continue;
        }

        runtimeInfo->HwTimerRegPtr = Gpt_GetHwRegPtr(channelCfg->HwTimer);
        if (NULL_PTR == runtimeInfo->HwTimerRegPtr) {
            /* Error already reported by Gpt_GetHwRegPtr if DET is ON */
            continue;
        }
        runtimeInfo->IrqNumber = Gpt_GetIrqNumber(channelCfg->HwTimer);


        /* Enable clock for the timer (simplified - Mcu should do this) */
        /* Example: if(channelCfg->HwTimer == GPT_HW_STM32_TIM2) { GPT_STM32_RCC_TIM2_CLK_ENABLE(); } */
        /* For this implementation, we assume clocks are already enabled by MCU or startup. */

        /* Calculate Prescaler (PSC) */
        uint32 timerBaseClock = Gpt_GetTimerInputClockFrequency(channelCfg->HwTimer);
        uint32 prescalerCalculated = (timerBaseClock / channelCfg->GptChannelTickFrequencyHz) - 1U;

        if (prescalerCalculated > 0xFFFFU) { /* Max PSC value for STM32 timers is 16-bit */
            prescalerCalculated = 0xFFFFU;
#if (GPT_DEV_ERROR_DETECT == STD_ON) /* Tick frequency might not be achievable */
            Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_INIT_ID, GPT_E_PARAM_VALUE);
#endif
        }
        runtimeInfo->CurrentPrescaler = (uint16)prescalerCalculated;
        runtimeInfo->HwTimerRegPtr->PSC = runtimeInfo->CurrentPrescaler;
        runtimeInfo->HwTimerRegPtr->ARR = channelCfg->GptChannelTickValueMax;

        if (channelCfg->GptChannelMode == GPT_MODE_ONESHOT) {
            runtimeInfo->HwTimerRegPtr->CR1 |= TIM_CR1_OPM;
        } else { /* GPT_MODE_CONTINUOUS */
            runtimeInfo->HwTimerRegPtr->CR1 &= (uint16_t)(~TIM_CR1_OPM);
        }
        runtimeInfo->HwTimerRegPtr->CR1 &= (uint16_t)(~TIM_CR1_DIR); /* Up-counter */
        runtimeInfo->HwTimerRegPtr->CR1 &= (uint16_t)(~TIM_CR1_CMS); /* Edge-aligned mode */

        runtimeInfo->HwTimerRegPtr->EGR = TIM_EGR_UG;      /* Generate an update event to load PSC and ARR */
        runtimeInfo->HwTimerRegPtr->SR = (uint16_t)(~TIM_SR_UIF); /* Clear update interrupt flag */
        runtimeInfo->HwTimerRegPtr->DIER &= (uint16_t)(~TIM_DIER_UIE); /* Disable update interrupt initially */

        runtimeInfo->Mode = channelCfg->GptChannelMode;
        runtimeInfo->Notification = channelCfg->GptNotification;
        runtimeInfo->IsNotificationEnabled = FALSE;
        runtimeInfo->TargetTicks = 0U;
        runtimeInfo->State = GPT_CHANNEL_STATE_INITIALIZED;
    }
}

FUNC(void, MCU_CODE) Gpt_DeInit(void) {
#if (GPT_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == Gpt_GlobalConfigPtr) { /* Not initialized or already de-initialized */
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_DEINIT_ID, GPT_E_UNINIT);
        return;
    }
#endif
    VAR(uint8, AUTOMATIC) i;
    P2VAR(TIM_TypeDef, AUTOMATIC, GPT_VAR) timer;
    for (i = 0U; i < Gpt_GlobalConfigPtr->NumberOfChannels; ++i) {
        timer = Gpt_RuntimeChannelData[i].HwTimerRegPtr;
        if (NULL_PTR != timer) {
            timer->CR1 &= (uint16_t)(~TIM_CR1_CEN); /* Disable timer */
            timer->DIER &= (uint16_t)(~TIM_DIER_UIE); /* Disable update interrupt */
            timer->SR = (uint16_t)(~TIM_SR_UIF);   /* Clear pending flags */
            /* Optionally disable NVIC interrupt: if(Gpt_RuntimeChannelData[i].IsNotificationEnabled) NVIC_DisableIRQ(Gpt_RuntimeChannelData[i].IrqNumber); */
            /* Optionally disable timer clock via RCC (e.g. GPT_STM32_RCC_TIM2_CLK_DISABLE()) - Mcu job */
        }
        Gpt_RuntimeChannelData[i].State = GPT_CHANNEL_STATE_UNINITIALIZED;
        Gpt_RuntimeChannelData[i].IsNotificationEnabled = FALSE;
    }
    Gpt_GlobalConfigPtr = NULL_PTR;
}

#if (GPT_VERSION_INFO_API == STD_ON)
FUNC(void, MCU_CODE) Gpt_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, APPL_DATA) VersionInfoPtr) {
#if (GPT_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == VersionInfoPtr) {
        Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_GET_VERSION_INFO_ID, GPT_E_PARAM_POINTER);
        return;
    }
#endif
    VersionInfoPtr->vendorID         = GPT_VENDOR_ID;
    VersionInfoPtr->moduleID         = GPT_MODULE_ID;
    VersionInfoPtr->sw_major_version = GPT_SW_MAJOR_VERSION; /* From Gpt.h */
    VersionInfoPtr->sw_minor_version = GPT_SW_MINOR_VERSION; /* From Gpt.h */
    VersionInfoPtr->sw_patch_version = GPT_SW_PATCH_VERSION; /* From Gpt.h */
}
#endif

FUNC(Gpt_ValueType, MCU_CODE) Gpt_GetTimeElapsed(Gpt_ChannelType Channel) {
#if (GPT_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == Gpt_GlobalConfigPtr) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_GET_TIME_ELAPSED_ID, GPT_E_UNINIT); return 0; }
    if (Channel >= Gpt_GlobalConfigPtr->NumberOfChannels) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_GET_TIME_ELAPSED_ID, GPT_E_PARAM_CHANNEL); return 0; }
    if (Gpt_RuntimeChannelData[Channel].State == GPT_CHANNEL_STATE_UNINITIALIZED) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_GET_TIME_ELAPSED_ID, GPT_E_CHANNEL_UNINITIALIZED); return 0;}
#endif
    return Gpt_RuntimeChannelData[Channel].HwTimerRegPtr->CNT;
}

FUNC(Gpt_ValueType, MCU_CODE) Gpt_GetTimeRemaining(Gpt_ChannelType Channel) {
#if (GPT_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == Gpt_GlobalConfigPtr) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_GET_TIME_REMAINING_ID, GPT_E_UNINIT); return 0; }
    if (Channel >= Gpt_GlobalConfigPtr->NumberOfChannels) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_GET_TIME_REMAINING_ID, GPT_E_PARAM_CHANNEL); return 0; }
    if (Gpt_RuntimeChannelData[Channel].State != GPT_CHANNEL_STATE_RUNNING) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_GET_TIME_REMAINING_ID, GPT_E_WRONG_STATE); return 0;}
#endif
    VAR(Gpt_ValueType, AUTOMATIC) target = Gpt_RuntimeChannelData[Channel].TargetTicks; /* For one-shot, this is the loaded ARR */
                                                                          /* For continuous, ARR is fixed by MaxTickValue. TargetTicks is not used this way. */
    if(Gpt_RuntimeChannelData[Channel].Mode == GPT_MODE_CONTINUOUS) {
        target = Gpt_RuntimeChannelData[Channel].HwTimerRegPtr->ARR;
    }
    return target - Gpt_RuntimeChannelData[Channel].HwTimerRegPtr->CNT;
}

FUNC(void, MCU_CODE) Gpt_StartTimer(Gpt_ChannelType Channel, Gpt_ValueType Value) {
#if (GPT_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == Gpt_GlobalConfigPtr) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_START_TIMER_ID, GPT_E_UNINIT); return; }
    if (Channel >= Gpt_GlobalConfigPtr->NumberOfChannels) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_START_TIMER_ID, GPT_E_PARAM_CHANNEL); return; }
    if (Gpt_RuntimeChannelData[Channel].State == GPT_CHANNEL_STATE_UNINITIALIZED) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_START_TIMER_ID, GPT_E_CHANNEL_UNINITIALIZED); return; }
    if (Value == 0U || Value > Gpt_GlobalConfigPtr->ChannelConfigSet[Channel].GptChannelTickValueMax) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_START_TIMER_ID, GPT_E_PARAM_VALUE); return; }
    if (Gpt_RuntimeChannelData[Channel].State == GPT_CHANNEL_STATE_RUNNING && Gpt_RuntimeChannelData[Channel].Mode == GPT_MODE_ONESHOT) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_START_TIMER_ID, GPT_E_INVALID_CALL); return; }
#endif

    P2VAR(TIM_TypeDef, AUTOMATIC, GPT_VAR) timer = Gpt_RuntimeChannelData[Channel].HwTimerRegPtr;
    Gpt_RuntimeChannelData[Channel].TargetTicks = Value;

    timer->ARR = Value;
    timer->CNT = 0U;
    timer->EGR = TIM_EGR_UG;
    timer->SR = (uint16_t)(~TIM_SR_UIF);

    if (Gpt_RuntimeChannelData[Channel].IsNotificationEnabled) {
        timer->DIER |= TIM_DIER_UIE;
    }
    timer->CR1 |= TIM_CR1_CEN;
    Gpt_RuntimeChannelData[Channel].State = GPT_CHANNEL_STATE_RUNNING;
}

FUNC(void, MCU_CODE) Gpt_StopTimer(Gpt_ChannelType Channel) {
#if (GPT_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == Gpt_GlobalConfigPtr) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_STOP_TIMER_ID, GPT_E_UNINIT); return; }
    if (Channel >= Gpt_GlobalConfigPtr->NumberOfChannels) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_STOP_TIMER_ID, GPT_E_PARAM_CHANNEL); return; }
    if (Gpt_RuntimeChannelData[Channel].State == GPT_CHANNEL_STATE_UNINITIALIZED) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_STOP_TIMER_ID, GPT_E_CHANNEL_UNINITIALIZED); return; }
    if (Gpt_RuntimeChannelData[Channel].State != GPT_CHANNEL_STATE_RUNNING) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_STOP_TIMER_ID, GPT_E_WRONG_STATE); return; }
#endif
    Gpt_RuntimeChannelData[Channel].HwTimerRegPtr->CR1 &= (uint16_t)(~TIM_CR1_CEN);
    Gpt_RuntimeChannelData[Channel].State = GPT_CHANNEL_STATE_STOPPED;
}

#if (GPT_ENABLE_DISABLE_NOTIFICATION_API == STD_ON)
FUNC(void, MCU_CODE) Gpt_EnableNotification(Gpt_ChannelType Channel) {
#if (GPT_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == Gpt_GlobalConfigPtr) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_ENABLE_NOTIFICATION_ID, GPT_E_UNINIT); return; }
    if (Channel >= Gpt_GlobalConfigPtr->NumberOfChannels) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_ENABLE_NOTIFICATION_ID, GPT_E_PARAM_CHANNEL); return; }
    if (Gpt_RuntimeChannelData[Channel].State == GPT_CHANNEL_STATE_UNINITIALIZED) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_ENABLE_NOTIFICATION_ID, GPT_E_CHANNEL_UNINITIALIZED); return; }
    if (NULL_PTR == Gpt_RuntimeChannelData[Channel].Notification) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_ENABLE_NOTIFICATION_ID, GPT_E_PARAM_POINTER); return; } /* No callback to enable */
#endif
    Gpt_RuntimeChannelData[Channel].IsNotificationEnabled = TRUE;
    Gpt_RuntimeChannelData[Channel].HwTimerRegPtr->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(Gpt_RuntimeChannelData[Channel].IrqNumber);
}

FUNC(void, MCU_CODE) Gpt_DisableNotification(Gpt_ChannelType Channel) {
#if (GPT_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == Gpt_GlobalConfigPtr) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_DISABLE_NOTIFICATION_ID, GPT_E_UNINIT); return; }
    if (Channel >= Gpt_GlobalConfigPtr->NumberOfChannels) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_DISABLE_NOTIFICATION_ID, GPT_E_PARAM_CHANNEL); return; }
    if (Gpt_RuntimeChannelData[Channel].State == GPT_CHANNEL_STATE_UNINITIALIZED) { Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, GPT_DISABLE_NOTIFICATION_ID, GPT_E_CHANNEL_UNINITIALIZED); return; }
#endif
    Gpt_RuntimeChannelData[Channel].IsNotificationEnabled = FALSE;
    Gpt_RuntimeChannelData[Channel].HwTimerRegPtr->DIER &= (uint16_t)(~TIM_DIER_UIE);
    /* Note: Disabling NVIC IRQ globally for the timer might affect other channels if same timer HW is shared.
       AUTOSAR GPT typically maps one logical channel to one hardware timer source. */
    NVIC_DisableIRQ(Gpt_RuntimeChannelData[Channel].IrqNumber);
}
#endif /* GPT_ENABLE_DISABLE_NOTIFICATION_API */


/* Example ISR Handlers (Actual names are TIMx_IRQHandler from startup_stm32f3xx.s) */
/* These need to be defined if notifications are used. */
/* They must be registered in the vector table. */

/* Generic handler, to be called by specific TIMx_IRQHandler */
STATIC FUNC(void, MCU_CODE) Gpt_ProcessInterrupt(Gpt_ChannelType Channel)
{
    if (Channel < GPT_CONFIGURED_CHANNELS && /* Basic safety check */
        NULL_PTR != Gpt_RuntimeChannelData[Channel].HwTimerRegPtr &&
        (Gpt_RuntimeChannelData[Channel].HwTimerRegPtr->SR & TIM_SR_UIF) )
    {
        Gpt_RuntimeChannelData[Channel].HwTimerRegPtr->SR = (uint16_t)(~TIM_SR_UIF); /* Clear interrupt flag */

        if (Gpt_RuntimeChannelData[Channel].IsNotificationEnabled &&
            NULL_PTR != Gpt_RuntimeChannelData[Channel].Notification)
        {
            Gpt_RuntimeChannelData[Channel].Notification();
        }

        if (Gpt_RuntimeChannelData[Channel].Mode == GPT_MODE_ONESHOT) {
            Gpt_RuntimeChannelData[Channel].HwTimerRegPtr->CR1 &= (uint16_t)(~TIM_CR1_CEN); /* Stop timer */
            Gpt_RuntimeChannelData[Channel].State = GPT_CHANNEL_STATE_EXPIRED;
        }
        /* For continuous mode, it reloads and continues automatically. */
    }
}

/* Example for TIM2 IRQ Handler */
/* This name (TIM2_IRQHandler) must match the name in the vector table. */
FUNC(void, MCU_CODE) TIM2_IRQHandler(void)
{
    /* Find which logical channel maps to TIM2. This mapping is in Gpt_PBcfg.c */
    /* This simple loop assumes GptChannelId in config is the index. */
    VAR(uint8, AUTOMATIC) i;
    for(i=0; i < GPT_CONFIGURED_CHANNELS; ++i) {
        if(Gpt_GlobalConfigPtr != NULL_PTR && Gpt_GlobalConfigPtr->ChannelConfigSet[i].HwTimer == GPT_HW_STM32_TIM2) {
            Gpt_ProcessInterrupt(Gpt_GlobalConfigPtr->ChannelConfigSet[i].GptChannelId);
            break; /* Assuming one logical channel per HW Timer for simplicity in ISR */
        }
    }
}

/* Example for TIM6_DAC_IRQHandler (TIM6 shares IRQ with DAC1 Ch1&2 Update Underrun) */
FUNC(void, MCU_CODE) TIM6_DAC_IRQHandler(void) /* Or TIM6_DAC1_IRQHandler on some F3 */
{
    VAR(uint8, AUTOMATIC) i;
    for(i=0; i < GPT_CONFIGURED_CHANNELS; ++i) {
        if(Gpt_GlobalConfigPtr != NULL_PTR && Gpt_GlobalConfigPtr->ChannelConfigSet[i].HwTimer == GPT_HW_STM32_TIM6) {
            Gpt_ProcessInterrupt(Gpt_GlobalConfigPtr->ChannelConfigSet[i].GptChannelId);
            break;
        }
    }
    /* Add DAC handling here if DAC interrupts are also used for this vector */
}

#define GPT_STOP_SEC_CODE
/* #include "MemMap.h" */
