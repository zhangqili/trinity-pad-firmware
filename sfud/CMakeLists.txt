project(sfud)

add_library(sfud 
port/sfud_port.c
src/sfud_sfdp.c
src/sfud.c
)

target_include_directories(sfud PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)