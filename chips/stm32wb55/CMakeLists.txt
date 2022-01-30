message("Adding ST32WB55 References")

#################################################################
# Setup Core CPU Compiler Options
#################################################################

target_compile_definitions(${EXE_BASE} PUBLIC 
    -DSTM32WB55xx
)

target_compile_options(${EXE_BASE} PUBLIC 
    -mcpu=cortex-m4
    -mthumb

    # Setup floating-point support
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

# Linker options
target_link_options(${EXE_BASE} PUBLIC
    -T${CMAKE_CURRENT_SOURCE_DIR}/STM32WB55RGVX_FLASH.ld
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard

    -specs=nano.specs
    -lc
    -lm
    -lnosys
    -Wl,-Map=${EXE_BASE}.map,--cref
    -Wl,--gc-sections
    -Xlinker -print-memory-usage -Xlinker
    -specs=nosys.specs
)

target_sources(${EXE_BASE} PUBLIC startup_stm32wb55xx_cm4.s system_stm32wbxx.c)

#################################################################
# Setup STM32 CUBE
#################################################################

if (NOT DEFINED USE_EXTERNAL_CUBE)
    message("Using Hatronic Internal Cube")
    set(STM32CUBEWB_ROOT "${HAT_BASE}/libs/st/STM32CubeWB")
    set(STM32CUBEWB_DRIVER "${STM32CUBEWB_ROOT}/Drivers")
    set(STM32CUBEWB_CMSIS "${STM32CUBEWB_DRIVER}/CMSIS")
    set(STM32CUBEWB_DEVICE "${STM32CUBEWB_CMSIS}/Device/ST/STM32WBxx")

    target_include_directories(${EXE_BASE} PUBLIC
        ${STM32CUBEWB_CMSIS}/Include
        ${STM32CUBEWB_DEVICE}/Include)

    if(DEFINED USE_ST_HAL)
        message("Adding ST HAL References")
        target_compile_definitions(${EXE_BASE} PUBLIC 
            -DUSE_HAL_DRIVER
        )

        target_include_directories(${EXE_BASE} PUBLIC
            ${STM32CUBEWB_DRIVER}/STM32WBxx_HAL_Driver/Inc/Legacy
            ${STM32CUBEWB_DRIVER}/STM32WBxx_HAL_Driver/Inc
            ${STM32CUBEWB_DRIVER}/BSP/P-NUCLEO-WB55.Nucleo
            ${STM32CUBEWB_DRIVER}/Include
        )

        FILE(GLOB_RECURSE ST_HAL_SOURCES ${STM32CUBEWB_DRIVER}/STM32WBxx_HAL_Driver/*.c) 
        list(FILTER ST_HAL_SOURCES EXCLUDE REGEX template)
        target_sources(${EXE_BASE} PUBLIC ${ST_HAL_SOURCES})

        # TODO sources
    endif()
else()
    message("Using External Cube")
endif()

#################################################################
# Setup pyocd
#################################################################

set(PYOCD_TARGET stm32wb55rgvx PARENT_SCOPE)