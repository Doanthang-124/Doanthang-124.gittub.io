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
 *          It serves as an internal abstraction point for such definitions if needed,
 *          but mostly relies on `stm32f3xx.h`.
 */

/* Include the main STM32F3 device header for TIMx and RCC register definitions */
#include "../../Common/Include/stm32f3xx.h"
/* This brings in:
 * - TIM_GeneralPurpose_TypeDef and TIMx base address pointers (TIM2, TIM6, etc.)
 * - RCC_TypeDef and RCC base address pointer.
 * - Register bit definitions (TIM_CR1_CEN, RCC_APB1ENR_TIM2EN, etc.)
 * - Std_Types.h (via stm32f3xx.h -> Std_Types.h) for uint32 etc.
 * - Compiler.h (via Std_Types.h) for _VOLATILE_ etc.
 */

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/*
 * Hardware Specific Macros for STM32F3 GPT Driver
 */

/*
 * RCC Clock Enabling/Disabling Macros for Timers (APB1 and APB2)
 * -------------------------------------------------------------
 * These are example helper macros. In a full AUTOSAR stack, the Mcu driver
 * is responsible for peripheral clock initialization. The Gpt driver typically
 * assumes clocks for configured timer peripherals are already enabled.
 * These macros use register bit definitions now expected to be in stm32f3xx.h.
 * The (void)RCC->APBxENR after enabling is a read-back sometimes used to ensure
 * the write operation has completed due to bus behavior, effectively a small delay.
 * This is optional and can be removed if not deemed necessary or handled by other means.
 */

/* Example for TIM2 on APB1 */
#define GPT_HW_STM32_RCC_TIM2_CLK_ENABLE()    do { RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; (void)RCC->APB1ENR; } while(0)
#define GPT_HW_STM32_RCC_TIM2_CLK_DISABLE()   (RCC->APB1ENR &= (uint32)(~RCC_APB1ENR_TIM2EN))

/* Example for TIM3 on APB1 */
#define GPT_HW_STM32_RCC_TIM3_CLK_ENABLE()    do { RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; (void)RCC->APB1ENR; } while(0)
#define GPT_HW_STM32_RCC_TIM3_CLK_DISABLE()   (RCC->APB1ENR &= (uint32)(~RCC_APB1ENR_TIM3EN))

/* Example for TIM4 on APB1 */
#define GPT_HW_STM32_RCC_TIM4_CLK_ENABLE()    do { RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; (void)RCC->APB1ENR; } while(0)
#define GPT_HW_STM32_RCC_TIM4_CLK_DISABLE()   (RCC->APB1ENR &= (uint32)(~RCC_APB1ENR_TIM4EN))

/* Example for TIM6 on APB1 (Basic Timer) */
#define GPT_HW_STM32_RCC_TIM6_CLK_ENABLE()    do { RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; (void)RCC->APB1ENR; } while(0)
#define GPT_HW_STM32_RCC_TIM6_CLK_DISABLE()   (RCC->APB1ENR &= (uint32)(~RCC_APB1ENR_TIM6EN))

/* Example for TIM7 on APB1 (Basic Timer) */
#define GPT_HW_STM32_RCC_TIM7_CLK_ENABLE()    do { RCC->APB1ENR |= RCC_APB1ENR_TIM7EN; (void)RCC->APB1ENR; } while(0)
#define GPT_HW_STM32_RCC_TIM7_CLK_DISABLE()   (RCC->APB1ENR &= (uint32)(~RCC_APB1ENR_TIM7EN))

/* Add other timers as needed, considering APB1 vs APB2 bus, e.g.:
 * For TIM1, TIM8, TIM15, TIM16, TIM17 (Advanced/General Purpose on APB2 for STM32F303)
 * #define GPT_HW_STM32_RCC_TIM1_CLK_ENABLE()    do { RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; (void)RCC->APB2ENR; } while(0)
 * #define GPT_HW_STM32_RCC_TIM1_CLK_DISABLE()   (RCC->APB2ENR &= (uint32)(~RCC_APB2ENR_TIM1EN))
 */


/*
 * NVIC Interrupt Control
 * ----------------------
 * Enabling/disabling interrupts in the NVIC is typically done using CMSIS core functions
 * like NVIC_EnableIRQ(IRQn_Type IRQn) and NVIC_DisableIRQ(IRQn_Type IRQn).
 * These functions and IRQn_Type enum (e.g., TIM2_IRQn) are usually provided by
 * including the device-specific CMSIS header (e.g., "stm32f303xc.h" which includes "core_cm4.h").
 * Our stm32f3xx.h should ideally ensure these are available if it aims to be a primary device header.
 *
 * This Gpt_Hw_Stm32f3.h file will not redefine these CMSIS functions. Gpt.c should
 * use them directly if stm32f3xx.h (or its includes) provides them.
 */

/* Example IRQ Numbers (these are defined in device specific CMSIS header like stm32f303xc.h)
 * TIM2_IRQn
 * TIM3_IRQn
 * TIM4_IRQn
 * TIM6_DAC_IRQn  (TIM6 and DAC share an IRQ vector on STM32F303)
 * TIM7_IRQn      (Sometimes shares with DAC2 on some F3 series)
 */

#endif /* GPT_HW_STM32F3_H */
