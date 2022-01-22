# Setup PyOcd
```
pyocd pack --update
pyocd pack --install stm32wb55rg
```

target name: stm32wb55rgvx

# Settings
USE_EXTERNAL_CUBE: If defined cmake will not link built-in  STCubeWB instance. It must be supplied from the external project or the compilation will fail
USE_ST_HAL: If the STCubeHal should be linked in

# Compiler Options
```
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
```

# Linker Options
```
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard

    -specs=nano.specs
    -lc
    -lm
    -lnosys
    -Wl,--cref
    -Wl,--gc-sections
    -Xlinker -print-memory-usage -Xlinker
```