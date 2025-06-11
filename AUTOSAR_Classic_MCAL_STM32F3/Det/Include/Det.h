#ifndef DET_H
#define DET_H

#include "Std_Types.h"  /* For Std_ReturnType, uint16, uint8, etc. */
#include "Det_Cfg.h"    /* For DET configuration switches like DET_ENABLE_REPORT_ERROR */
#include "Compiler.h"   /* For FUNC, VAR macros */


/* AUTOSAR Development Error Tracer */

/* DET Module ID (example, can be defined in Det_Cfg.h or a global symbols file) */
#ifndef DET_MODULE_ID
    #define DET_MODULE_ID                 15U
#endif

/* DET Instance ID (example) */
#ifndef DET_INSTANCE_ID
    #define DET_INSTANCE_ID               0U
#endif

/* API Service ID for Det_ReportError */
#define DET_SID_REPORTERROR           0x01U
#define DET_SID_INIT                  0x02U /* Example SID for Det_Init */
#define DET_SID_START                 0x03U /* Example SID for Det_Start */


/* Standard error codes reported by DET (examples) */
/* These can be expanded or made configurable */
#define DET_E_PARAM_POINTER           0x01U
#define DET_E_PARAM_INVALID           0x02U
#define DET_E_UNINITIALIZED           0x03U
#define DET_E_TIMEOUT                 0x04U
/* Add more DET specific error codes if needed */


#if (DET_ENABLE_REPORT_ERROR == STD_ON)
/*
 * @brief Report a development error to the DET module.
 * @param ModuleId The ID of the module reporting the error.
 * @param InstanceId The instance ID of the module reporting the error.
 * @param ApiId The ID of the API service in which the error was detected.
 * @param ErrorId The ID of the detected development error.
 * @return Std_ReturnType (E_OK if error was processed, E_NOT_OK otherwise - though often void for DET)
 */
extern FUNC(Std_ReturnType, DET_CODE) Det_ReportError(
    VAR(uint16, AUTOMATIC) ModuleId,
    VAR(uint8, AUTOMATIC) InstanceId,
    VAR(uint8, AUTOMATIC) ApiId,
    VAR(uint8, AUTOMATIC) ErrorId
);
#else
/* If DET reporting is disabled, provide a dummy macro implementation */
#define Det_ReportError(ModuleId, InstanceId, ApiId, ErrorId) ((void)0)
#endif /* (DET_ENABLE_REPORT_ERROR == STD_ON) */

/* Function to initialize the DET module (if DET needs initialization) */
extern FUNC(void, DET_CODE) Det_Init(void); /* Basic init, content in Det.c */

/* Function to start the DET module (if DET has a start phase, e.g., after basic init) */
extern FUNC(void, DET_CODE) Det_Start(void); /* Basic start, content in Det.c */

#endif /* DET_H */
