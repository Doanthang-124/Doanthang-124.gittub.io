/**
 * @file main.c
 * @brief Simple LED Toggle Example for AUTOSAR Dio, Port, and Gpt drivers.
 *
 * This application demonstrates:
 * - Initialization of Port, Dio, and Gpt drivers.
 * - Reading a button state using Dio_ReadChannel.
 * - Controlling an LED using Dio_WriteChannel.
 * - Creating delays using Gpt_StartTimer and polling Gpt_GetTimeRemaining.
 * - Adjusting LED blink speed based on button input.
 */

/* MCAL Driver Includes */
#include "Port.h"         /* Port Driver public header */
#include "Dio.h"          /* Dio Driver public header */
#include "Gpt.h"          /* Gpt Driver public header */

/* Configuration Includes */
/* These headers declare the 'Xxx_Config' structures needed by Xxx_Init(). */
#include "Port_PBcfg.h"    /* Port Driver Post-Build Configuration declarations */
#include "Dio_PBcfg.h"     /* Dio Driver Post-Build Configuration declarations */
#include "Gpt_PBcfg.h"     /* Gpt Driver Post-Build Configuration declarations */

/* Pre-compile Configuration Includes (for symbolic names and API switches) */
#include "Dio_Cfg.h"       /* For DIO_CHANNEL_xxx symbolic names */
#include "Gpt_Cfg.h"       /* For GPT_CHANNEL_xxx and GPT_xxx_API macros */

/* Common Includes (already included via MCAL headers, but explicit for clarity if needed) */
/* #include "Std_Types.h" */ /* For uint32, FUNC, VAR if not via others */
/* #include "Compiler.h" */  /* For FUNC, VAR macros */


/* Symbolic name for the LED channel (e.g., PA5) */
#ifndef LED_CHANNEL
    #define LED_CHANNEL DIO_CHANNEL_PA5
#endif

/* Symbolic name for the Button channel (e.g., PC13) */
#ifndef BUTTON_CHANNEL
    #define BUTTON_CHANNEL DIO_CHANNEL_PC13
#endif

/* GPT Channel used for delay - assumed to be TIM6 (16-bit, one-shot, 1kHz tick from Gpt_PBcfg.c) */
#ifndef DELAY_GPT_CHANNEL
    #define DELAY_GPT_CHANNEL GPT_CHANNEL_1
#endif


/**
 * @brief Main function for the LED toggle application with GPT-based delay.
 */
FUNC(int, APPL_CODE) main(void)
{
    VAR(Dio_LevelType, AUTOMATIC) buttonState;
    VAR(Gpt_ValueType, AUTOMATIC) delay_ticks; /* Gpt_ValueType is typically uint32 */

    /* Initialize MCAL drivers */
    Port_Init(&Port_Config);    /* Configure pins (direction, mode, etc.) */
    Dio_Init(&Dio_Config);      /* Initialize Dio driver (if any internal state needed) */
    Gpt_Init(&Gpt_Config);      /* Initialize Gpt driver (configures timers) */

    /* Application's main loop */
    while(1)
    {
        /* Read the state of the button */
        /* Assuming PC13 (BUTTON_CHANNEL) is configured with a pull-up:
           - Not pressed: STD_HIGH
           - Pressed (to GND): STD_LOW
        */
        buttonState = Dio_ReadChannel(BUTTON_CHANNEL);

        /* Adjust delay based on button state */
        if (buttonState == STD_LOW) /* Button is pressed */
        {
            delay_ticks = 100UL;  /* Faster blink: 0.1s delay (100 ticks * 1ms/tick for 1kHz GPT channel) */
        }
        else /* Button is not pressed */
        {
            delay_ticks = 500UL;  /* Slower blink: 0.5s delay (500 ticks * 1ms/tick) */
        }

        /* Turn LED ON */
        Dio_WriteChannel(LED_CHANNEL, STD_HIGH);

        /* Delay using GPT Channel (configured as one-shot) */
        Gpt_StartTimer(DELAY_GPT_CHANNEL, delay_ticks);
        while(Gpt_GetTimeRemaining(DELAY_GPT_CHANNEL) > 0U)
        {
            /* Poll until timer channel finishes. */
            /* In a more advanced system, could use WFI (Wait For Interrupt) here if
               GPT notifications were configured to wake the CPU from a low-power mode. */
        }
        /* Gpt_StopTimer(DELAY_GPT_CHANNEL); // Not strictly necessary for one-shot mode as it stops automatically. */


        /* Turn LED OFF */
        Dio_WriteChannel(LED_CHANNEL, STD_LOW);

        /* Delay again using the same GPT Channel */
        Gpt_StartTimer(DELAY_GPT_CHANNEL, delay_ticks);
        while(Gpt_GetTimeRemaining(DELAY_GPT_CHANNEL) > 0U)
        {
            /* Poll */
        }
        /* Gpt_StopTimer(DELAY_GPT_CHANNEL); */
    }

    /* return 0; // Should not be reached in an embedded system. */
}


#if (GPT_ENABLE_DISABLE_NOTIFICATION_API == STD_ON) /* From Gpt_Cfg.h */
/**
 * @brief Example/Stub notification function for GPT Channel 0.
 * @details This function is declared as extern in Gpt_PBcfg.c if configured.
 *          The application must provide its definition.
 *          This function would be called from the GPT ISR if notifications are enabled
 *          for this channel and the timer event occurs.
 */
FUNC(void, APPL_CODE) Gpt_Notification_Channel0(void)
{
    /* Example action: Toggle a different LED, set a flag, etc. */
    /* For this main.c, it's just a stub to allow linking if Gpt_PBcfg.c refers to it. */
    (void)0; /* No operation */
}

/**
 * @brief Example/Stub notification function for GPT Channel 1.
 */
FUNC(void, APPL_CODE) Gpt_Notification_Channel1(void)
{
    /* Example action for GPT Channel 1 notification. */
    (void)0; /* No operation */
}

/* Add definitions for other notification functions if configured in Gpt_PBcfg.c */
#endif /* GPT_ENABLE_DISABLE_NOTIFICATION_API */
