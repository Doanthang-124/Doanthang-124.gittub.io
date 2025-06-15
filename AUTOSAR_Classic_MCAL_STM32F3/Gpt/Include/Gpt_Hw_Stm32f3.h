#ifndef GPT_HW_STM32F3_H
#define GPT_HW_STM32F3_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/

/**
 * @file    Gpt_Hw_Stm32f3.h
 * @brief   Private header file for GPT driver, providing STM32F3 specific hardware
 *          abstractions, primarily register definitions via the main device header.
 * @details This file includes the main device header (`stm32f3xx.h`) which contains
 *          the definitions for Timer (TIMx) and RCC peripheral registers.
 *          It primarily serves to ensure these definitions are available to Gpt.c.
 */

#include "../../Common/Include/stm32f3xx.h"
/* This brings in:
 * - TIM_GeneralPurpose_TypeDef and TIMx base address pointers (TIM2, TIM6, etc.)
 * - RCC_TypeDef and RCC base address pointer.
 * - Register bit definitions (TIM_CR1_CEN, RCC_APB1ENR_TIM2EN, etc.)
 * - Std_Types.h (via stm32f3xx.h -> Std_Types.h) for uint32 etc.
 * - Compiler.h (via Std_Types.h) for _VOLATILE_ etc.
 * - CMSIS core functions like NVIC_EnableIRQ (via stm32f3xx.h -> device header -> core_cmX.h)
 * - IRQn_Type enum (via stm32f3xx.h -> device header)
 */

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/*
 * No Gpt-driver-specific hardware access macros are defined here for now.
 * Gpt.c will use the register definitions and bitfields directly from stm32f3xx.h.
 * RCC clock enable macros that were here previously have been removed as stm32f3xx.h
 * now provides the necessary RCC register bit definitions (e.g., RCC_APB1ENR_TIM2EN),
 * and clock enabling is typically an Mcu module responsibility or part of system startup.
 * If Gpt_Init were to enable clocks, it would use these bits directly.
 */

#endif /* GPT_HW_STM32F3_H */
