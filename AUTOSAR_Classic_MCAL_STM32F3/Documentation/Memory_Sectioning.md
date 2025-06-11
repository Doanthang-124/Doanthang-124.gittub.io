# Memory Sectioning Strategy with Compiler.h

This document outlines how memory sectioning is conceptually achieved using the
AUTOSAR `Compiler.h` abstraction macros and how it relates to the linker script.

## 1. Compiler Abstraction (`Compiler.h`)

The `AUTOSAR_Classic_MCAL_STM32F3/Common/Include/Compiler.h` file defines macros like `FUNC`, `VAR`, and `CONST`,
along with memory class identifiers (e.g., `PORT_CODE`, `DIO_CONST`, `DET_VAR`).

Example definitions in `Compiler.h` for GCC:

```c
/* Memory class for Port driver code */
#define PORT_CODE_SEC __attribute__((section(".mcal_port_code")))
#define PORT_CODE PORT_CODE_SEC /* Assign active definition */

/* Memory class for Port driver constants */
#define PORT_CONST_SEC __attribute__((section(".mcal_port_const")))
#define PORT_CONST PORT_CONST_SEC

/* Macro for function definition */
#define FUNC(rettype, memclass) rettype memclass

/* Macro for constant definition */
#define CONST(consttype, memclass) const consttype memclass
```

When used in code, for example:
`FUNC(void, PORT_CODE) Port_Init(void) { /* ... */ }`
This expands (for GCC) to:
`void __attribute__((section(".mcal_port_code"))) Port_Init(void) { /* ... */ }`

This tells the compiler to place the object code for `Port_Init` into a section named `.mcal_port_code`.

Similarly for constants:
`CONST(uint8, PORT_CONST) Port_SomeConstant = 10;`
Expands to:
`const uint8 __attribute__((section(".mcal_port_const"))) Port_SomeConstant = 10;`

## 2. Linker Script (`.ld` file)

The linker script (e.g., `stm32f303xc_flash.ld`, **which is user-provided and not part of this repository**)
is responsible for defining where these named sections are placed in the microcontroller's memory map.

An excerpt from a linker script might look like this:

```ld
MEMORY
{
  FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 256K
  RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 40K  /* Example for STM32F303VC */
  /* Add other memory regions if needed, e.g., CCMRAM, specific RAM banks */
}

SECTIONS
{
  .text : /* Standard code section */
  {
    . = ALIGN(4);
    *(.isr_vector)     /* Vector table */
    *(.text)           /* Default code */
    *(.text*)
    *(.rodata)         /* Default read-only data */
    *(.rodata*)
    . = ALIGN(4);
  } >FLASH

  /* Custom MCAL Code Sections */
  .mcal_port_code :
  {
    . = ALIGN(4);
    *(.mcal_port_code) /* All code marked with PORT_CODE */
    . = ALIGN(4);
  } >FLASH /* Place Port driver code in FLASH */

  .mcal_dio_code :
  {
    . = ALIGN(4);
    *(.mcal_dio_code)
    . = ALIGN(4);
  } >FLASH

  .mcal_det_code :
  {
    . = ALIGN(4);
    *(.mcal_det_code)
    . = ALIGN(4);
  } >FLASH

  /* Add other specific code sections here */
  /* e.g., .fast_code for time-critical routines placed in RAM if supported */

  /* Custom MCAL Constant Sections */
  .mcal_port_const :
  {
    . = ALIGN(4);
    *(.mcal_port_const) /* Constants for Port driver */
    . = ALIGN(4);
  } >FLASH /* Constants are read-only, so in FLASH */

  .config_const : /* For Post-Build Config data structures */
  {
    . = ALIGN(4);
    *(.config_const)
    . = ALIGN(4);
  } >FLASH

  /* Default initialized data section */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol for start of .data */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */
    . = ALIGN(4);
    _edata = .;        /* define a global symbol for end of .data */
  } >RAM AT> FLASH     /* Load from FLASH, copy to RAM at startup */

  /* Custom MCAL Variable Sections (RAM) */
  .mcal_det_var :
  {
    . = ALIGN(4);
    *(.mcal_det_var)   /* Variables for Det driver, e.g. error buffer */
    . = ALIGN(4);
  } >RAM AT> FLASH /* If initialized, otherwise just >RAM for .bss like */

  /* Uninitialized data section (BSS) */
  .bss :
  {
    . = ALIGN(4);
    _sbss = .;         /* define a global symbol for start of .bss */
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;         /* define a global symbol for end of .bss */
  } >RAM

  /* Add other sections as needed */
}
```

**Explanation of Linker Script Snippet:**
- `MEMORY`: Defines the available memory regions (FLASH, RAM) and their sizes/origins.
- `SECTIONS`: Defines how input sections (from compiled object files) are grouped and placed into output sections, and where those output sections reside in the `MEMORY` regions.
- `.mcal_port_code : { *(.mcal_port_code) } >FLASH`: This rule tells the linker to take all input sections named `.mcal_port_code` (which were created by the `__attribute__((section(".mcal_port_code")))` in the C code) and place them together in an output section also named `.mcal_port_code`, which is then located in the `FLASH` memory region.

## 3. Benefits of Memory Sectioning

- **Modularity and Organization**: Code and data for specific modules can be grouped together.
- **Memory Optimization**:
    - Placing infrequently used code or constants in slower/cheaper memory.
    - Placing performance-critical code or data in faster memory (e.g., CCMRAM on STM32, or RAM for execution).
    - Ensuring specific data (e.g., calibration values) is at fixed addresses.
- **Linker Control**: Finer control over memory layout, which can be important for bootloaders, memory protection units (MPU), and safety requirements.
- **Debugging**: Easier to locate specific module code/data in memory maps.

By using the `Compiler.h` abstractions, the C code remains portable across different compilers, with only `Compiler.h` needing adaptation for the compiler's specific sectioning syntax. The linker script then handles the final placement.
