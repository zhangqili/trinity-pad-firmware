# TrinityPad Firmware

## Prerequisites
- CMake
- Ninja
- Download compiler from <http://mounriver.com/download>

## Build

Set your compiler path in `CMakeLists.txt`

```cmake
set(TOOLPATH  "C:/MounRiver/MounRiver_Studio/toolchain/RISC-V Embedded GCC/bin/riscv-none-embed-")
```

Compile.

```bash
git submodule init
git submodule update
cmake -G"Ninja"
ninja
```

