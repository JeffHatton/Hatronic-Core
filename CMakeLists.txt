cmake_minimum_required(VERSION 3.15.3)

# set(CMAKE_VERBOSE_MAKEFILE ON)

# turn on compile commands so VS Code can use the include paths
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Path to toolchain file. This one has to be before 'project()' below
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/boards/arm-none-eabi-gcc.cmake)

set(LINKER_FILE ${CMAKE_SOURCE_DIR}/boards/stm32wb55/STM32WB55RGVX_FLASH.ld)

# Setup project, output and linker file
project(Hatronic_Core VERSION 0.1.0 DESCRIPTION "Core lib for Hatronic Projects")
set(EXECUTABLE ${PROJECT_NAME}.elf)

# Setup language properties
enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(LIB_NAME Hatronic_Core_Lib)

set(LIB_BASE ${CMAKE_SOURCE_DIR}/lib)

# Gather Source Files
set(SRC_FILES
        ${LIB_BASE}/src/Fifo.c
        )

# Build the executable based on the source files
add_library(${LIB_NAME}
        ${SRC_FILES}
)

set_target_properties(${LIB_NAME} PROPERTIES SOVERSION 1)
set_target_properties(${LIB_NAME} PROPERTIES VERSION ${PROJECT_VERSION})        

# List of include directories
target_include_directories(${LIB_NAME} PRIVATE
        ${LIB_BASE}/src
        )

# TODO this is a work around for test intellisense
target_compile_definitions(${LIB_NAME} PRIVATE
        -DTEST
       )

# Compiler options
target_compile_options(${LIB_NAME} PRIVATE
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

install(TARGETS ${LIB_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/${LIB_NAME}.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)