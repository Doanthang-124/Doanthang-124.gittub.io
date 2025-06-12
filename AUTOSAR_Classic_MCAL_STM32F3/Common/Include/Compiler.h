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
 * The actual section names (e.g., ".mcal_dio_code", ".mcal_port_code") are conventions
 * and must match definitions in the linker script.
 */

/* Default definitions (empty - no specific sectioning applied by default) */
#define DIO_CODE_DEFAULT
#define PORT_CODE_DEFAULT
#define DET_CODE_DEFAULT
#define MCU_CODE_DEFAULT
#define COMMON_CODE_DEFAULT
#define APPL_CODE_DEFAULT
#define CONFIG_CODE_DEFAULT /* For PostBuildConfig data, usually goes to .rodata or similar */
#define DEFAULT_CODE_DEFAULT

#define DIO_CONST_DEFAULT
#define PORT_CONST_DEFAULT
#define DET_CONST_DEFAULT
#define CONFIG_CONST_DEFAULT /* For Config constants, e.g. in PBcfg.c */
#define DEFAULT_CONST_DEFAULT

#define DIO_VAR_DEFAULT
#define PORT_VAR_DEFAULT
#define DET_VAR_DEFAULT   /* For Det runtime variables like the error buffer */
#define GPT_VAR_DEFAULT             /* For GPT runtime variables */
#define DEFAULT_VAR_DEFAULT


/* Compiler specific section mapping (Example for GCC) */
#ifdef __GNUC__
    #define GPT_CODE_SEC                __attribute__((section(".mcal_gpt_code"))) /* Added for GPT */
    #define DIO_CODE_SEC                __attribute__((section(".mcal_dio_code")))
    #define PORT_CODE_SEC               __attribute__((section(".mcal_port_code")))
    #define DET_CODE_SEC                __attribute__((section(".mcal_det_code")))
    #define MCU_CODE_SEC                __attribute__((section(".mcal_mcu_code")))
    #define COMMON_CODE_SEC             __attribute__((section(".mcal_common_code")))
    #define APPL_CODE_SEC               __attribute__((section(".appl_code")))
    #define CONFIG_CODE_SEC             __attribute__((section(".config_code"))) /* For executable config code if any */
    #define DEFAULT_CODE_SEC            /* Can map to .text or a default code section */

    #define DIO_CONST_SEC               __attribute__((section(".mcal_dio_const")))
    #define PORT_CONST_SEC              __attribute__((section(".mcal_port_const")))
    #define DET_CONST_SEC               __attribute__((section(".mcal_det_const")))
    #define CONFIG_CONST_SEC            __attribute__((section(".config_const"))) /* For PBcfg structures */
    #define DEFAULT_CONST_SEC           __attribute__((section(".rodata"))) /* General read-only data */

    #define DIO_VAR_SEC                 __attribute__((section(".mcal_dio_var")))
    #define PORT_VAR_SEC                __attribute__((section(".mcal_port_var")))
    #define DET_VAR_SEC                 __attribute__((section(".mcal_det_var"))) /* For Det_ErrorBuffer */
    #define GPT_VAR_SEC                 __attribute__((section(".mcal_gpt_var")))   /* Added for GPT */
    #define DEFAULT_VAR_SEC             /* Can map to .data, .bss or a default RAM section */

    #define GPT_CONST_SEC               __attribute__((section(".mcal_gpt_const"))) /* Added for GPT */
#else
    /* Define for other compilers or default to empty if no specific sectioning */
    #define GPT_CODE_SEC                GPT_CODE_DEFAULT /* Added for GPT */
    #define DIO_CODE_SEC                DIO_CODE_DEFAULT
    #define PORT_CODE_SEC               PORT_CODE_DEFAULT
    #define DET_CODE_SEC                DET_CODE_DEFAULT
    #define MCU_CODE_SEC                MCU_CODE_DEFAULT
    #define COMMON_CODE_SEC             COMMON_CODE_DEFAULT
    #define APPL_CODE_SEC               APPL_CODE_DEFAULT
    #define CONFIG_CODE_SEC             CONFIG_CODE_DEFAULT
    #define DEFAULT_CODE_SEC            DEFAULT_CODE_DEFAULT

    #define DIO_CONST_SEC               DIO_CONST_DEFAULT
    #define PORT_CONST_SEC              PORT_CONST_DEFAULT
    #define DET_CONST_SEC               DET_CONST_DEFAULT
    #define GPT_CONST_SEC               GPT_CONST_DEFAULT /* Added for GPT */
    #define CONFIG_CONST_SEC            CONFIG_CONST_DEFAULT
    #define DEFAULT_CONST_SEC           DEFAULT_CONST_DEFAULT

    #define DIO_VAR_SEC                 DIO_VAR_DEFAULT
    #define PORT_VAR_SEC                PORT_VAR_DEFAULT
    #define DET_VAR_SEC                 DET_VAR_DEFAULT
    #define GPT_VAR_SEC                 GPT_VAR_DEFAULT /* Added for GPT */
    #define DEFAULT_VAR_SEC             DEFAULT_VAR_DEFAULT
#endif

/* Assign the active section macros to the generic AUTOSAR memclass names */
#define DIO_CODE                    DIO_CODE_SEC
#define PORT_CODE                   PORT_CODE_SEC
#define DET_CODE                    DET_CODE_SEC
#define GPT_CODE                    GPT_CODE_SEC /* Added for GPT */
#define MCU_CODE                    MCU_CODE_SEC
#define COMMON_CODE                 COMMON_CODE_SEC
#define APPL_CODE                   APPL_CODE_SEC
#define CONFIG_CODE                 CONFIG_CODE_SEC /* For code related to config, if any */
#define DEFAULT_CODE                DEFAULT_CODE_SEC

#define DIO_CONST                   DIO_CONST_SEC
#define PORT_CONST                  PORT_CONST_SEC
#define DET_CONST                   DET_CONST_SEC
#define GPT_CONST                   GPT_CONST_SEC /* Added for GPT */
#define CONFIG_CONST                CONFIG_CONST_SEC /* For PostBuild Config data structures */
#define DEFAULT_CONST               DEFAULT_CONST_SEC

#define DIO_VAR                     DIO_VAR_SEC
#define PORT_VAR                    PORT_VAR_SEC
#define DET_VAR                     DET_VAR_SEC
#define GPT_VAR                     GPT_VAR_SEC   /* Added for GPT */
#define DEFAULT_VAR                 DEFAULT_VAR_SEC


/* Function macro: Maps to function definition with specific memory class */
#define FUNC(rettype, memclass)                             rettype memclass

/* Pointer to function type definition macro */
/* Example: typedef P2FUNC_TYPEDEF(void, PORT_APPL_CODE, Port_NotificationFuncType)(uint8 channel); */
#define P2FUNC_TYPEDEF(rettype, memclass_ptr, fctname)      rettype (* fctname)

/* Pointer to function macro (for variable declaration) */
#define P2FUNC(rettype, memclass_ptr, fctname)              rettype (* fctname)

/* Pointer to constant data */
/* ptrclass is an AUTOSAR concept like AUTOMATIC, TYPEDEF, possibly memclass for pointer itself */
#define P2CONST(ptrtype, memclass_ptr, ptrclass_target)     const ptrclass_target ptrtype * memclass_ptr

/* Pointer to variable data */
#define P2VAR(ptrtype, memclass_ptr, ptrclass_target)       ptrclass_target ptrtype * memclass_ptr

/* Constant data definition */
#define CONST(consttype, memclass)                          const consttype memclass

/* Variable data definition */
#define VAR(vartype, memclass)                              vartype memclass


/* Define _VOLATILE_ and _INLINE_ if not already present */
#ifndef _VOLATILE_
  #define _VOLATILE_ volatile
#endif

#ifndef _INLINE_
  #define _INLINE_ inline __attribute__((always_inline)) /* Example for GCC to suggest stronger inlining */
#endif

/* Define STATIC if not already present (often used for static functions/variables) */
#ifndef STATIC
  #define STATIC static
#endif

#endif /* COMPILER_H */
