/*==================================================================================================
*   Project              : AUTOSAR 4.x MCAL STM32F3 Sample Application
*   Platform             : STM32
*   Peripheral           : GPIO, GPT
*   Dependencies         : Dio, Port, Gpt drivers
*
*   Autosar Version      : 4.3.1
*   Sw Version           : 1.0.0
*
==================================================================================================*/

/* MCAL Driver Headers */
#include "Dio.h"         /* For Dio_Init, Dio_WriteChannel, Dio_ReadChannel */
#include "Port.h"       /* For Port_Init */
#include "Gpt.h"         /* For Gpt_Init, Gpt_StartTimer, Gpt_GetTimeRemaining */

/* Configuration Headers for MCAL drivers */
#include "Dio_Cfg.h"        /* For DIO_CHANNEL_PA5, DIO_CHANNEL_PC13 symbolic names */
#include "Port_Cfg.h"       /* (Not directly used for symbols here, but good practice if general Port settings were needed) */
#include "Gpt_Cfg.h"        /* For GPT_CHANNEL_1, GPT_ENABLE_DISABLE_NOTIFICATION_API */

#include "Dio_PBcfg.h"      /* For extern DioConfig */
#include "Port_PBcfg.h"     /* For extern PortConfig */
#include "Gpt_PBcfg.h"      /* For extern GptConfig */

/* Std_Types.h and Compiler.h are included via the MCAL driver headers */


/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

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


/*==================================================================================================
*                                       GLOBAL FUNCTIONS (main)
==================================================================================================*/

/**
 * @brief Main function for the Simple LED Toggle application.
 * @details Initializes Port, Dio, and Gpt drivers.
 *          Toggles an LED (PA5) based on a button input (PC13).
 *          Uses GPT channel (GPT_CHANNEL_1, mapped to TIM6) for delays.
 *
 * @return int Standard main return type (typically not used in embedded).
 *
 * @note This application assumes specific configurations in Port_PBcfg.c and Dio_Cfg.h:
 *       - PA5 (DIO_CHANNEL_PA5) is configured as a GPIO output for the LED.
 *       - PC13 (DIO_CHANNEL_PC13) is configured as a GPIO input with pull-up for the button.
 *       - GPT_CHANNEL_1 is configured in Gpt_PBcfg.c (e.g., TIM6, 1kHz tick, one-shot mode).
 */
FUNC(int, APPL_CODE) main(void)
{
    VAR(Dio_LevelType, AUTOMATIC) u8ButtonState;
    VAR(Gpt_ValueType, AUTOMATIC) u32DelayTicks; /* Using Gpt_ValueType (uint32) */

    /* Initialize MCAL Drivers */
    Port_Init(&PortConfig); /* PortConfig is extern const from Port_PBcfg.h */
    Dio_Init(&DioConfig);   /* DioConfig is extern const from Dio_PBcfg.h */
    Gpt_Init(&GptConfig);   /* GptConfig is extern const from Gpt_PBcfg.h */

    /* Infinite loop for application logic */
    while(1)
    {
        u8ButtonState = Dio_ReadChannel(BUTTON_CHANNEL);

        if (u8ButtonState == STD_LOW) /* Assuming button press pulls PC13 (with pull-up) LOW */
        {
            u32DelayTicks = 100UL;  /* 0.1s delay for TIM6 (configured at 1kHz tick in Gpt_PBcfg.c) */
        }
        else /* Button not pressed (PC13 is HIGH due to pull-up) */
        {
            u32DelayTicks = 500UL;  /* 0.5s delay for TIM6 (configured at 1kHz tick) */
        }

        /* Turn LED ON */
        Dio_WriteChannel(LED_CHANNEL, STD_HIGH);

        /* Delay using GPT Channel 1 (configured as one-shot) */
        Gpt_StartTimer(DELAY_GPT_CHANNEL, u32DelayTicks);
        while(Gpt_GetTimeRemaining(DELAY_GPT_CHANNEL) > 0U)
        {
            /* Poll until timer channel finishes. */
        }
        /* Gpt_StopTimer(DELAY_GPT_CHANNEL); // Not strictly necessary for one-shot mode as it stops automatically. */


        /* Turn LED OFF */
        Dio_WriteChannel(LED_CHANNEL, STD_LOW);

        /* Delay again using GPT Channel 1 */
        Gpt_StartTimer(DELAY_GPT_CHANNEL, u32DelayTicks);
        while(Gpt_GetTimeRemaining(DELAY_GPT_CHANNEL) > 0U)
        {
            /* Poll. */
        }
        /* Gpt_StopTimer(DELAY_GPT_CHANNEL); */
    }

    /* return 0; // Unreachable in embedded applications with while(1) loop. */
}


/*==================================================================================================
*                                  GPT NOTIFICATION CALLBACKS (STUBS)
==================================================================================================*/

#if (defined(GPT_ENABLE_DISABLE_NOTIFICATION_API) && (GPT_ENABLE_DISABLE_NOTIFICATION_API == STD_ON))
/**
 * @brief Example notification callback for GPT Channel 0.
 * @details This function is called from the GPT ISR when Channel 0 expires,
 *          if notifications are enabled for this channel.
 *          Defined here as a stub to allow linking if Gpt_PBcfg.c references it.
 *          The user application would provide the actual implementation.
 */
FUNC(void, APPL_CODE) Gpt_Notification_Channel0(void)
{
    /* Example: Increment a counter, set a flag, or toggle an I/O. */
    (void)0; /* No operation for this stub - explicit void cast to suppress warnings. */
}

/**
 * @brief Example notification callback for GPT Channel 1.
 * @details This function is called from the GPT ISR when Channel 1 expires,
 *          if notifications are enabled for this channel.
 *          Defined here as a stub to allow linking if Gpt_PBcfg.c references it.
 */
FUNC(void, APPL_CODE) Gpt_Notification_Channel1(void)
{
    (void)0; /* No operation for this stub. */
}

/* Add other notification function stubs if configured in Gpt_PBcfg.c and used */

#endif /* GPT_ENABLE_DISABLE_NOTIFICATION_API == STD_ON */
