/*
 * STM32F303VCTx Linker Script.
 * Target: STM32F303VCT6 (256KB FLASH, 40KB SRAM)
 *
 * NOTE: This is a template. Verify memory origins and lengths against your specific
 * hardware datasheet and board configuration. Stack and Heap sizes are examples.
 */

/* Entry Point */
ENTRY(Reset_Handler)

/* Highest memory address */
_estack = 0x2000A000; /* End of SRAM for stack pointer (40KB SRAM: 0x20000000 + 40*1024) */
                      /* STM32F303VC has 40KB SRAM1 (0x20000000-0x20007FFF) + 8KB SRAM2 (0x20008000-0x20009FFF) often aliased */
                      /* For simplicity, considering a contiguous 40KB block here. Check datasheet for actual mapping. */

/* Memory Regions */
MEMORY
{
  FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 256K /* STM32F303VC has 256KB FLASH */
  RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 40K  /* Main SRAM (SRAM1+SRAM2 combined or largest block) */
  /* CCMRAM (rw)  : ORIGIN = 0x10000000, LENGTH = 8K */ /* Optional: CCM RAM if used, not included by default here */
}

/* Define output sections */
SECTIONS
{
  /* The startup code goes first in FLASH */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >FLASH

  /* Program code and read-only data */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbol at end of code */
  } >FLASH

  /* Read-only data */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >FLASH

  /* Custom MCAL Code Sections (as defined by __attribute__((section)) in Compiler.h) */
  .mcal_dio_code : { . = ALIGN(4); *(.mcal_dio_code) *(.mcal_dio_code*) . = ALIGN(4); } >FLASH
  .mcal_port_code : { . = ALIGN(4); *(.mcal_port_code) *(.mcal_port_code*) . = ALIGN(4); } >FLASH
  .mcal_det_code : { . = ALIGN(4); *(.mcal_det_code) *(.mcal_det_code*) . = ALIGN(4); } >FLASH
  .mcal_mcu_code : { . = ALIGN(4); *(.mcal_mcu_code) *(.mcal_mcu_code*) . = ALIGN(4); } >FLASH
  .mcal_common_code : { . = ALIGN(4); *(.mcal_common_code) *(.mcal_common_code*) . = ALIGN(4); } >FLASH
  /* Add .mcal_gpt_code here when GPT driver is more complete */
  /* .mcal_gpt_code : { . = ALIGN(4); *(.mcal_gpt_code) *(.mcal_gpt_code*) . = ALIGN(4); } >FLASH */

  .appl_code : { . = ALIGN(4); *(.appl_code) *(.appl_code*) . = ALIGN(4); } >FLASH
  .config_code : { . = ALIGN(4); *(.config_code) *(.config_code*) . = ALIGN(4); } >FLASH


  /* Custom MCAL Constant Data Sections */
  .mcal_dio_const : { . = ALIGN(4); *(.mcal_dio_const) *(.mcal_dio_const*) . = ALIGN(4); } >FLASH
  .mcal_port_const : { . = ALIGN(4); *(.mcal_port_const) *(.mcal_port_const*) . = ALIGN(4); } >FLASH
  .mcal_det_const : { . = ALIGN(4); *(.mcal_det_const) *(.mcal_det_const*) . = ALIGN(4); } >FLASH
  .config_const : /* For Post-Build Config data structures like PortConfig, DioConfig */
  {
    . = ALIGN(4);
    *(.config_const)
    *(.config_const*)
    . = ALIGN(4);
  } >FLASH
  /* Add .mcal_gpt_const here when GPT driver is more complete */

  /* Initialized data sections goes into RAM, load LMA copy from FLASH */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol for start of .data */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */
    . = ALIGN(4);
    _edata = .;        /* define a global symbol for end of .data */
  } >RAM AT> FLASH

  /* Custom MCAL Variable Data Sections (RAM) */
  /* These will be zero-initialized if not explicitly initialized (like .bss) or copied from FLASH if initialized (like .data) */
  /* For simplicity, treating them like .bss unless they have initializers. */
  /* If they have initializers, they should be in a .data-like section with AT>FLASH. */
  .mcal_dio_var (NOLOAD) : { . = ALIGN(4); _smcal_dio_var = .; *(.mcal_dio_var) *(.mcal_dio_var*) . = ALIGN(4); _emcal_dio_var = .; } >RAM
  .mcal_port_var (NOLOAD) : { . = ALIGN(4); _smcal_port_var = .; *(.mcal_port_var) *(.mcal_port_var*) . = ALIGN(4); _emcal_port_var = .; } >RAM
  .mcal_det_var (NOLOAD) : /* For Det_ErrorBuffer, typically zero-init or no-init */
  {
    . = ALIGN(4);
    _smcal_det_var = .;
    *(.mcal_det_var)
    *(.mcal_det_var*)
   . = ALIGN(4);
   _emcal_det_var = .;
  } >RAM
  /* Add .mcal_gpt_var here when GPT driver is more complete */


  /* Uninitialized data section */
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

  /* User heap and stack */
  /* Adjust sizes as needed by the application */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + DEFINED(_Min_Heap_Size) ? _Min_Heap_Size : 0x200; /* Minimum Heap Size */
    . = . + DEFINED(_Min_Stack_Size) ? _Min_Stack_Size : 0x400; /* Minimum Stack Size */
    . = ALIGN(8);
  } >RAM

  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
