#ifndef PORT_REGS_H
#define PORT_REGS_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/

/**
 * @file    Port_Regs.h
 * @brief   Header file for direct register access used by the PORT driver.
 * @details This file includes the main device header (`stm32f3xx.h`) which contains
 *          the definitions for GPIO peripheral registers (MODER, OTYPER, OSPEEDR, PUPDR, AFRx, etc.).
 *          The Port driver uses these definitions to configure individual pin properties.
 *          It serves as an abstraction point for register definitions specific to the PORT module,
 *          though for basic GPIO configuration, it often defers entirely to the device header.
 */

/* Include the main STM32F3 device header for GPIO register definitions */
#include "../../Common/Include/stm32f3xx.h"
/* Through stm32f3xx.h (which includes Std_Types.h, which includes Compiler.h),
   macros like _VOLATILE_ are available. The GPIO_TypeDef in stm32f3xx.h
   should use _VOLATILE_ for all its register members.
*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/*
 * No Port-driver-specific register access macros or alternative register views are defined here for now.
 * The Port driver implementation (Port.c) will directly use the peripheral
 * pointers (e.g., GPIOA, GPIOB) and register structure (GPIO_TypeDef)
 * defined in "stm32f3xx.h" to configure pin modes, speeds, pull resistors,
 * alternate functions, etc.
 *
 * If the Port driver required specific bit masks for combined register settings
 * or convenience macros not provided by the main device header, they would be defined here.
 * Example: #define PORT_MODER_PINX_OUTPUT_MASK (0x01UL << (PINX_OFFSET * 2))
 */

#endif /* PORT_REGS_H */
