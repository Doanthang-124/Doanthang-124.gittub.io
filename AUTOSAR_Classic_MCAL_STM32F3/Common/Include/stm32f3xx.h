#ifndef STM32F3XX_H
#define STM32F3XX_H

/*
 * Basic Device Header for STM32F3xx Series (Example focusing on GPIO, RCC, Basic Timers)
 * This is a simplified version. For a full project, use STMicroelectronics official headers (CMSIS).
 */

#include "Std_Types.h" /* For uint32, and via Std_Types->Compiler.h for _VOLATILE_ */

/*==================================================================================================
*                                       GPIO DEFINITIONS
==================================================================================================*/

/**
 * @brief General Purpose I/O
 */
typedef struct
{
  _VOLATILE_ uint32 MODER;    /*!< GPIO port mode register,                 Address offset: 0x00 */
  _VOLATILE_ uint32 OTYPER;   /*!< GPIO port output type register,          Address offset: 0x04 */
  _VOLATILE_ uint32 OSPEEDR;  /*!< GPIO port output speed register,         Address offset: 0x08 */
  _VOLATILE_ uint32 PUPDR;    /*!< GPIO port pull-up/pull-down register,    Address offset: 0x0C */
  _VOLATILE_ uint32 IDR;      /*!< GPIO port input data register,           Address offset: 0x10 */
  _VOLATILE_ uint32 ODR;      /*!< GPIO port output data register,          Address offset: 0x14 */
  _VOLATILE_ uint32 BSRR;     /*!< GPIO port bit set/reset register,        Address offset: 0x18 */
  _VOLATILE_ uint32 LCKR;     /*!< GPIO port configuration lock register,   Address offset: 0x1C */
  _VOLATILE_ uint32 AFR[2];   /*!< GPIO alternate function registers (AFRL, AFRH), Address offset: 0x20-0x24 */
  /* _VOLATILE_ uint32 BRR; */     /*!< GPIO port bit reset register,            Address offset: 0x28 (covered by BSRR high word) */
} GPIO_TypeDef;


/* Peripheral Memory Map Base Addresses */
#define PERIPH_BASE           (0x40000000UL) /*!< Base address for APB, AHB peripherals */

/* AHB2 Bus Peripherals */
#define AHB2PERIPH_BASE       (PERIPH_BASE + 0x08000000UL)
#define GPIOA_BASE            (AHB2PERIPH_BASE + 0x0000UL)
#define GPIOB_BASE            (AHB2PERIPH_BASE + 0x0400UL)
#define GPIOC_BASE            (AHB2PERIPH_BASE + 0x0800UL)
#define GPIOD_BASE            (AHB2PERIPH_BASE + 0x0C00UL)
#define GPIOE_BASE            (AHB2PERIPH_BASE + 0x1000UL)
#define GPIOF_BASE            (AHB2PERIPH_BASE + 0x1400UL)
/* GPIOG, GPIOH not typically on STM32F303 */

/* GPIO Peripheral Pointer Definitions */
#define GPIOA                 ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB                 ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC                 ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD                 ((GPIO_TypeDef *)GPIOD_BASE)
#define GPIOE                 ((GPIO_TypeDef *)GPIOE_BASE)
#define GPIOF                 ((GPIO_TypeDef *)GPIOF_BASE)

/*==================================================================================================
*                                       RCC DEFINITIONS
==================================================================================================*/

/**
 * @brief Reset and Clock Control
 */
typedef struct
{
  _VOLATILE_ uint32 CR;         /*!< RCC clock control register,                                   Address offset: 0x00 */
  _VOLATILE_ uint32 CFGR;       /*!< RCC clock configuration register,                               Address offset: 0x04 */
  _VOLATILE_ uint32 CIR;        /*!< RCC clock interrupt register,                                   Address offset: 0x08 */
  _VOLATILE_ uint32 APB2RSTR;   /*!< RCC APB2 peripheral reset register,                             Address offset: 0x0C */
  _VOLATILE_ uint32 APB1RSTR;   /*!< RCC APB1 peripheral reset register,                             Address offset: 0x10 */
  _VOLATILE_ uint32 AHBENR;     /*!< RCC AHB peripheral clock enable register,                       Address offset: 0x14 */
  _VOLATILE_ uint32 APB2ENR;    /*!< RCC APB2 peripheral clock enable register,                      Address offset: 0x18 */
  _VOLATILE_ uint32 APB1ENR;    /*!< RCC APB1 peripheral clock enable register,                      Address offset: 0x1C */
  _VOLATILE_ uint32 BDCR;       /*!< RCC Backup domain control register,                             Address offset: 0x20 */
  _VOLATILE_ uint32 CSR;        /*!< RCC clock control & status register,                            Address offset: 0x24 */
  _VOLATILE_ uint32 AHBRSTR;    /*!< RCC AHB peripheral reset register,                              Address offset: 0x28 */
  _VOLATILE_ uint32 CFGR2;      /*!< RCC clock configuration register 2,                             Address offset: 0x2C */
  _VOLATILE_ uint32 CFGR3;      /*!< RCC clock configuration register 3,                             Address offset: 0x30 */
} RCC_TypeDef;

#define RCC_BASE              (0x40021000UL) /*!< RCC base address (STM32F303x) */
#define RCC                   ((RCC_TypeDef *)RCC_BASE)

/* RCC_APB1ENR bit definitions for Timer clocks */
#define RCC_APB1ENR_TIM2EN_Pos    (0U)                                /*!< TIM2 clock enable bit position */
#define RCC_APB1ENR_TIM2EN_Msk    (0x1UL << RCC_APB1ENR_TIM2EN_Pos)   /*!< TIM2 clock enable bit Mask */
#define RCC_APB1ENR_TIM2EN        RCC_APB1ENR_TIM2EN_Msk             /*!< TIM2 clock enable */

#define RCC_APB1ENR_TIM3EN_Pos    (1U)
#define RCC_APB1ENR_TIM3EN_Msk    (0x1UL << RCC_APB1ENR_TIM3EN_Pos)
#define RCC_APB1ENR_TIM3EN        RCC_APB1ENR_TIM3EN_Msk

#define RCC_APB1ENR_TIM4EN_Pos    (2U)
#define RCC_APB1ENR_TIM4EN_Msk    (0x1UL << RCC_APB1ENR_TIM4EN_Pos)
#define RCC_APB1ENR_TIM4EN        RCC_APB1ENR_TIM4EN_Msk

/* TIM5 is not on all STM32F303. If it were:
#define RCC_APB1ENR_TIM5EN_Pos    (3U)
#define RCC_APB1ENR_TIM5EN_Msk    (0x1UL << RCC_APB1ENR_TIM5EN_Pos)
#define RCC_APB1ENR_TIM5EN        RCC_APB1ENR_TIM5EN_Msk
*/

#define RCC_APB1ENR_TIM6EN_Pos    (4U)
#define RCC_APB1ENR_TIM6EN_Msk    (0x1UL << RCC_APB1ENR_TIM6EN_Pos)
#define RCC_APB1ENR_TIM6EN        RCC_APB1ENR_TIM6EN_Msk

#define RCC_APB1ENR_TIM7EN_Pos    (5U)
#define RCC_APB1ENR_TIM7EN_Msk    (0x1UL << RCC_APB1ENR_TIM7EN_Pos)
#define RCC_APB1ENR_TIM7EN        RCC_APB1ENR_TIM7EN_Msk

/* RCC_APB2ENR bit definitions for Timer clocks (example for TIM1) */
#define RCC_APB2ENR_TIM1EN_Pos    (11U)
#define RCC_APB2ENR_TIM1EN_Msk    (0x1UL << RCC_APB2ENR_TIM1EN_Pos)
#define RCC_APB2ENR_TIM1EN        RCC_APB2ENR_TIM1EN_Msk

/*==================================================================================================
*                                       TIMER DEFINITIONS
==================================================================================================*/

/**
 * @brief General Purpose Timer (Simplified for GPT usage)
 */
typedef struct
{
  _VOLATILE_ uint32 CR1;         /*!< TIM control register 1,              Address offset: 0x00 */
  _VOLATILE_ uint32 CR2;         /*!< TIM control register 2,              Address offset: 0x04 */
  _VOLATILE_ uint32 SMCR;        /*!< TIM slave mode control register,     Address offset: 0x08 */
  _VOLATILE_ uint32 DIER;        /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
  _VOLATILE_ uint32 SR;          /*!< TIM status register,                 Address offset: 0x10 */
  _VOLATILE_ uint32 EGR;         /*!< TIM event generation register,       Address offset: 0x14 */
  _VOLATILE_ uint32 CCMR1;       /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
  _VOLATILE_ uint32 CCMR2;       /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
  _VOLATILE_ uint32 CCER;        /*!< TIM capture/compare enable register, Address offset: 0x20 */
  _VOLATILE_ uint32 CNT;         /*!< TIM counter register,                Address offset: 0x24 */
  _VOLATILE_ uint32 PSC;         /*!< TIM prescaler register,              Address offset: 0x28 */
  _VOLATILE_ uint32 ARR;         /*!< TIM auto-reload register,            Address offset: 0x2C */
  /* Other registers (RCR, CCRx, BDTR, DCR, DMAR etc.) are omitted for this simplified GPT usage. */
  /* A real project should use the full TIM_TypeDef from ST's CMSIS headers. */
} TIM_GeneralPurpose_TypeDef;

/* Base addresses for APB1 Timers (STM32F303xC) */
#define APB1PERIPH_BASE       (PERIPH_BASE) /* APB1 Peripherals start at PERIPH_BASE */
#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000UL)
#define TIM3_BASE             (APB1PERIPH_BASE + 0x0400UL)
#define TIM4_BASE             (APB1PERIPH_BASE + 0x0800UL)
#define TIM6_BASE             (APB1PERIPH_BASE + 0x1000UL)
#define TIM7_BASE             (APB1PERIPH_BASE + 0x1400UL)

/* Pointers to Timer Peripheral Structures */
#define TIM2                  ((TIM_GeneralPurpose_TypeDef *) TIM2_BASE)
#define TIM3                  ((TIM_GeneralPurpose_TypeDef *) TIM3_BASE)
#define TIM4                  ((TIM_GeneralPurpose_TypeDef *) TIM4_BASE)
#define TIM6                  ((TIM_GeneralPurpose_TypeDef *) TIM6_BASE)
#define TIM7                  ((TIM_GeneralPurpose_TypeDef *) TIM7_BASE)

/* Timer Register Bit Definitions (used by Gpt.c) */
/* TIM_CR1 Register Bits */
#define TIM_CR1_CEN_Pos           (0U)                                /*!< Counter enable bit position */
#define TIM_CR1_CEN_Msk           (0x1UL << TIM_CR1_CEN_Pos)          /*!< Counter enable bit Mask */
#define TIM_CR1_CEN               TIM_CR1_CEN_Msk                    /*!< Counter enable */
#define TIM_CR1_OPM_Pos           (3U)                                /*!< One-pulse mode bit position */
#define TIM_CR1_OPM_Msk           (0x1UL << TIM_CR1_OPM_Pos)          /*!< One-pulse mode bit Mask */
#define TIM_CR1_OPM               TIM_CR1_OPM_Msk                    /*!< One-pulse mode */
#define TIM_CR1_DIR_Pos           (4U)                                /*!< Direction bit position */
#define TIM_CR1_DIR_Msk           (0x1UL << TIM_CR1_DIR_Pos)          /*!< Direction bit Mask */
#define TIM_CR1_DIR               TIM_CR1_DIR_Msk                    /*!< Direction */
#define TIM_CR1_CMS_Pos           (5U)                                /*!< Center-aligned mode selection bit position */
#define TIM_CR1_CMS_Msk           (0x3UL << TIM_CR1_CMS_Pos)          /*!< Center-aligned mode selection bit Mask */
#define TIM_CR1_CMS               TIM_CR1_CMS_Msk                    /*!< Center-aligned mode selection */

/* TIM_DIER Register Bits */
#define TIM_DIER_UIE_Pos          (0U)                                /*!< Update interrupt enable bit position */
#define TIM_DIER_UIE_Msk          (0x1UL << TIM_DIER_UIE_Pos)         /*!< Update interrupt enable bit Mask */
#define TIM_DIER_UIE              TIM_DIER_UIE_Msk                   /*!< Update interrupt enable */

/* TIM_SR Register Bits */
#define TIM_SR_UIF_Pos            (0U)                                /*!< Update interrupt Flag bit position */
#define TIM_SR_UIF_Msk            (0x1UL << TIM_SR_UIF_Pos)           /*!< Update interrupt Flag bit Mask */
#define TIM_SR_UIF                TIM_SR_UIF_Msk                     /*!< Update interrupt Flag */

/* TIM_EGR Register Bits */
#define TIM_EGR_UG_Pos            (0U)                                /*!< Update generation bit position */
#define TIM_EGR_UG_Msk            (0x1UL << TIM_EGR_UG_Pos)           /*!< Update generation bit Mask */
#define TIM_EGR_UG                TIM_EGR_UG_Msk                     /*!< Update generation */

#endif /* STM32F3XX_H */
