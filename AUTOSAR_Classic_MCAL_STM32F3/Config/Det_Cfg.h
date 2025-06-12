#ifndef DET_CFG_H
#define DET_CFG_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "Std_Types.h" /* For STD_ON, STD_OFF definitions */

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/**
 * @brief Switch to enable or disable development error detection and reporting for all modules
 *        that use the DET module (or specifically for DET features if DET has internal errors to report).
 * @details STD_ON: Enable development error detection.
 *          STD_OFF: Disable development error detection.
 *          MISRA C:2012 Rule 10.3 - Relies on STD_ON/STD_OFF using U suffix.
 */
#define DET_DEV_ERROR_DETECT          STD_ON  /* General switch for DET usage by other modules */
                                            /* Renamed from DET_DEVELOPMENT_ERROR_DETECT to be more aligned with typical AUTOSAR naming for this specific macro */

/**
 * @brief Switch to enable or disable the Det_GetVersionInfo API.
 * @details STD_ON: Gpt_GetVersionInfo API is available.
 *          STD_OFF: Gpt_GetVersionInfo API is not available (not part of this DET stub).
 *          (Currently, Det.h does not declare GetVersionInfo for Det itself)
 */
#define DET_VERSION_INFO_API          STD_OFF /* Example: Det module itself usually doesn't have one, but other modules call Det. */


/**
 * @brief Switch to enable or disable the error logging API within the DET module.
 * @details STD_ON: Enables Det_GetLogErrorCount and Det_GetErrorInfo functions.
 *          STD_OFF: These functions are not available.
 */
#define DET_LOG_ERRORS_API            STD_ON

/**
 * @brief Defines the size of the internal buffer used for logging development errors.
 * @details This value determines how many recent errors can be stored by the DET module.
 *          Must be an unsigned integer. Adheres to MISRA C:2012 Rule 10.3.
 */
#define DET_ERROR_BUFFER_SIZE         10U


/* Add other DET configurations as needed, for example:
 * - Hooks for specific error reporting mechanisms.
 * - Filtering for specific errors.
 */

#endif /* DET_CFG_H */
