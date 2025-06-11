#ifndef DET_CFG_H
#define DET_CFG_H

#include "Std_Types.h" /* For STD_ON/STD_OFF */

/* Switch to enable/disable development error detection and reporting. */
#define DET_DEVELOPMENT_ERROR_DETECT STD_ON /* Or STD_OFF to disable DET globally or for this module */

/* Configuration for DET error logging API */
#define DET_LOG_ERRORS_API           STD_ON  /* Enable functions like Det_GetLogErrorCount */
#define DET_ERROR_BUFFER_SIZE        10U     /* Number of errors to store in the buffer */

/* Add other DET configurations as needed, e.g., version info API switch, specific error hooks, etc. */
// #define DET_VERSION_INFO_API      STD_OFF

#endif /* DET_CFG_H */
