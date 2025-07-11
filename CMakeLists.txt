set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_COLOR_DIAGNOSTICS ON)
cmake_minimum_required(VERSION 3.18)

# 工具链设置
set(TOOLPATH  "C:/MounRiver/MounRiver_Studio2/resources/app/resources/win32/components/WCH/Toolchain/RISC-V Embedded GCC12/bin/riscv-wch-elf-")

if (WIN32)
	MESSAGE(STATUS "Now is windows!")
    set(CMAKE_C_COMPILER ${TOOLPATH}gcc.exe)
    set(CMAKE_CXX_COMPILER ${TOOLPATH}g++.exe)
    set(CMAKE_ASM_COMPILER ${TOOLPATH}gcc.exe)
    set(CMAKE_AR ${TOOLPATH}ar.exe)
    set(CMAKE_OBJCOPY ${TOOLPATH}objcopy.exe)
    set(CMAKE_OBJDUMP ${TOOLPATH}objdump.exe)
    set(SIZE ${TOOLPATH}size.exe)
elseif (UNIX)
	MESSAGE(STATUS "Now is UNIX-like OS!")
    set(CMAKE_C_COMPILER ${TOOLPATH}gcc)
    set(CMAKE_CXX_COMPILER ${TOOLPATH}g++)
    set(CMAKE_ASM_COMPILER ${TOOLPATH}gcc)
    set(CMAKE_AR ${TOOLPATH}ar)
    set(CMAKE_OBJCOPY ${TOOLPATH}objcopy)
    set(CMAKE_OBJDUMP ${TOOLPATH}objdump)
    set(SIZE ${TOOLPATH}size)
else ()
    MESSAGE(STATUS "Unsupported system!")
endif ()

# 项目设置
project(trinity-pad-firmware C CXX ASM)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

# 编译参数 一般不用改
add_compile_options(-march=rv32imafcxw -mabi=ilp32f -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -u _printf_float)
add_compile_options(-fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -fsingle-precision-constant)

# 编译等级
add_compile_options(-Ofast)

# 编译信息等级
add_compile_options(-Wall)

# 头文件路径
include_directories(
    User
    Core
    Debug 
    Peripheral/inc
    fezui/ui
    fezui/src
    libamp_user
    lefl
    usb
    fram
    snake
    CherryUSB/core
    CherryUSB/common
    CherryUSB/class/hid
    CherryUSB/class/midi
    CherryUSB/port/ch32
)
                    

# 宏定义
# add_definitions(-DDEBUG=1)
add_compile_definitions(U8G2_USE_LARGE_FONTS)

# 源码文件
file(GLOB_RECURSE SOURCES 
    "User/*.c"
    "Core/core_riscv.c" 
    "Debug/debug.c"
    "libamp_user/*.c" 
    "lefl/*.c" 
    "fezui/ui/*.c" 
    "fezui/src/*.c" 
    "fezui/dialog/*.c" 
    "Peripheral/src/*.c" 
    "fram/*.c" 
    "Startup/startup_ch32v30x_D8C.S"
    "usb/*.c"
    "snake/*.c"
)

file(GLOB_RECURSE SOURCES ${SOURCES} 
    "CherryUSB/port/ch32/usb_dc_usbhs.c"
    "./CherryUSB/core/usbd_core.c"
    "./CherryUSB/class/hid/usbd_hid.c"
)

add_compile_definitions(CONFIG_USB_HS)

# 链接参数
set(LINKER_SCRIPT  ${CMAKE_SOURCE_DIR}/Ld/Link.ld)
add_link_options(
                -march=rv32imafcxw -mabi=ilp32f
	            -nostartfiles 
                -Xlinker --gc-sections  
                -Wl,--print-memory-usage
                -Wl,-Map,${PROJECT_NAME}.map 
                --specs=nano.specs 
                --specs=nosys.specs
                -u _printf_float
                )
add_link_options(-T ${LINKER_SCRIPT})
#link_libraries(m) 
#add_link_options(-lprintfloat)

# 编译可执行文件
add_subdirectory(u8g2)
add_subdirectory(sfud)
add_subdirectory(libamp)
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

# 链接静态库
target_link_libraries(${PROJECT_NAME}.elf u8g2)
target_link_libraries(${PROJECT_NAME}.elf sfud)
target_link_libraries(${PROJECT_NAME}.elf libamp)
target_link_libraries(${PROJECT_NAME}.elf m)

# 输出hex和bin
set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)
set(LST_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.lst)
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMAND ${CMAKE_OBJDUMP} --all-headers --demangle --disassemble $<TARGET_FILE:${PROJECT_NAME}.elf> > ${LST_FILE}
        COMMAND ${SIZE} --format=berkeley $<TARGET_FILE:${PROJECT_NAME}.elf>
)
