# AUTOSAR Classic MCAL Drivers for STM32F3

## Overview
This project provides a basic implementation of AUTOSAR 4.x compliant MCAL (Microcontroller Abstraction Layer) drivers for the STM32F3 series of microcontrollers. The primary focus is on the Dio (Digital Input/Output) and Port (Port Configuration) drivers, along with common AUTOSAR base components.

## Modules Implemented
- **Dio Driver**: For reading digital input channels and writing to digital output channels.
- **Port Driver**: For configuring GPIO pins, including their direction, mode (GPIO, Alternate Function, Analog), pull-up/pull-down resistors, output speed, and initial levels.
- **Common Components**:
    - `Std_Types.h`: Standard AUTOSAR type definitions.
    - `Platform_Types.h`: Platform-specific (e.g., CPU and compiler related) type definitions.
    - `Mcal.h`: A general top-level header for the MCAL layer.
    - `Det.c` / `Det.h`: A stub implementation for the Development Error Tracer (DET), allowing driver code to compile with DET calls.
    - `Compiler.h`: Compiler abstraction macros.
    - `stm32f3xx.h`: A basic device header providing register definitions for STM32F3 GPIO peripherals.

## Directory Structure
The project is organized as follows, within the `AUTOSAR_Classic_MCAL_STM32F3` root:
- `Include/`: Public MCAL headers for application and other module access (e.g., `Dio.h`, `Port.h`).
- `Config/`: Configuration files for the drivers.
    - `Xxx_Cfg.h`: Pre-compile configuration headers.
    - `Xxx_PBcfg.h` & `Xxx_PBcfg.c`: Post-build configuration headers and source files.
- `Common/`: Shared base AUTOSAR types, utilities, and MCU-specific headers.
    - `Include/`: Header files for common components.
    - `Src/`: Source files for common utilities (e.g., `Det.c`).
- `Dio/`: Dio driver specific implementation.
    - `Include/`: Private Dio driver headers (e.g., `Dio_Regs.h`).
    - `Src/`: Dio driver source file (`Dio.c`).
- `Port/`: Port driver specific implementation.
    - `Include/`: Private Port driver headers (e.g., `Port_Regs.h`).
    - `Src/`: Port driver source file (`Port.c`).
- `App_Examples/SimpleLedToggle/`: An example application demonstrating the usage of the Dio and Port drivers.
    - `main.c`: Source code for the example.
    - (Linker script should be placed here by the user).
- `build/`: This directory is created by the Makefile during the build process and contains all output files (object files, ELF, MAP, BIN, HEX).
- `Makefile`: The Makefile used to build the sample application and drivers.
- `README.md`: This documentation file.

## Prerequisites for Building
To build the sample application, you will need:
1.  **ARM GCC Toolchain**: The `arm-none-eabi-gcc` compiler and associated tools (linker, objcopy, etc.) must be installed and accessible in your system's PATH.
2.  **Make Utility**: The GNU `make` utility is required to process the Makefile.
3.  **STM32F3 Linker Script**: A linker script (`.ld` file) specific to your STM32F3 target microcontroller (e.g., `stm32f303xc_flash.ld` for an STM32F303VC). This file defines the memory layout for the linker.
    -   **This linker script is NOT included in this repository.**
    -   You can typically obtain one by creating a new project for your target MCU in STM32CubeIDE or by generating it with STM32CubeMX.
    -   The Makefile expects this script to be placed in the `App_Examples/SimpleLedToggle/` directory.

## How to Build
1.  Ensure all prerequisites listed above are met.
2.  Open a terminal or command prompt.
3.  Navigate to the root directory of this project (`AUTOSAR_Classic_MCAL_STM32F3/`).
4.  To clean previous build artifacts (optional, but recommended for a fresh build):
    ```bash
    make clean
    ```
5.  To build the project:
    ```bash
    make
    ```
6.  If the build is successful, the output files (e.g., `App_SimpleLedToggle.elf`, `App_SimpleLedToggle.bin`, `App_SimpleLedToggle.hex`) will be located in the `build/` directory.

## Sample Application (`SimpleLedToggle`)
The sample application provided in `App_Examples/SimpleLedToggle/` demonstrates basic usage of the Port and Dio drivers:
- It initializes the Port driver using the post-build configuration defined in `Port_PBcfg.c`. This sets up the specific pin (e.g., PA5) as a GPIO output.
- It then initializes the Dio driver using its post-build configuration.
- In an infinite loop, it calls `Dio_FlipChannel()` to toggle the state of the configured LED pin (PA5 in the example configuration).
- A simple software delay is used between toggles to make the blinking visible.

## Configuration
The drivers are configured using a combination of pre-compile and post-build options:
- **Pre-compile configurations**: These are defined in `Config/Xxx_Cfg.h` files (e.g., `Dio_Cfg.h`, `Port_Cfg.h`). They control aspects like enabling/disabling specific APIs (e.g., `Dio_GetVersionInfo`), development error detection, and defining symbolic names for channels/pins.
- **Post-build configurations**: These are defined in `Config/Xxx_PBcfg.c` files with declarations in corresponding `Config/Xxx_PBcfg.h` headers. These structures specify the detailed configuration for each Dio channel and Port pin used by the application (e.g., direction, mode, pull-resistors for Port pins). The `Port_Init` and `Dio_Init` functions receive pointers to these configuration structures.

## AUTOSAR Compliance
The implemented Dio and Port drivers aim to adhere to the AUTOSAR Classic Platform Release 4.x specifications. The level of compliance will depend on the completeness of feature implementation and adherence to specific AUTOSAR requirements and constraints.

## Limitations / Future Work
- **Basic Implementation**: This project provides a foundational set of drivers. Full validation, including compliance with AUTOSAR test suites, has not been performed.
- **DET Stub**: The Development Error Tracer (DET) is currently a stub implementation. It allows code to compile but does not provide full error tracing capabilities.
- **Linker Script Required**: Users must provide their own linker script appropriate for their specific STM32F3 target microcontroller.
- **Limited MCAL Scope**: Only Dio, Port, and common base modules are included. Future work could involve adding other MCAL modules like Adc, Pwm, Spi, Can, etc.
- **Testing**: Comprehensive unit and integration testing is required.
- **Compiler Abstraction**: While `Compiler.h` is present, full compiler abstraction for various toolchains might need further refinement.
- **Memory Mapping**: `MemMap.h` files are included as placeholders; actual memory section mapping is not yet implemented.
