# Basic Usage
NOTE: This has only been tested on linux

## Installing Hatronic Core
1. Clone repo: git clone ~link
1. Install GNU Arm tools and set the GNU_ARM environment variable to its bin directory
1. Export the Cmake package: cmake -P cmake/HatronicCoreExport.cmake


## Using HatronicCore in a Project

### Set Up Project
1. Setup CMakeLists.txt file in the root of your project
1. Add find_package(HatronicCore) before you project definition
1. Optionally add set(BOARD {YourBoard}) before find_package if you only plan on supporting a single board

### Using HatronicCore
1. HatronicCore exports a target called HatronicCore
1. Use this target to add any local resources to the target
1. Use cmake and build and flash the executable from your local project

### Building
Note: This assume that ninja will be used as the build tool.

To Build in Debug Mode:
```
cmake -S . -B build -D BOARD={BOARD} -GNinja -D CMAKE_BUILD_TYPE=Debug
cmake --build build
```

To Build in Release Mode:
```
cmake -S . -B build -D BOARD={BOARD} -GNinja -D CMAKE_BUILD_TYPE=Release
cmake --build build
```

### Flashing Command
Hatronic Core use pyocd to flash images to the target. Run the following from the main application to flash your device
```
cmake --build build -t flash
```

# Running Tests
To run the tests install ceedling then run the following:
```
cd test
ceedling test:all
```

# Dependencies

ARM GNU
https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads

Ninja
export PATH=~/dev/tools/ninja/ninja/build-cmake:"$PATH"
https://ninja-build.org/

Ceedling/Unity/CMock Throw the Switch
http://www.throwtheswitch.org/ceedling

STM32 Cube
STM32CubeWB: 
Included as a submodule. No Setup required.
https://github.com/STMicroelectronics/STM32CubeWB

PyOcd

# Important Settings

## General

BOARD: The board the library is being compiled for see supported boards to see which are supported
WARNINGS_AS_ERRORS: If defined all compiler warnings will be treated as errors
HAT_EXE_NAME: Set the base name of executable. Will be HatronicCore if not supplied

## ST

USE_ST_HAL: Define if you want to use the built in ST HAL
USE_EXTERNAL_CUBE: Define if you want to use a STM32Cube instance that is not part of the library

# Supported Boards
stm32wb55