cmake_minimum_required(VERSION 3.20)

project(TDPEL C)

set(CMAKE_C_STANDARD 11)

find_package(SDL3 REQUIRED)

add_executable(TDPEL
    "Thousand Divine Palm Enrapturing Light TDPEL/TDPEL/TDPELmac.c"
)

target_include_directories(TDPEL PRIVATE include)

target_link_libraries(TDPEL PRIVATE SDL3::SDL3)

add_custom_command(TARGET TDPEL POST_BUILD
    COMMAND install_name_tool
        -change
        "/opt/homebrew/opt/sdl3/lib/libSDL3.0.dylib"
        "@loader_path/libSDL3.0.dylib"
        "$<TARGET_FILE:TDPEL>"
)
