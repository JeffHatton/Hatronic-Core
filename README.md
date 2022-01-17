### Building
1. Make build director
2. Generate cmake configuration
3. build

```
mkdir build
cd build
cmake -DCMAKE_MAKE_PROGRAM=ninja -G "Ninja" ..
cmake --build ..
```

### Flashing Command
~/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer.sh -c port=SWD -w ./STM32_WB55_Template.hex 


#Depenancies

ARM GNU
https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads

Ninja

export PATH=~/dev/tools/ninja/ninja/build-cmake:"$PATH"
https://ninja-build.org/

Ceedling/Unity/CMock Throw the Switch
http://www.throwtheswitch.org/ceedling