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

Complie.

```bash
cmake -G"Ninja"
ninja
```

