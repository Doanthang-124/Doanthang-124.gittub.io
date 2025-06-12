#ifndef COMPILER_H
#define COMPILER_H

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/*
 * Compiler Abstraction: Memory Class Macros
 * ----------------------------------------
 * These macros define memory classes for code, constants, and variables.
 * They are intended to be mapped to compiler-specific section attributes
 * to control memory placement (e.g., for linker script).
 *
 * The `_SEC` suffixed macros provide compiler-specific definitions (e.g., for GCC).
 * The generic macros (e.g., `DIO_CODE`) are then defined using these specific ones.
 * If no specific sectioning is intended for a class or compiler, it defaults to empty.
 */

/* Default (empty) definitions for memory classes */
#define DIO_CODE_DEFAULT                /* Default for DIO driver code sections */
#define PORT_CODE_DEFAULT               /* Default for Port driver code sections */
#define DET_CODE_DEFAULT                /* Default for DET module code sections */
#define GPT_CODE_DEFAULT                /* Default for GPT driver code sections */
#define MCU_CODE_DEFAULT                /* Default for MCU driver code sections */
#define COMMON_CODE_DEFAULT             /* Default for common/shared code sections */
#define APPL_CODE_DEFAULT               /* Default for Application code sections */
#define CONFIG_CODE_DEFAULT             /* Default for executable configuration code sections */
#define DEFAULT_CODE_DEFAULT            /* Default for general code sections */

#define DIO_CONST_DEFAULT               /* Default for DIO driver constant data */
#define PORT_CONST_DEFAULT              /* Default for Port driver constant data */
#define DET_CONST_DEFAULT               /* Default for DET module constant data */
#define GPT_CONST_DEFAULT               /* Default for GPT driver constant data */
#define CONFIG_CONST_DEFAULT            /* Default for PostBuildConfig data constants */
#define DEFAULT_CONST_DEFAULT           /* Default for general constant data */

#define DIO_VAR_DEFAULT                 /* Default for DIO driver variables */
#define PORT_VAR_DEFAULT                /* Default for Port driver variables */
#define DET_VAR_DEFAULT                 /* Default for DET module variables */
#define GPT_VAR_DEFAULT                 /* Default for GPT module variables */
#define DEFAULT_VAR_DEFAULT             /* Default for general variables */


/* Compiler specific section mapping (Example for GCC) */
#ifdef __GNUC__
    /* Code Sections */
    #define DIO_CODE_SEC                __attribute__((section(".mcal_dio_code")))
    #define PORT_CODE_SEC               __attribute__((section(".mcal_port_code")))
    #define DET_CODE_SEC                __attribute__((section(".mcal_det_code")))
    #define GPT_CODE_SEC                __attribute__((section(".mcal_gpt_code")))
    #define MCU_CODE_SEC                __attribute__((section(".mcal_mcu_code")))
    #define COMMON_CODE_SEC             __attribute__((section(".mcal_common_code")))
    #define APPL_CODE_SEC               __attribute__((section(".appl_code")))
    #define CONFIG_CODE_SEC             __attribute__((section(".config_code")))
    #define DEFAULT_CODE_SEC            /* Mapped to standard .text by linker */

    /* Constant Data Sections */
    #define DIO_CONST_SEC               __attribute__((section(".mcal_dio_const")))
    #define PORT_CONST_SEC              __attribute__((section(".mcal_port_const")))
    #define DET_CONST_SEC               __attribute__((section(".mcal_det_const")))
    #define GPT_CONST_SEC               __attribute__((section(".mcal_gpt_const")))
    #define CONFIG_CONST_SEC            __attribute__((section(".config_const")))
    #define DEFAULT_CONST_SEC           /* Mapped to standard .rodata by linker */

    /* Variable Data Sections (RAM) */
    #define DIO_VAR_SEC                 __attribute__((section(".mcal_dio_var")))
    #define PORT_VAR_SEC                __attribute__((section(".mcal_port_var")))
    #define DET_VAR_SEC                 __attribute__((section(".mcal_det_var")))
    #define GPT_VAR_SEC                 __attribute__((section(".mcal_gpt_var")))
    #define DEFAULT_VAR_SEC             /* Mapped to standard .data or .bss by linker */
#else
    /* Define for other compilers or default to empty if no specific sectioning */
    #define DIO_CODE_SEC                DIO_CODE_DEFAULT
    #define PORT_CODE_SEC               PORT_CODE_DEFAULT
    #define DET_CODE_SEC                DET_CODE_DEFAULT
    #define GPT_CODE_SEC                GPT_CODE_DEFAULT
    #define MCU_CODE_SEC                MCU_CODE_DEFAULT
    #define COMMON_CODE_SEC             COMMON_CODE_DEFAULT
    #define APPL_CODE_SEC               APPL_CODE_DEFAULT
    #define CONFIG_CODE_SEC             CONFIG_CODE_DEFAULT
    #define DEFAULT_CODE_SEC            DEFAULT_CODE_DEFAULT

    #define DIO_CONST_SEC               DIO_CONST_DEFAULT
    #define PORT_CONST_SEC              PORT_CONST_DEFAULT
    #define DET_CONST_SEC               DET_CONST_DEFAULT
    #define GPT_CONST_SEC               GPT_CONST_DEFAULT
    #define CONFIG_CONST_SEC            CONFIG_CONST_DEFAULT
    #define DEFAULT_CONST_SEC           DEFAULT_CONST_DEFAULT

    #define DIO_VAR_SEC                 DIO_VAR_DEFAULT
    #define PORT_VAR_SEC                PORT_VAR_DEFAULT
    #define DET_VAR_SEC                 DET_VAR_DEFAULT
    #define GPT_VAR_SEC                 GPT_VAR_DEFAULT
    #define DEFAULT_VAR_SEC             DEFAULT_VAR_DEFAULT
#endif

/* Assign the active section macros to the generic AUTOSAR memclass names */
/* Code memory classes */
#define DIO_CODE                    DIO_CODE_SEC
#define PORT_CODE                   PORT_CODE_SEC
#define DET_CODE                    DET_CODE_SEC
#define GPT_CODE                    GPT_CODE_SEC
#define MCU_CODE                    MCU_CODE_SEC        /* For general MCAL code not fitting other categories */
#define COMMON_CODE                 COMMON_CODE_SEC     /* For truly shared common code, e.g. in Common/Src */
#define APPL_CODE                   APPL_CODE_SEC       /* For Application code (e.g., main.c) */
#define CONFIG_CODE                 CONFIG_CODE_SEC     /* For any executable code within configuration files */
#define DEFAULT_CODE                DEFAULT_CODE_SEC    /* Default if no specific class applies */

/* Constant data memory classes */
#define DIO_CONST                   DIO_CONST_SEC
#define PORT_CONST                  PORT_CONST_SEC
#define DET_CONST                   DET_CONST_SEC
#define GPT_CONST                   GPT_CONST_SEC
#define CONFIG_CONST                CONFIG_CONST_SEC    /* For PostBuild Config data structures (e.g. DioConfig, PortConfig) */
#define DEFAULT_CONST               DEFAULT_CONST_SEC   /* Default for other constants */

/* Variable data memory classes (RAM) */
#define DIO_VAR                     DIO_VAR_SEC
#define PORT_VAR                    PORT_VAR_SEC
#define DET_VAR                     DET_VAR_SEC         /* For Det_ErrorBuffer and other Det runtime variables */
#define GPT_VAR                     GPT_VAR_SEC         /* For Gpt runtime channel data */
#define DEFAULT_VAR                 DEFAULT_VAR_SEC     /* Default for other variables */


/*
 * Compiler Abstraction: Function and Variable Declaration Macros
 * -------------------------------------------------------------
 * These macros abstract the declaration of functions, variables, and constants,
 * incorporating the memory class macros defined above.
 */

/**
 * @brief Macro for function definition/declaration.
 * @param rettype Return type of the function.
 * @param memclass Memory class where the function's code should be placed (e.g., PORT_CODE).
 * @details Expands to `rettype memclass_expanded_attributes`.
 * Example: `FUNC(void, PORT_CODE) Port_Init(void);`
 */
#define FUNC(rettype, memclass)                             rettype memclass

/**
 * @brief Macro for defining a function pointer type.
 * @param rettype Return type of the function pointer.
 * @param memclass_ptr Memory class for the function pointer variable itself (often DEFAULT_VAR or AUTOMATIC).
 * @param fctname Name of the function pointer type to be defined (e.g., MyFuncPtrType).
 * @details Expands to `rettype (*fctname)`. Arguments list follows the macro.
 * Example: `typedef P2FUNC_TYPEDEF(void, DEFAULT_CODE, MyCallbackType)(uint8 param);`
 *          `MyCallbackType cb_ptr;`
 */
#define P2FUNC_TYPEDEF(rettype, memclass_ptr, fctname)      rettype (* fctname) /* memclass_ptr is for the pointer variable, not the target code */

/**
 * @brief Macro for declaring a function pointer variable.
 * @param rettype Return type of the function pointer.
 * @param memclass_ptr Memory class for the function pointer variable itself.
 * @param fctname Name of the function pointer variable.
 * @details Expands to `rettype (*fctname)`. Arguments list follows the macro.
 * Example: `P2FUNC(void, DEFAULT_VAR, g_NotificationHandler)(void);`
 */
#define P2FUNC(rettype, memclass_ptr, fctname)              rettype (* fctname)


/**
 * @brief Macro for declaring a pointer to constant data.
 * @param ptrtype Type of the data being pointed to.
 * @param memclass_ptr Memory class for the pointer variable itself.
 * @param ptrclass_target Target data memory class / pointer class (e.g., APPL_CONST, CONFIG_CONST).
 * @details Expands to `const ptrclass_target_expanded ptrtype * memclass_ptr_expanded`.
 * Example: `P2CONST(uint8, AUTOMATIC, APPL_CONST) pDataToConst;`
 */
#define P2CONST(ptrtype, memclass_ptr, ptrclass_target)     const ptrclass_target ptrtype * memclass_ptr

/**
 * @brief Macro for declaring a pointer to variable data.
 * @param ptrtype Type of the data being pointed to.
 * @param memclass_ptr Memory class for the pointer variable itself.
 * @param ptrclass_target Target data memory class / pointer class (e.g., APPL_DATA, DEFAULT_VAR).
 * @details Expands to `ptrclass_target_expanded ptrtype * memclass_ptr_expanded`.
 * Example: `P2VAR(uint8, AUTOMATIC, APPL_VAR) pDataToVar;`
 */
#define P2VAR(ptrtype, memclass_ptr, ptrclass_target)       ptrclass_target ptrtype * memclass_ptr


/**
 * @brief Macro for constant data definition.
 * @param consttype Type of the constant.
 * @param memclass Memory class where the constant should be placed (e.g., CONFIG_CONST).
 * @details Expands to `const consttype memclass_expanded_attributes`.
 * Example: `CONST(uint16, CONFIG_CONST) MyConfigParam = 100U;`
 */
#define CONST(consttype, memclass)                          const consttype memclass

/**
 * @brief Macro for variable data definition.
 * @param vartype Type of the variable.
 * @param memclass Memory class where the variable should be placed (e.g., DET_VAR).
 * @details Expands to `vartype memclass_expanded_attributes`.
 * Example: `VAR(uint8, DET_VAR) Det_ErrorCounter;`
 */
#define VAR(vartype, memclass)                              vartype memclass


/*
 * Compiler Abstraction: General Keywords
 * --------------------------------------
 */

/* @brief Macro for the `volatile` keyword. */
#ifndef _VOLATILE_
  #define _VOLATILE_ volatile
#endif

/* @brief Macro for the `inline` keyword.
 * @details `inline` is C99. `__attribute__((always_inline))` is GCC specific.
 *          For MISRA C compliance, use of `inline` should be justified.
 */
#ifndef _INLINE_
  #ifdef __GNUC__
    #define _INLINE_ inline __attribute__((always_inline))
  #else
    #define _INLINE_ inline
  #endif
#endif

/* @brief Macro for the `static` keyword for defining static functions or variables. */
#ifndef STATIC
  #define STATIC static
#endif

/* @brief Macro for NULL pointer, compatible with C type checking. */
/* Already defined in Std_Types.h, but good to have a central compiler abstraction for it if needed elsewhere.
   Std_Types.h definition is `#define NULL_PTR ((void *)0)` which is typical.
   This is just for completeness of compiler abstraction items.
*/
#ifndef NULL_PTR
    #define NULL_PTR ((void *)0)
#endif

#endif /* COMPILER_H */
