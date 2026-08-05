cmake_minimum_required(VERSION 3.20)

project(TDPEL C)

set(CMAKE_C_STANDARD 11)

find_package(SDL3 REQUIRED)

add_executable(TDPEL
    Thousand Divine Palm Enrapturing Light/TDPEL/TDPELmac.c
)

target_include_directories(TDPEL PRIVATE include)

target_link_libraries(TDPEL PRIVATE SDL3::SDL3)
