cmake_minimum_required(VERSION 3.15.3)

set(CMAKE_VERBOSE_MAKEFILE ON)

# turn on compile commands so VS Code can use the include paths
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

#TODO export version to header
set(HatronicCore_VERSION 0.0.1)
set(EXE_BASE HatronicCore)

get_filename_component(HAT_BASE "." ABSOLUTE)

#################################################################
# Gather chip and board information
#################################################################
if(NOT BOARD) 
    message(FATAL_ERROR "BOARD was not supplied")
else()
    message("BOARD is ${BOARD}")
endif()

set(ChipPath "${HAT_BASE}/chips/${BOARD}")
message("Path to Chip Resources: ${ChipPath}")
if (NOT EXISTS ${ChipPath})
    message(FATAL_ERROR "Chip ${BOARD} is not supported")
endif()

#################################################################
# Setup Toolchain and language settings
#################################################################

# Path to toolchain file. This one has to be before 'project()' below
set(CMAKE_TOOLCHAIN_FILE ${HAT_BASE}/chips/arm-none-eabi-gcc.cmake)
message("Path to tool chain: ${CMAKE_TOOLCHAIN_FILE}")

# Setup project
project(HatronicCore VERSION 0.0.1.0 DESCRIPTION "Core lib for Hatronic Projects")

# Setup language properties
enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

#################################################################
# Setup Core Executable
#################################################################

# Gather Source Files
set(SRC_FILES
    src/DataStructures/Fifo.c
    src/DataStructures/LinkedList.c
)

set(CMAKE_EXECUTABLE_SUFFIX .elf)

# Build the executable based on the source files
add_executable(${EXE_BASE} 
    ${SRC_FILES}
)

if(DEFINED HAT_EXE_NAME)
    set_target_properties(${EXE_BASE} PROPERTIES OUTPUT_NAME ${HAT_EXE_NAME})
endif()

# Setup header for project information
configure_file(cmake/HatronicCore.h.in inc/${EXE_BASE}.h)

#################################################################
# Gather dependancies
#################################################################

# Include Chip Level Setup
add_subdirectory(${ChipPath})

#################################################################
# Setup Include/Options/Defines
#################################################################

target_include_directories(${EXE_BASE} PRIVATE
    src
    src/DataStructures
    ${CMAKE_CURRENT_BINARY_DIR}/inc
)

#TODO figure out a better way of handling this
# This is a work around for test intellisense
target_compile_definitions(${EXE_BASE} PRIVATE
    -DTEST
)

#TODO better way of doing this
if(${WARNINGS_AS_ERRORS})
    set(WARNING_VALUE "-Werror")
else()
    set(WARNING_VALUE "-Wall")
endif()

# Compiler options
target_compile_options(${EXE_BASE} PRIVATE
    -fdata-sections
    -ffunction-sections

    #Set Warnings
    ${WARNING_VALUE}

    # Add debug generation
    $<$<CONFIG:Debug>:-g3>
    $<$<CONFIG:RelWithDebInfo>:--g3>        
    
    # Set optimiztion
    $<$<CONFIG:Debug>:-O1>
    $<$<CONFIG:Release,RelWithDebInfo>:-Omax3>
    $<$<CONFIG:MinSizeRel>:-Oz>
)

# Linker options
target_link_options(${EXE_BASE} PRIVATE
    -specs=nano.specs
    -lc
    -lm
    -lnosys
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -Wl,--gc-sections
    -Xlinker -print-memory-usage -Xlinker
)

#################################################################
# Post Build Commands
#################################################################

# Print out size of library
add_custom_command(TARGET ${EXE_BASE}
    POST_BUILD
    COMMAND ${CMAKE_SIZE_UTIL} ${EXE_BASE}${CMAKE_EXECUTABLE_SUFFIX}
)

# Create hex and bin files
add_custom_command(TARGET ${EXE_BASE}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${EXE_BASE}${CMAKE_EXECUTABLE_SUFFIX} ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary ${EXE_BASE}${CMAKE_EXECUTABLE_SUFFIX} ${PROJECT_NAME}.bin
)

#################################################################
# Custom Commands
#################################################################

# Add command to flash target
add_custom_target(flash 
    COMMAND pyocd load ${CMAKE_CURRENT_BINARY_DIR}/HatronicCore.elf --target=${PYOCD_TARGET} 
)
