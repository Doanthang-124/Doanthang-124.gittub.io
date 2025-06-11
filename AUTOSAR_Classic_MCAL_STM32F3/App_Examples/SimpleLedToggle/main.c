/**
 * @file main.c
 * @brief Simple LED Toggle Example for AUTOSAR Dio and Port drivers.
 *
 * This application demonstrates basic initialization and usage of Port and Dio
 * drivers to toggle an LED connected to a GPIO pin.
 */

/* MCAL Driver Includes */
#include "../../Include/Port.h"         /* Port Driver public header */
#include "../../Include/Dio.h"          /* Dio Driver public header */

/* Configuration Includes */
/* These headers declare the 'Port_Config' and 'Dio_Config' structures. */
#include "../../Config/Port_PBcfg.h"    /* Port Driver Post-Build Configuration declarations */
#include "../../Config/Dio_PBcfg.h"     /* Dio Driver Post-Build Configuration declarations */

/* For uint32 definition for the delay function parameter */
#include "../../Common/Include/Std_Types.h"


/* Symbolic name for the LED channel, ensure this is defined in Dio_Cfg.h
   and corresponds to a pin configured as output in Port_PBcfg.c.
   Example: DIO_CHANNEL_PA5 (Pin PA5)
*/
#ifndef LED_CHANNEL
    // Assuming PA5 is configured as an LED output in Port_PBcfg.c
    // and DIO_CHANNEL_PA5 is defined in Dio_Cfg.h
    #define LED_CHANNEL DIO_CHANNEL_PA5
#endif


/**
 * @brief Simple software delay function.
 * @param count Loop iterations to perform for delay.
 *
 * This is a blocking delay. In a real application, timers or OS delays
 * would be preferred.
 */
static void simple_delay(volatile uint32 count)
{
    while (count > 0U)
    {
        count--;
    }
}

/**
 * @brief Main function for the LED toggle application.
 *
 * Initializes Port and Dio drivers, then enters an infinite loop
 * to toggle the configured LED channel.
 */
int main(void)
{
    /* Initialize the Port driver with the post-build configuration. */
    /* This function configures all port pins according to Port_PinConfigurations. */
    Port_Init(&Port_Config);

    /* Initialize the Dio driver with its post-build configuration. */
    /* This makes the Dio configuration available to Dio functions. */
    Dio_Init(&Dio_Config);

    /* Application's main loop */
    while (1)
    {
        /* Toggle the LED channel state. */
        /* Dio_FlipChannel reads the current state and writes the opposite. */
        (void)Dio_FlipChannel(LED_CHANNEL);

        /* Wait for some time to make the blinking visible. */
        simple_delay(500000U); /* Adjust this value for desired blinking speed. */
                               /* With typical MCU clocks (e.g., 8-72MHz), this might be
                                  in the range of 100ms to 500ms. */
    }

    /* The program should not typically reach here in an embedded system. */
    return 0;
}
