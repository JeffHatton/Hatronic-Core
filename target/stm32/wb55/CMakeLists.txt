cmake_minimum_required(VERSION 3.15.3)

# set(CMAKE_VERBOSE_MAKEFILE ON)

# turn on compile commands so VS Code can use the include paths
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Path to toolchain file. This one has to be before 'project()' below
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-none-eabi-gcc.cmake)

# Setup project, output and linker file
project(STM32_WB55_Template)
set(EXECUTABLE ${PROJECT_NAME}.elf)
set(LINKER_FILE ${CMAKE_SOURCE_DIR}/ST/STM32WB55RGVX_FLASH.ld)

# Setup language properties
enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Setup ST Cube References
set(STCUBE_BASE "/home/jeff/dev/libs/stm32/cube/wb/")
set(STCUBE_DRIVERS_CMSIS "${STCUBE_BASE}/Drivers/CMSIS/Device/ST/STM32WBxx")


# Gather Source Files
FILE(GLOB_RECURSE ST_SOURCES ${STCUBE_BASE}Drivers/STM32WBxx_HAL_Driver/Src/*.c) 
set(SRC_FILES
        src/main.c
        src/stm32wbxx_it.c
        src/stm32wbxx_hal_msp.c
        ST/system_stm32wbxx.c
        ST/startup_stm32wb55rgvx.s
        )

# Build the executable based on the source files
add_executable(${EXECUTABLE} 
        ${SRC_FILES}
        ${ST_SOURCES})

# List of compiler defines, prefix with -D compiler option
target_compile_definitions(${EXECUTABLE} PRIVATE
         -DUSE_HAL_DRIVER
         -DSTM32WB55xx
        )

# List of include directories
target_include_directories(${EXECUTABLE} PRIVATE
        src
        inc
        ${STCUBE_DRIVERS_CMSIS}/Include
        ${STCUBE_BASE}/Drivers/STM32WBxx_HAL_Driver/Inc/Legacy
        ${STCUBE_BASE}/Drivers/STM32WBxx_HAL_Driver/Inc
        ${STCUBE_BASE}/Drivers/BSP/P-NUCLEO-WB55.Nucleo
        ${STCUBE_BASE}/Drivers/Include
        ${STCUBE_BASE}/Drivers/CMSIS/Include
        )

# Compiler options
target_compile_options(${EXECUTABLE} PRIVATE
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard

        -fdata-sections
        -ffunction-sections

        -Wall
        -O0
        -g3
        )

# Linker options
target_link_options(${EXECUTABLE} PRIVATE
        -T${LINKER_FILE}
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
        -specs=nano.specs
        -lc
        -lm
        -lnosys
        -Wl,-Map=${PROJECT_NAME}.map,--cref
        -Wl,--gc-sections
        -Xlinker -print-memory-usage -Xlinker
        )

add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${CMAKE_SIZE_UTIL} ${EXECUTABLE})

# Optional: Create hex, bin and S-Record files after the build
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O srec --srec-len=64 ${EXECUTABLE} ${PROJECT_NAME}.s19
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)