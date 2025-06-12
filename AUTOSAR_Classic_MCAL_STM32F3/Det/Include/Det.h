#ifndef DET_H
#define DET_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "Std_Types.h"      /* For Std_ReturnType, uint8, uint16, P2VAR, FUNC, etc. */
#include "Det_Cfg.h"        /* For DET configurations like DET_LOG_ERRORS_API, DET_DEV_ERROR_DETECT */

/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

/**
 * @brief Module ID for the Development Error Tracer.
 * @details As per AUTOSAR_SWS_StandardTypes. This ID is specific to the Det module itself.
 */
#define DET_MODULE_ID                     15U

/**
 * @brief Instance ID for the Development Error Tracer.
 * @details Typically 0 for non-reentrant modules.
 */
#define DET_INSTANCE_ID                   0U


/* API Service IDs for DET functions (for reporting errors within DET itself, if applicable) */
#define DET_SID_REPORT_ERROR               0x01U /* Service ID for Det_ReportError */
#define DET_SID_GET_LOG_ERROR_COUNT        0x02U /* Service ID for Det_GetLogErrorCount */
#define DET_SID_GET_ERROR_INFO             0x03U /* Service ID for Det_GetErrorInfo */
/* DET_SID_GET_VERSION_INFO      0x00U (if GetVersionInfo was added for Det itself) */


/*==================================================================================================
*                                             TYPES
==================================================================================================*/

/**
 * @brief Structure to hold detailed information about a reported development error.
 * @details Members use fixed-width types for clarity and portability, as defined in Std_Types.h / Platform_Types.h.
 */
typedef struct
{
    uint16  ModuleId;       /**< @brief ID of the module that reported the error. */
    uint8   InstanceId;     /**< @brief Instance ID of the reporting module. */
    uint8   ApiId;          /**< @brief ID of the API service where the error was detected. */
    uint8   ErrorId;        /**< @brief ID of the detected development error. */
} Det_ErrorType;


/*==================================================================================================
*                                  FUNCTION PROTOTYPES
==================================================================================================*/

#if (DET_DEV_ERROR_DETECT == STD_ON) || defined(DET_FORCE_INCLUDE_API) /* Allow API declaration even if DET is off for stubbing/linking */

/**
 * @brief Reports a development error to the DET module. (SWS_Det_00001)
 * @details This function is called by other BSW modules when a development error is detected.
 *          Its behavior (e.g., logging, halting) depends on the DET module's implementation
 *          and configuration.
 *
 * @param[in] ModuleId    Module ID of the calling module (as defined in its Xxx.h).
 * @param[in] InstanceId  The instance ID of the calling module (0 if not reentrant).
 * @param[in] ApiId       ID of the API service in which the error was detected (module-specific).
 * @param[in] ErrorId     ID of the detected development error (module-specific).
 */
extern FUNC(void, DET_CODE) Det_ReportError(
    VAR(uint16, AUTOMATIC) ModuleId,
    VAR(uint8, AUTOMATIC) InstanceId,
    VAR(uint8, AUTOMATIC) ApiId,
    VAR(uint8, AUTOMATIC) ErrorId
);


#if (DET_LOG_ERRORS_API == STD_ON) /* From Det_Cfg.h */
/**
 * @brief Gets the number of development errors currently logged in the DET buffer.
 * @details This function is only available if error logging is enabled via `DET_LOG_ERRORS_API`.
 *
 * @return uint8 Number of logged errors. Returns 0 if no errors are logged or logging is disabled.
 */
extern FUNC(uint8, DET_CODE) Det_GetLogErrorCount(void);

/**
 * @brief Retrieves information about a specific logged development error.
 * @details This function is only available if error logging is enabled via `DET_LOG_ERRORS_API`.
 *          The error is identified by its index in the log buffer.
 *
 * @param[in]  ErrorIndex   Index of the error in the log buffer (0 to Det_GetLogErrorCount() - 1).
 * @param[out] ErrorInfoPtr Pointer to a `Det_ErrorType` structure where the error information will be stored.
 *                          This pointer must not be NULL.
 * @return Std_ReturnType   E_OK if the error information was successfully retrieved.
 *                          E_NOT_OK if `ErrorInfoPtr` is NULL, `ErrorIndex` is out of bounds,
 *                          or no errors are logged/logging is disabled.
 */
extern FUNC(Std_ReturnType, DET_CODE) Det_GetErrorInfo(
    VAR(uint8, AUTOMATIC) ErrorIndex,
    P2VAR(Det_ErrorType, AUTOMATIC, DET_APPL_DATA) ErrorInfoPtr
);
#endif /* (DET_LOG_ERRORS_API == STD_ON) */

#else /* (DET_DEV_ERROR_DETECT == STD_OFF) && !defined(DET_FORCE_INCLUDE_API) */

/*
 * If DET is disabled, provide a macro to make calls to Det_ReportError effectively disappear.
 * This prevents compilation errors in modules that call Det_ReportError.
 * MISRA C:2012 Rule 20.7: Parenthesize macro parameters. (Not strictly needed here as parameters are not in expressions within the macro)
 */
#define Det_ReportError(ModuleId, InstanceId, ApiId, ErrorId) ((void)0)

#if (DET_LOG_ERRORS_API == STD_ON)
  /*
   * If DET is globally disabled but the logging API is still configured as ON (unlikely scenario),
   * provide stubs for the logging functions to allow linking with Det.c (which would also provide stubs).
   * Alternatively, these could also be macros returning default values.
   */
  extern FUNC(uint8, DET_CODE) Det_GetLogErrorCount(void);
  extern FUNC(Std_ReturnType, DET_CODE) Det_GetErrorInfo(
    VAR(uint8, AUTOMATIC) ErrorIndex,
    P2VAR(Det_ErrorType, AUTOMATIC, DET_APPL_DATA) ErrorInfoPtr
  );
#endif /* (DET_LOG_ERRORS_API == STD_ON) */

#endif /* (DET_DEV_ERROR_DETECT == STD_ON) || defined(DET_FORCE_INCLUDE_API) */

#endif /* DET_H */
