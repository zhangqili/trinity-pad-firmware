project(u8g2-extension)

aux_source_directory(./src files)
add_library(u8g2-extension ${files})

target_include_directories(u8g2-extension PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_link_libraries(u8g2-extension u8g2)