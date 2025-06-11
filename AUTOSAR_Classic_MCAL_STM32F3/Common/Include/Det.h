#ifndef DET_H
#define DET_H

#include "Std_Types.h"

/* AUTOSAR Development Error Tracer */

/* DET Module ID */
#define DET_MODULE_ID                 15U

/* DET Instance ID */
#define DET_INSTANCE_ID               0U

/* API Service ID for Det_ReportError */
#define DET_SID_REPORTERROR           0x01U

/* Standard error codes reported by DET */
/* These are examples, a full Det module would have more specific error codes */
#define DET_E_PARAM_POINTER           0x01U
#define DET_E_PARAM_INVALID           0x02U
#define DET_E_UNINITIALIZED           0x03U
#define DET_E_TIMEOUT                 0x04U


#if defined(USE_DET_STUB) || !defined(DET_ENABLED) || (DET_ENABLED == STD_OFF)
/* If DET is disabled or using a stub, provide a dummy implementation */
#define Det_ReportError(ModuleId, InstanceId, ApiId, ErrorId) ((void)0)
#else
/*
 * @brief Report a development error to the DET module.
 * @param ModuleId The ID of the module reporting the error.
 * @param InstanceId The instance ID of the module reporting the error.
 * @param ApiId The ID of the API service in which the error was detected.
 * @param ErrorId The ID of the detected development error.
 * @return Std_ReturnType (E_OK if error was processed, E_NOT_OK otherwise - though often void for DET)
 *
 * @note This is a function declaration. The actual implementation will be in Det.c
 *       or could be a macro if DET is simple.
 */
extern FUNC(Std_ReturnType, DET_CODE) Det_ReportError(
    VAR(uint16, AUTOMATIC) ModuleId,
    VAR(uint8, AUTOMATIC) InstanceId,
    VAR(uint8, AUTOMATIC) ApiId,
    VAR(uint8, AUTOMATIC) ErrorId
);
#endif /* USE_DET_STUB || !DET_ENABLED ... */

/* Function to initialize the DET module (if needed) */
extern FUNC(void, DET_CODE) Det_Init(void);

/* Function to start the DET module (if needed, e.g. after Init) */
extern FUNC(void, DET_CODE) Det_Start(void);

#endif /* DET_H */
