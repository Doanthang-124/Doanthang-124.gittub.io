#include "Det.h" /* Includes Std_Types.h, Compiler.h (via Std_Types), Det_Cfg.h */

#if (DET_DEVELOPMENT_ERROR_DETECT == STD_ON)

#if (DET_LOG_ERRORS_API == STD_ON)
STATIC VAR(Det_ErrorType, DET_VAR) Det_ErrorBuffer[DET_ERROR_BUFFER_SIZE];
STATIC VAR(uint8, DET_VAR) Det_ErrorBufferIndex = 0; /* Points to the next slot to write */
STATIC VAR(uint8, DET_VAR) Det_LoggedErrorCount = 0; /* Number of valid entries in buffer */
STATIC VAR(boolean, DET_VAR) Det_BufferWrapped = FALSE; /* To indicate if buffer has wrapped around */
#endif /* DET_LOG_ERRORS_API */

FUNC(void, DET_CODE) Det_ReportError(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId)
{
    /* In a real scenario, this could trigger a breakpoint, log to a specific interface, etc. */
    /* For now, it just logs to internal buffer if enabled. */
#if (DET_LOG_ERRORS_API == STD_ON)
    if (DET_ERROR_BUFFER_SIZE > 0) {
        Det_ErrorBuffer[Det_ErrorBufferIndex].ModuleId = ModuleId;
        Det_ErrorBuffer[Det_ErrorBufferIndex].InstanceId = InstanceId;
        Det_ErrorBuffer[Det_ErrorBufferIndex].ApiId = ApiId;
        Det_ErrorBuffer[Det_ErrorBufferIndex].ErrorId = ErrorId;

        Det_ErrorBufferIndex++;
        if (Det_ErrorBufferIndex >= DET_ERROR_BUFFER_SIZE) {
            Det_ErrorBufferIndex = 0;
            Det_BufferWrapped = TRUE; /* Buffer has wrapped */
        }

        if (!Det_BufferWrapped) {
            Det_LoggedErrorCount = Det_ErrorBufferIndex;
        } else {
            Det_LoggedErrorCount = DET_ERROR_BUFFER_SIZE; /* Buffer is full */
        }
    }
#else
    /* If logging is off, still acknowledge parameters to prevent unused warnings if that is desired */
    (void)ModuleId;
    (void)InstanceId;
    (void)ApiId;
    (void)ErrorId;
#endif /* DET_LOG_ERRORS_API */

    /* Example: To halt on error for debugging when no advanced logging is present */
    /* while(1); */
}

#if (DET_LOG_ERRORS_API == STD_ON)
FUNC(uint8, DET_CODE) Det_GetLogErrorCount(void)
{
    return Det_LoggedErrorCount;
}

FUNC(Std_ReturnType, DET_CODE) Det_GetErrorInfo(uint8 ErrorIndex, P2VAR(Det_ErrorType, AUTOMATIC, DET_APPL_DATA) ErrorInfoPtr)
{
    Std_ReturnType retVal = E_NOT_OK;

    if (ErrorInfoPtr == NULL_PTR) {
        /* Optionally report another error for NULL pointer, though it might cause recursion if Det_ReportError itself calls this. */
        return E_NOT_OK;
    }

    if (ErrorIndex < Det_LoggedErrorCount)
    {
        uint8 actualReadIndex;
        if (Det_BufferWrapped) {
            /* Buffer is full and wrapped, 0 is oldest, DET_ERROR_BUFFER_SIZE-1 is newest if index is 0
               Or, if index is Det_ErrorBufferIndex, then Det_ErrorBufferIndex is oldest, Det_ErrorBufferIndex-1 is newest.
               Let's make 0 the oldest available error.
            */
            actualReadIndex = (Det_ErrorBufferIndex + ErrorIndex) % DET_ERROR_BUFFER_SIZE;
        } else {
            actualReadIndex = ErrorIndex; /* Buffer not yet wrapped, direct indexing */
        }

        *ErrorInfoPtr = Det_ErrorBuffer[actualReadIndex];
        retVal = E_OK;
    }
    return retVal;
}
#endif /* DET_LOG_ERRORS_API */

#else /* DET_DEVELOPMENT_ERROR_DETECT == STD_OFF */

/* Provide stub for Det_ReportError if DET is globally off, to allow modules to compile. */
/* Modules should ideally check DET_DEVELOPMENT_ERROR_DETECT before calling. */
/* This stub means calls to Det_ReportError will compile but do nothing. */
FUNC(void, DET_CODE) Det_ReportError(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId)
{
    (void)ModuleId;
    (void)InstanceId;
    (void)ApiId;
    (void)ErrorId;
}

#if (DET_LOG_ERRORS_API == STD_ON)
/* Stubs for logging functions if DET is off but API is ON (unlikely scenario but for completeness) */
FUNC(uint8, DET_CODE) Det_GetLogErrorCount(void)
{
    return 0;
}

FUNC(Std_ReturnType, DET_CODE) Det_GetErrorInfo(uint8 ErrorIndex, P2VAR(Det_ErrorType, AUTOMATIC, DET_APPL_DATA) ErrorInfoPtr)
{
    (void)ErrorIndex;
    (void)ErrorInfoPtr;
    return E_NOT_OK;
}
#endif /* DET_LOG_ERRORS_API */

#endif /* DET_DEVELOPMENT_ERROR_DETECT */
