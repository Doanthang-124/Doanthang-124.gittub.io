# Unit Testing Strategy for STM32F3 MCAL Drivers

## 1. Introduction

Unit testing is crucial for ensuring the reliability and correctness of MCAL (Microcontroller Abstraction Layer) drivers. For drivers like Dio and Port, unit tests verify that the software logic correctly manipulates hardware registers as per the AUTOSAR specifications and handles error conditions appropriately, independent of the full hardware setup.

This document outlines a strategy for unit testing the `Dio.c` and `Port.c` drivers developed for the STM32F3 platform.

## 2. Recommended Testing Framework: Ceedling

**Ceedling** (which bundles Unity for test assertions and CMock for mocking) is highly recommended for unit testing C projects, especially in embedded systems.

-   **Unity**: A simple assertion library for C.
-   **CMock**: A tool that generates mock interfaces for C functions based on their header files. This is essential for isolating the unit under test.
-   **Ruby Rake Build System**: Ceedling uses Rake to automate the test build process, compilation of mocks, test execution, and reporting.

*Alternatives*: GoogleTest (primarily C++, but can be used for C), or custom simple test harnesses. However, Ceedling provides a comprehensive, ready-to-use environment.

## 3. Scope of Unit Tests

The primary goal is to test the logic within each function of `Dio.c` and `Port.c`:

-   **Correctness of Hardware Interaction**: Verify that the driver functions attempt to write the correct values to the appropriate (mocked) hardware registers based on input parameters and configuration.
-   **Error Handling**: Ensure that development errors (as per `PORT_DEV_ERROR_DETECT` and `DIO_DEV_ERROR_DETECT`) are correctly reported by calling `Det_ReportError` with the specified error codes.
-   **State Management**: If functions rely on or modify internal state variables, these state changes should be verified.
-   **Boundary Conditions**: Test with valid and invalid inputs, edge cases, and null pointers where applicable.

Unit tests do **not** aim to test:
-   The C compiler's correctness.
-   The actual STM32F3 hardware functionality (this is for integration/hardware testing).
-   The full AUTOSAR stack integration.

## 4. Mocking Strategy

To isolate the unit under test (e.g., a function in `Dio.c`), its external dependencies must be mocked.

### 4.1. Hardware Registers (GPIO)

-   **Mock Register Structures**: In the test files (e.g., `test_Port.c`), define C structures that mirror the STM32F3 GPIO peripheral's register map (`GPIO_TypeDef`). For example:
    ```c
    // In test_common_gpio_mock.h (or similar)
    typedef struct {
        volatile uint32_t MODER;
        volatile uint32_t OTYPER;
        volatile uint32_t OSPEEDR;
        volatile uint32_t PUPDR;
        volatile uint32_t IDR;
        volatile uint32_t ODR;
        volatile uint32_t BSRR;
        volatile uint32_t LCKR;
        volatile uint32_t AFR[2];
    } GPIO_TypeDef_Mock;

    // In test_Port.c or a shared mock setup file
    GPIO_TypeDef_Mock mock_gpioa_regs;
    GPIO_TypeDef_Mock mock_gpiob_regs;
    // ... and so on for all relevant GPIO ports.
    ```
-   **Redirection**: During the test build, preprocessor directives will redirect the driver's access from actual hardware addresses to these mock structures. This is typically done in the Ceedling project configuration (`project.yml`) or a test-specific header.
    ```c
    // In a test-specific header automatically included by Ceedling, or in project.yml
    #define GPIOA ((GPIO_TypeDef*) &mock_gpioa_regs) // Note: Original stm32f3xx.h uses (GPIO_TypeDef *)
    #define GPIOB ((GPIO_TypeDef*) &mock_gpiob_regs)
    // ... etc.
    // This overrides the definitions from the actual stm32f3xx.h for test builds.
    ```
-   **Test Control**: Before each test case, the mock registers can be initialized to a known state. After the driver function is called, the test can assert the values of these mock registers.

### 4.2. `Det_ReportError`

-   **CMock Generation**: Ceedling's CMock tool will automatically generate a mock implementation for `Det_ReportError` based on `Det.h`.
-   **Expectations and Verification**: In test cases, you can set expectations for `Det_ReportError` calls:
    ```c
    // Example using CMock syntax in a test case
    Det_ReportError_Expect(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_ID, PORT_E_PARAM_CONFIG);
    Port_Init(NULL_PTR); // Call the function that should trigger the error
    ```
    CMock will verify that `Det_ReportError` was called with the exact parameters expected. You can also specify how many times it should be called, or ignore calls if not relevant to a specific test.

### 4.3. Other Dependencies
- If `Dio.c` or `Port.c` were to call other modules (e.g., `Mcu_GetClockFrequency`), those would also be mocked using CMock based on their respective header files.

## 5. Example Test Cases - Dio Driver (`test_Dio.c`)

Each test function should follow the Arrange-Act-Assert pattern.

-   `void test_Dio_ReadChannel_ValidPin_ReadsLow(void)`
    -   Arrange: `mock_gpioa_regs.IDR = 0; // Pin PA0 is low`
    -   Act: `Dio_LevelType level = Dio_ReadChannel(DIO_CHANNEL_PA0);`
    -   Assert: `TEST_ASSERT_EQUAL(STD_LOW, level);`
-   `void test_Dio_ReadChannel_ValidPin_ReadsHigh(void)`
    -   Arrange: `mock_gpioa_regs.IDR = (1U << 0); // Pin PA0 is high`
    -   Act: `Dio_LevelType level = Dio_ReadChannel(DIO_CHANNEL_PA0);`
    -   Assert: `TEST_ASSERT_EQUAL(STD_HIGH, level);`
-   `void test_Dio_ReadChannel_InvalidChannelId_ReportsError(void)`
    -   Arrange: `Det_ReportError_Expect(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_READCHANNEL_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);`
    -   Act: `Dio_ReadChannel(INVALID_DIO_CHANNEL_ID); // Define INVALID_DIO_CHANNEL_ID appropriately`
    -   Assert: (CMock handles assertion of Det_ReportError call)
-   `void test_Dio_WriteChannel_ValidPin_LevelLow_SetsBsrrCorrectly(void)`
    -   Arrange: `mock_gpioa_regs.BSRR = 0;`
    -   Act: `Dio_WriteChannel(DIO_CHANNEL_PA0, STD_LOW);`
    -   Assert: `TEST_ASSERT_EQUAL_HEX32((1U << (0 + 16)), mock_gpioa_regs.BSRR); // Check BR0 bit`
-   `void test_Dio_WriteChannel_ValidPin_LevelHigh_SetsBsrrCorrectly(void)`
    -   Arrange: `mock_gpioa_regs.BSRR = 0;`
    -   Act: `Dio_WriteChannel(DIO_CHANNEL_PA0, STD_HIGH);`
    -   Assert: `TEST_ASSERT_EQUAL_HEX32((1U << 0), mock_gpioa_regs.BSRR); // Check BS0 bit`
-   `void test_Dio_FlipChannel_FromLowToHigh_ReturnsHighAndSetsBsrr(void)`
    -   Arrange: `mock_gpioa_regs.IDR = 0; // Pin PA0 is low`
                 `mock_gpioa_regs.BSRR = 0;`
    -   Act: `Dio_LevelType newLevel = Dio_FlipChannel(DIO_CHANNEL_PA0);`
    -   Assert: `TEST_ASSERT_EQUAL(STD_HIGH, newLevel);`
                 `TEST_ASSERT_EQUAL_HEX32((1U << 0), mock_gpioa_regs.BSRR);`

## 6. Example Test Cases - Port Driver (`test_Port.c`)

-   `void test_Port_Init_SinglePin_OutputPushPull_ConfiguresAllRegistersCorrectly(void)`
    -   Arrange: Define a `Port_ConfigType` in the test with one pin (e.g., PA5) configured as output push-pull, low speed, no pull, initial level low. Clear all mock_gpioa_regs.
    -   Act: `Port_Init(&testPortConfig);`
    -   Assert:
        -   `TEST_ASSERT_EQUAL_HEX32(expected_moder_val, mock_gpioa_regs.MODER);`
        -   `TEST_ASSERT_EQUAL_HEX32(expected_otyper_val, mock_gpioa_regs.OTYPER);`
        -   `TEST_ASSERT_EQUAL_HEX32(expected_ospeedr_val, mock_gpioa_regs.OSPEEDR);`
        -   `TEST_ASSERT_EQUAL_HEX32(expected_pupdr_val, mock_gpioa_regs.PUPDR);`
        -   Verify `mock_gpioa_regs.BSRR` for initial level setting.
-   `void test_Port_Init_NullConfig_ReportsErrorToDet(void)`
    -   Arrange: `Det_ReportError_Expect(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_ID, PORT_E_PARAM_CONFIG);`
    -   Act: `Port_Init(NULL_PTR);`
    -   Assert: (CMock handles assertion)
-   `void test_Port_SetPinDirection_UnchangeablePin_ReportsError(void)`
    -   Arrange: Configure a pin in `testPortConfig` with `PinDirectionChangeable = FALSE`. Initialize Port with this config.
                 `Det_ReportError_Expect(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SETPINDIRECTION_ID, PORT_E_DIRECTION_UNCHANGEABLE);`
    -   Act: `Port_SetPinDirection(TEST_PIN_ID, PORT_PIN_OUT); // Define TEST_PIN_ID`
    -   Assert: (CMock handles assertion)
-   `void test_Port_SetPinMode_ToAnalog_ChangeablePin_SetsModerCorrectly(void)`
    -   Arrange: Configure a pin as changeable. Initialize Port. `mock_gpioa_regs.MODER = 0; // Assume pin PA0, pinIndex = 0`
    -   Act: `Port_SetPinMode(PORT_PIN_PA0, PORT_PIN_MODE_GPIO_IN_ANALOG);`
    -   Assert: `TEST_ASSERT_EQUAL_HEX32((3UL << (0 * 2)), mock_gpioa_regs.MODER & (3UL << (0 * 2))); // Check only relevant bits`

## 7. Build Process for Tests

-   A separate build configuration (typically managed by Ceedling via its `project.yml` file) is used for unit tests.
-   This build compiles the specific `.c` file of the driver under test along with the test file itself (e.g., `test_Dio.c`) and any generated mocks.
-   It excludes `main.c` from the application and other drivers not under test (or uses their mocked versions).
-   The test executable runs on the host machine (e.g., PC), not the embedded target.

## 8. Conclusion

Adopting a unit testing strategy with a framework like Ceedling significantly improves the quality and robustness of MCAL drivers. It allows for:
-   Early detection of bugs in the driver logic.
-   Prevention of regressions when changes are made.
-   Better understanding and documentation of driver behavior through tests.
-   Design improvement by forcing modularity and testability.
