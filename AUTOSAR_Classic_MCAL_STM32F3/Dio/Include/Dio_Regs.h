#ifndef DIO_REGS_H
#define DIO_REGS_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/

/**
 * @file    Dio_Regs.h
 * @brief   Header file for direct register access used by the DIO driver.
 * @details This file includes the main device header (`stm32f3xx.h`) which contains
 *          the definitions for GPIO peripheral registers. It serves as an abstraction
 *          point for register definitions specific to the DIO module, though for basic
 *          GPIO access, it often defers entirely to the device header.
 */

/* Include the main STM32F3 device header for GPIO register definitions */
#include "../../Common/Include/stm32f3xx.h"
/* Through stm32f3xx.h (which includes Std_Types.h, which includes Compiler.h),
   macros like _VOLATILE_ are available if needed for any direct register access
   macros defined here in the future.
*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/*
 * No Dio-specific register access macros are defined here for now.
 * The Dio driver implementation (Dio.c) will directly use the peripheral
 * pointers (e.g., GPIOA, GPIOB) and register structure (GPIO_TypeDef)
 * defined in "stm32f3xx.h".
 *
 * If Dio required specific bit masks or register utility macros not provided
 * by the main device header, they would be defined here.
 * Example: #define DIO_PIN_X_MASK (0x01U << PIN_X_OFFSET)
 */

#endif /* DIO_REGS_H */
