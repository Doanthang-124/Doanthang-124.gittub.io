#ifndef DET_CFG_H
#define DET_CFG_H

#include "Std_Types.h" // For STD_ON/STD_OFF

/* Switch to enable/disable development error detection and reporting. */
/* This can be a global switch for all DET calls or specific to DET module's own checks if any */
#define DET_DEV_ERROR_DETECT STD_ON // Kept for consistency, actual effect depends on how Det_ReportError is used.

/* Specific switch for enabling/disabling the Det_ReportError functionality itself. */
/* If this is STD_OFF, calls to Det_ReportError might compile out to nothing. */
#define DET_ENABLE_REPORT_ERROR STD_ON // Example: Enable reporting

/* Add other DET configurations as needed, e.g., version info API switch, specific error hooks, etc. */
// #define DET_VERSION_INFO_API      STD_OFF

#endif /* DET_CFG_H */
