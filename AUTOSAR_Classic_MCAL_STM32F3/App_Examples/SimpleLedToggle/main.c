#include "Port.h"       /* For Port_Init */
#include "Dio.h"        /* For Dio_Init, Dio_FlipChannel, Dio_ReadChannel */
#include "Port_PBcfg.h" /* To make PortConfig available */
#include "Dio_PBcfg.h"  /* To make DioConfig available */
#include "Dio_Cfg.h"    /* For DIO_CHANNEL_PA5, DIO_CHANNEL_PC13 symbolic names */
#include "../../Common/Include/Std_Types.h" /* For uint32, potentially FUNC, VAR if not via others */
#include "../../Common/Include/Compiler.h" /* For FUNC, VAR macros */


// A simple software delay function
FUNC(void, APPL_CODE) simple_delay(VAR(volatile uint32, AUTOMATIC) count) {
    while(count--) {
        // Just loop to consume time
    }
}

FUNC(int, APPL_CODE) main(void) {
    VAR(Dio_LevelType, AUTOMATIC) buttonState;
    VAR(uint32, AUTOMATIC) delay_val = 500000; // Default delay

    // Initialize the Port driver with the default configuration
    Port_Init(&Port_Config);

    // Initialize the Dio driver with its configuration
    Dio_Init(&Dio_Config);

    // Application loop
    while(1) {
        // Read the state of the button connected to PC13
        // Assuming PC13 is configured with a pull-up, it reads HIGH when not pressed, LOW when pressed.
        buttonState = Dio_ReadChannel(DIO_CHANNEL_PC13); // DIO_CHANNEL_PC13 should be defined in Dio_Cfg.h

        if (buttonState == STD_LOW) { // Button is pressed
            delay_val = 100000; // Faster blink rate
        } else { // Button is not pressed
            delay_val = 500000; // Slower blink rate
        }

        // Flip the state of the LED connected to PA5
        Dio_FlipChannel(DIO_CHANNEL_PA5); // DIO_CHANNEL_PA5 should be defined in Dio_Cfg.h

        // Delay
        simple_delay(delay_val);
    }

    // return 0; // Should not be reached in an embedded system
}
