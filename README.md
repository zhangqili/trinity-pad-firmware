# TrinityPad Firmware

Configurator: <https://github.com/zhangqili/EMIKeyboardConfigurator>

## Prerequisites
- Git
- CMake
- Ninja
- Download compiler from <http://mounriver.com/download>

## Build

Download source.

```bash
git clone --recurse-submodules https://github.com/zhangqili/trinity-pad-firmware.git
cd trinity-pad-firmware 
```

Set your compiler path in `CMakeLists.txt`

```cmake
set(TOOLPATH  "C:/MounRiver/MounRiver_Studio/toolchain/RISC-V Embedded GCC/bin/riscv-none-embed-")
```

Compile.

```bash
mkdir build
cd build
cmake .. -G"Ninja"
ninja
```

