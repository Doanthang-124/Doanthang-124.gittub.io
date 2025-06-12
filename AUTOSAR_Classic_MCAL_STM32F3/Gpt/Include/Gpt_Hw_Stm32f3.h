#ifndef GPT_HW_STM32F3_H
#define GPT_HW_STM32F3_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "Std_Types.h" /* For basic types, not strictly needed here but good practice */
                       /* stm32f3xx.h is included from Common/Include by Makefile path */
#include "stm32f3xx.h" /* Main STM32F3 device header (GPIO_TypeDef, TIM_TypeDef, RCC_TypeDef etc.) */

/*==================================================================================================
*                                          MACROS
==================================================================================================*/

/*
 * Helper macros for RCC clock enabling/disabling for timers.
 * In a full AUTOSAR stack, the Mcu driver would typically handle clock initialization.
 * These macros are provided for completeness or if the Gpt driver needs to manage
 * peripheral clocks directly in some limited scenarios.
 *
 * Note: Enabling a peripheral clock usually requires checking which bus it's on (APB1, APB2)
 * and using the correct RCC register (RCC->APB1ENR, RCC->APB2ENR).
 * STM32F3xx basic timers (TIM6, TIM7) and general-purpose timers (TIM2, TIM3, TIM4) are on APB1.
 * Advanced timers (TIM1, TIM8) are on APB2.
 */

/* Timers on APB1 bus */
#define GPT_STM32_RCC_TIM2_CLK_ENABLE()   do { RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;   (void)RCC->APB1ENR; } while(0) /* Delay after an RCC peripheral clock enabling */
#define GPT_STM32_RCC_TIM2_CLK_DISABLE()  (RCC->APB1ENR &= ~RCC_APB1ENR_TIM2EN)

#define GPT_STM32_RCC_TIM3_CLK_ENABLE()   do { RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;   (void)RCC->APB1ENR; } while(0)
#define GPT_STM32_RCC_TIM3_CLK_DISABLE()  (RCC->APB1ENR &= ~RCC_APB1ENR_TIM3EN)

#define GPT_STM32_RCC_TIM4_CLK_ENABLE()   do { RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;   (void)RCC->APB1ENR; } while(0)
#define GPT_STM32_RCC_TIM4_CLK_DISABLE()  (RCC->APB1ENR &= ~RCC_APB1ENR_TIM4EN)

/* TIM5 is not available on all STM32F303 variants, check datasheet. If available, usually APB1. */
// #define GPT_STM32_RCC_TIM5_CLK_ENABLE()   do { RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;   (void)RCC->APB1ENR; } while(0)
// #define GPT_STM32_RCC_TIM5_CLK_DISABLE()  (RCC->APB1ENR &= ~RCC_APB1ENR_TIM5EN)

#define GPT_STM32_RCC_TIM6_CLK_ENABLE()   do { RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;   (void)RCC->APB1ENR; } while(0)
#define GPT_STM32_RCC_TIM6_CLK_DISABLE()  (RCC->APB1ENR &= ~RCC_APB1ENR_TIM6EN)

#define GPT_STM32_RCC_TIM7_CLK_ENABLE()   do { RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;   (void)RCC->APB1ENR; } while(0)
#define GPT_STM32_RCC_TIM7_CLK_DISABLE()  (RCC->APB1ENR &= ~RCC_APB1ENR_TIM7EN)


/* Timers on APB2 bus (example: TIM1, TIM15, TIM16, TIM17 on STM32F303) */
#define GPT_STM32_RCC_TIM1_CLK_ENABLE()    do { RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;    (void)RCC->APB2ENR; } while(0)
#define GPT_STM32_RCC_TIM1_CLK_DISABLE()   (RCC->APB2ENR &= ~RCC_APB2ENR_TIM1EN)

#define GPT_STM32_RCC_TIM15_CLK_ENABLE()   do { RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;   (void)RCC->APB2ENR; } while(0)
#define GPT_STM32_RCC_TIM15_CLK_DISABLE()  (RCC->APB2ENR &= ~RCC_APB2ENR_TIM15EN)

#define GPT_STM32_RCC_TIM16_CLK_ENABLE()   do { RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;   (void)RCC->APB2ENR; } while(0)
#define GPT_STM32_RCC_TIM16_CLK_DISABLE()  (RCC->APB2ENR &= ~RCC_APB2ENR_TIM16EN)

#define GPT_STM32_RCC_TIM17_CLK_ENABLE()   do { RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;   (void)RCC->APB2ENR; } while(0)
#define GPT_STM32_RCC_TIM17_CLK_DISABLE()  (RCC->APB2ENR &= ~RCC_APB2ENR_TIM17EN)


/*
 * NVIC Interrupt Enable/Disable function-like macros.
 * These rely on CMSIS core functions typically included via stm32f3xx.h -> stm32f303xc.h -> core_cm4.h.
 * Ensure these CMSIS functions are available in the build path.
 * The IRQn_Type enum values (e.g., TIM2_IRQn) are device-specific, defined in stm32f303xc.h.
 */
// Example:
// #define Gpt_EnableNvicInterrupt(IRQn)   NVIC_EnableIRQ(IRQn)
// #define Gpt_DisableNvicInterrupt(IRQn)  NVIC_DisableIRQ(IRQn)
// #define Gpt_SetNvicPriority(IRQn, Prio) NVIC_SetPriority(IRQn, Prio)

#endif /* GPT_HW_STM32F3_H */
