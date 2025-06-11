#ifndef DET_H
#define DET_H

#include "Std_Types.h" /* For Std_ReturnType, uint8, uint16, P2VAR etc. */
                         /* Std_Types.h should also include Compiler.h for FUNC */
#include "Det_Cfg.h"   /* For DET configurations like DET_ERROR_BUFFER_SIZE */

/* Structure to hold error information */
typedef struct {
    uint16 ModuleId;
    uint8 InstanceId;
    uint8 ApiId;
    uint8 ErrorId;
} Det_ErrorType;

/*
 * @brief Reports a development error.
 * @param ModuleId    Module ID of calling module.
 * @param InstanceId  The identifier of the index based instance of a module, if the module is reentrant.
 * @param ApiId       ID of API service in which error is detected.
 * @param ErrorId     ID of detected development error.
 */
extern FUNC(void, DET_CODE) Det_ReportError(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId);

#if (DET_LOG_ERRORS_API == STD_ON) /* Controlled by a new macro in Det_Cfg.h */
/**
 * @brief Gets the number of errors currently logged in the buffer.
 * @return Number of logged errors.
 */
extern FUNC(uint8, DET_CODE) Det_GetLogErrorCount(void);

/**
 * @brief Retrieves information about a logged error.
 * @param ErrorIndex Index of the error in the log buffer (0 to Det_GetLogErrorCount() - 1).
 * @param ErrorInfoPtr Pointer to a Det_ErrorType structure to store the error information.
 * @return E_OK if successful, E_NOT_OK if ErrorIndex is out of bounds or buffer is empty.
 */
extern FUNC(Std_ReturnType, DET_CODE) Det_GetErrorInfo(uint8 ErrorIndex, P2VAR(Det_ErrorType, AUTOMATIC, DET_APPL_DATA) ErrorInfoPtr);
#endif /* DET_LOG_ERRORS_API */

#endif /* DET_H */
