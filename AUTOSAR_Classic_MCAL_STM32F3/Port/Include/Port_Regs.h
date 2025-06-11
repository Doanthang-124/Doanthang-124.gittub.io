#ifndef PORT_REGS_H
#define PORT_REGS_H

/* Include the specific device header file */
#include "../../Common/Include/stm32f3xx.h" // Contains GPIO_TypeDef, GPIOA, GPIOB etc.

/*
 * The Port driver will use the same GPIO register definitions (MODER, OTYPER, OSPEEDR, PUPDR, AFR)
 * as defined in stm32f3xx.h. No further Port-specific register macros are typically needed at this level
 * if stm32f3xx.h is comprehensive for GPIO.
 *
 * Example:
 * GPIOA->MODER can be used directly to configure the mode of pins in Port A.
 */

#endif /* PORT_REGS_H */
