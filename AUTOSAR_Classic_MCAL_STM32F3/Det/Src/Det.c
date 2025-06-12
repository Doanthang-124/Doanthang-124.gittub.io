/*==================================================================================================
*   Project              : AUTOSAR 4.x MCAL
*   Platform             : STM32
*   Peripheral           : DET
*   Dependencies         : Standard Types
*
*   Autosar Version      : 4.3.1
*   Autosar Revision     : ASR_REL_4_3_REV_0001
*   Sw Version           : 1.0.0
*   Build Version        : DET_MCAL_1_0_0_RTM_ASR_REL_4_3_REV_0001_2024xxxx
*
==================================================================================================*/

#include "Det.h" /* For Det_ErrorType, API declarations, DET_MODULE_ID etc. */
                 /* Includes Std_Types.h, Compiler.h (via Std_Types), Det_Cfg.h */

/*==================================================================================================
*                                       VERSION CHECKS
==================================================================================================*/
/* Add checks for DET_SW_MAJOR_VERSION, etc. if defined in Det.h */
/* Example:
#if ((DET_SW_MAJOR_VERSION_C != DET_SW_MAJOR_VERSION) || \
     (DET_SW_MINOR_VERSION_C != DET_SW_MINOR_VERSION) || \
     (DET_SW_PATCH_VERSION_C != DET_SW_PATCH_VERSION))
    #error "Software Version Numbers of Det.c and Det.h are different"
#endif
*/

/*==================================================================================================
*                                          VARIABLES
==================================================================================================*/

#if (DET_DEV_ERROR_DETECT == STD_ON) /* From Det_Cfg.h */

#if (DET_LOG_ERRORS_API == STD_ON) /* From Det_Cfg.h */
/**
 * @brief Buffer to store reported development errors.
 * @details Static variable, zero-initialized by default.
 *          Size is defined by DET_ERROR_BUFFER_SIZE in Det_Cfg.h.
 *          Uses AUTOSAR VAR macro with DET_VAR memory class.
 */
STATIC VAR(Det_ErrorType, DET_VAR) Det_aErrorBuffer[DET_ERROR_BUFFER_SIZE];

/**
 * @brief Index to the next available slot in Det_aErrorBuffer for writing.
 * @details Wraps around to implement a circular buffer.
 */
STATIC VAR(uint8, DET_VAR) Det_u8ErrorBufferIndex = 0U;

/**
 * @brief Number of valid error entries currently stored in the buffer.
 * @details This count will not exceed DET_ERROR_BUFFER_SIZE.
 */
STATIC VAR(uint8, DET_VAR) Det_u8LoggedErrorCount = 0U;

/**
 * @brief Flag to indicate if the error buffer has wrapped around at least once.
 * @details Helps in calculating the correct index for reading older errors.
 */
STATIC VAR(boolean, DET_VAR) Det_bBufferWrapped = FALSE;
#endif /* (DET_LOG_ERRORS_API == STD_ON) */

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/**
 * @brief Records a development error. (SWS_Det_00001)
 * @details If error logging is enabled (DET_LOG_ERRORS_API == STD_ON) and the buffer size is greater than 0,
 *          the error information is stored in an internal circular buffer.
 *          This function adheres to MISRA C guidelines.
 *
 * @param[in] ModuleId    Module ID of the calling module.
 * @param[in] InstanceId  The instance ID of the calling module.
 * @param[in] ApiId       ID of the API service where the error was detected.
 * @param[in] ErrorId     ID of the detected development error.
 */
FUNC(void, DET_CODE) Det_ReportError(
    VAR(uint16, AUTOMATIC) ModuleId,
    VAR(uint8, AUTOMATIC) InstanceId,
    VAR(uint8, AUTOMATIC) ApiId,
    VAR(uint8, AUTOMATIC) ErrorId
)
{
#if (DET_LOG_ERRORS_API == STD_ON)
    /* Check if buffer has space. DET_ERROR_BUFFER_SIZE is a preprocessor macro. */
    if (DET_ERROR_BUFFER_SIZE > 0U)
    {
        /* Store error information in the circular buffer */
        Det_aErrorBuffer[Det_u8ErrorBufferIndex].ModuleId   = ModuleId;
        Det_aErrorBuffer[Det_u8ErrorBufferIndex].InstanceId = InstanceId;
        Det_aErrorBuffer[Det_u8ErrorBufferIndex].ApiId      = ApiId;
        Det_aErrorBuffer[Det_u8ErrorBufferIndex].ErrorId    = ErrorId;

        /* Advance buffer index and handle wrap-around */
        Det_u8ErrorBufferIndex++;
        if (Det_u8ErrorBufferIndex >= DET_ERROR_BUFFER_SIZE)
        {
            Det_u8ErrorBufferIndex = 0U;
            Det_bBufferWrapped = TRUE; /* Buffer has now wrapped at least once */
        }

        /* Update logged error count */
        if (Det_bBufferWrapped == FALSE)
        {
            /* If buffer has not wrapped, count is simply the current index */
            Det_u8LoggedErrorCount = Det_u8ErrorBufferIndex;
        }
        else
        {
            /* If buffer has wrapped, it is considered full */
            Det_u8LoggedErrorCount = DET_ERROR_BUFFER_SIZE;
        }
    }
    /* If DET_ERROR_BUFFER_SIZE is 0, error is not logged but call is valid. */
#else /* (DET_LOG_ERRORS_API == STD_OFF) */
    /* Error logging is disabled, but DET reporting is active. */
    /* Prevent "unused parameter" warnings by casting to void. */
    (void)ModuleId;
    (void)InstanceId;
    (void)ApiId;
    (void)ErrorId;
#endif /* (DET_LOG_ERRORS_API == STD_ON) */

    /* Optional: Implement a breakpoint or specific halt for immediate debugging */
    /* Example:
       #if defined(DEBUG_HALT_ON_DET_ERROR) && (DEBUG_HALT_ON_DET_ERROR == STD_ON)
           __asm volatile("bkpt #0");
       #endif
    */
}


#if (DET_LOG_ERRORS_API == STD_ON)
/**
 * @brief Retrieves the number of errors currently logged in the DET buffer.
 * @details Adheres to MISRA C guidelines.
 *
 * @return uint8 The number of valid error entries in the log.
 */
FUNC(uint8, DET_CODE) Det_GetLogErrorCount(void)
{
    return Det_u8LoggedErrorCount;
}

/**
 * @brief Retrieves information about a specific logged development error.
 * @details Errors are indexed from 0 (oldest if buffer wrapped) to Det_GetLogErrorCount() - 1.
 *          Adheres to MISRA C guidelines, including pointer validity checks.
 *
 * @param[in]  ErrorIndex   The index of the error to retrieve from the log buffer.
 * @param[out] ErrorInfoPtr Pointer to a `Det_ErrorType` structure where the error
 *                          information will be copied. Must not be NULL.
 * @return Std_ReturnType   E_OK if retrieval is successful.
 *                          E_NOT_OK if `ErrorInfoPtr` is NULL or `ErrorIndex` is out of bounds.
 */
FUNC(Std_ReturnType, DET_CODE) Det_GetErrorInfo(
    VAR(uint8, AUTOMATIC) ErrorIndex,
    P2VAR(Det_ErrorType, AUTOMATIC, DET_APPL_DATA) ErrorInfoPtr
)
{
    Std_ReturnType eReturnValue = E_NOT_OK;
    VAR(uint8, AUTOMATIC) u8ActualReadIndex;

    if (NULL_PTR == ErrorInfoPtr)
    {
        /* As per SWS_Det_00024: if ErrorInfoPtr is NULL, no error shall be reported to DET,
           and the function shall return E_NOT_OK. */
        eReturnValue = E_NOT_OK;
    }
    else if (ErrorIndex >= Det_u8LoggedErrorCount) /* Check bounds first */
    {
        /* ErrorIndex is out of bounds of currently logged errors */
        eReturnValue = E_NOT_OK;
    }
    else if (DET_ERROR_BUFFER_SIZE == 0U) /* Check if buffer has any size */
    {
        /* No buffer to read from */
        eReturnValue = E_NOT_OK;
    }
    else
    {
        /* Calculate the actual index in the circular buffer */
        if (Det_bBufferWrapped == TRUE)
        {
            /* Buffer is full and has wrapped.
             * Det_u8ErrorBufferIndex points to the slot *after* the newest error (which is oldest).
             * So, (Det_u8ErrorBufferIndex + ErrorIndex) % DET_ERROR_BUFFER_SIZE gives the correct item.
             */
            u8ActualReadIndex = (Det_u8ErrorBufferIndex + ErrorIndex) % DET_ERROR_BUFFER_SIZE;
        }
        else
        {
            /* Buffer has not wrapped, ErrorIndex is a direct index from the start. */
            u8ActualReadIndex = ErrorIndex;
        }

        /* Copy error information to the output pointer */
        ErrorInfoPtr->ModuleId   = Det_aErrorBuffer[u8ActualReadIndex].ModuleId;
        ErrorInfoPtr->InstanceId = Det_aErrorBuffer[u8ActualReadIndex].InstanceId;
        ErrorInfoPtr->ApiId      = Det_aErrorBuffer[u8ActualReadIndex].ApiId;
        ErrorInfoPtr->ErrorId    = Det_aErrorBuffer[u8ActualReadIndex].ErrorId;

        eReturnValue = E_OK;
    }

    return eReturnValue;
}
#endif /* (DET_LOG_ERRORS_API == STD_ON) */

#else /* (DET_DEV_ERROR_DETECT == STD_OFF) */

/*
 * If DET_DEV_ERROR_DETECT is STD_OFF, Det_ReportError is typically defined as a void macro
 * in Det.h to eliminate the function call overhead. This C implementation serves as a linkable stub
 * if the macro is not used or if other Det functions (like logging API) are somehow enabled.
 */
FUNC(void, DET_CODE) Det_ReportError(
    VAR(uint16, AUTOMATIC) ModuleId,
    VAR(uint8, AUTOMATIC) InstanceId,
    VAR(uint8, AUTOMATIC) ApiId,
    VAR(uint8, AUTOMATIC) ErrorId
)
{
    /* DET is disabled - do nothing */
    (void)ModuleId;
    (void)InstanceId;
    (void)ApiId;
    (void)ErrorId;
}

#if (DET_LOG_ERRORS_API == STD_ON)
/* Stubs for logging functions if DET is globally off but logging API is ON (unlikely configuration) */
FUNC(uint8, DET_CODE) Det_GetLogErrorCount(void)
{
    return 0U;
}

FUNC(Std_ReturnType, DET_CODE) Det_GetErrorInfo(
    VAR(uint8, AUTOMATIC) ErrorIndex,
    P2VAR(Det_ErrorType, AUTOMATIC, DET_APPL_DATA) ErrorInfoPtr
)
{
    (void)ErrorIndex; /* Parameter not used */
    if (NULL_PTR != ErrorInfoPtr)
    {
        /* Clear the output struct to avoid returning uninitialized data */
        ErrorInfoPtr->ModuleId   = 0U;
        ErrorInfoPtr->InstanceId = 0U;
        ErrorInfoPtr->ApiId      = 0U;
        ErrorInfoPtr->ErrorId    = 0U;
    }
    return E_NOT_OK; /* Logging API is conceptually ON but DET is OFF, so no valid data */
}
#endif /* (DET_LOG_ERRORS_API == STD_ON) */

#endif /* (DET_DEV_ERROR_DETECT == STD_ON) */
