#ifndef COMPILER_H
#define COMPILER_H

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/*
 * Define memory class macros. These are placeholders and would need to be mapped
 * to actual memory sections in a linker script and via compiler-specific attributes
 * for real memory sectioning.
 *
 * For GCC, a common way to assign to a section is: __attribute__((section(".section_name")))
 * Example: #define DIO_CODE __attribute__((section (".DIO_CODE_R")))
 *
 * For now, these are defined as empty, meaning they will not add any special
 * attributes to the function/variable declarations.
 */

/* Memory classes for code */
#define DIO_CODE                                            /* Used for DIO code */
#define PORT_CODE                                           /* Used for Port code */
#define DET_CODE                                            /* Used for DET code */
#define MCU_CODE                                            /* Used for MCU driver code */
#define COMMON_CODE                                         /* Used for common/shared code */
#define APPL_CODE                                           /* Used for Application code */
#define CONFIG_CODE                                         /* Used for configuration code (e.g. PBcfg.c) */
#define DEFAULT_CODE                                        /* Default code section */

/* Memory classes for constants (example, can be more granular) */
#define DIO_CONST                                           /* Used for DIO constants */
#define PORT_CONST                                          /* Used for Port constants */
#define DET_CONST                                           /* Used for DET constants */
#define COMMON_CONST                                        /* Used for common constants */
#define APPL_CONST                                          /* Used for Application constants */
#define CONFIG_CONST                                        /* Used for configuration constants (e.g. in PBcfg.c) */
#define DEFAULT_CONST                                       /* Default constant section */

/* Memory classes for variables (example) */
#define DIO_VAR                                             /* Used for DIO variables */
#define PORT_VAR                                            /* Used for Port variables */
#define DET_VAR                                             /* Used for DET variables */
#define COMMON_VAR                                          /* Used for common variables */
#define APPL_VAR                                            /* Used for Application variables */
#define DEFAULT_RAM                                         /* Default RAM section for variables */


/*
 * @brief Macro for function definition.
 * @param rettype The return type of the function.
 * @param memclass The memory class where the function code shall be placed (e.g., DIO_CODE).
 * @details Expands to `rettype memclass`. If `memclass` is empty, it's just `rettype`.
 *          If `memclass` is an attribute like `__attribute__((section(".text"))`, it becomes
 *          `rettype __attribute__((section(".text")))`.
 * @example FUNC(void, DIO_CODE) Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level);
 */
#define FUNC(rettype, memclass)                             memclass rettype

/**
 * @brief Macro for pointer to function declaration.
 * @param rettype The return type of the function that the pointer refers to.
 * @param memclass The memory class of the pointer variable itself (less common to section pointers this way).
 *                 Often, this is a placeholder like `TYPEDEF` or `AUTOMATIC` in AUTOSAR specs.
 *                 For simplicity here, it might not be used by the expansion if basic C pointers are formed.
 * @param fctname The name of the function pointer type or variable.
 * @details Expands to `rettype (*fctname)`.
 * @example typedef P2FUNC(void, APPL_CODE, MyFunctionPointerType)(uint8);
 *          MyFunctionPointerType ptrToFunc;
 * @example P2FUNC(void, APPL_CODE, anotherFuncPtr)(void); // Declares variable anotherFuncPtr
 */
#define P2FUNC(rettype, memclass, fctname)                  rettype (* fctname)

/**
 * @brief Macro for pointer to constant data declaration.
 * @param ptrtype The type of the data that the pointer refers to.
 * @param memclass Memory class of the pointer variable itself. (Placeholder here)
 * @param ptrclass Target memory class or pointer class qualifier (e.g. AUTOMATIC). (Placeholder here)
 * @details Expands to `const ptrtype *`.
 * @example P2CONST(uint8, APPL_VAR, COMMON_CONST) myConstPtrToConstData;
 */
#define P2CONST(ptrtype, memclass, ptrclass)                const ptrtype *

/**
 * @brief Macro for pointer to variable data declaration.
 * @param ptrtype The type of the data that the pointer refers to.
 * @param memclass Memory class of the pointer variable itself. (Placeholder here)
 * @param ptrclass Target memory class or pointer class qualifier. (Placeholder here)
 * @details Expands to `ptrtype *`.
 * @example P2VAR(uint8, APPL_VAR, APPL_VAR) myPtrToVarData;
 */
#define P2VAR(ptrtype, memclass, ptrclass)                  ptrtype *

/**
 * @brief Macro for constant data definition.
 * @param consttype The type of the constant.
 * @param memclass The memory class where the constant shall be placed (e.g., CONFIG_CONST).
 * @details Expands to `const consttype memclass`. If `memclass` is empty, it's `const consttype`.
 * @example CONST(uint8, CONFIG_CONST) MyConfigConstant = 5U;
 */
#define CONST(consttype, memclass)                          const consttype memclass

/**
 * @brief Macro for variable data definition.
 * @param vartype The type of the variable.
 * @param memclass The memory class where the variable shall be placed (e.g., APPL_VAR).
 * @details Expands to `vartype memclass`. If `memclass` is empty, it's `vartype`.
 * @example VAR(uint8, APPL_VAR) myApplicationVariable;
 */
#define VAR(vartype, memclass)                              vartype memclass


/* Standard C keywords abstraction */
#ifndef _VOLATILE_
  #define _VOLATILE_          volatile
#endif

#ifndef _INLINE_
  #define _INLINE_            inline
#endif

/* STATIC macro for static functions and variables */
/* Using a non-keyword name like STATIC avoids conflict if 'static' is used directly */
#ifndef STATIC
  #define STATIC              static
#endif

#endif /* COMPILER_H */
