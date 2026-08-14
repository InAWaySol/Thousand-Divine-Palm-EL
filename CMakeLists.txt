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

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Thousand Divine Palm Enrapturing Light TDPEL/TDPEL/audio"
        "$<TARGET_FILE_DIR:TDPEL>/audio"

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Thousand Divine Palm Enrapturing Light TDPEL/TDPEL/images"
        "$<TARGET_FILE_DIR:TDPEL>/images"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Thousand Divine Palm Enrapturing Light TDPEL/TDPEL/SimuData.txt"
        "$<TARGET_FILE_DIR:TDPEL>/SimuData.txt"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Thousand Divine Palm Enrapturing Light TDPEL/TDPEL/HandLog.txt"
        "$<TARGET_FILE_DIR:TDPEL>/HandLog.txt"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Thousand Divine Palm Enrapturing Light TDPEL/TDPEL/PriceLog.txt"
        "$<TARGET_FILE_DIR:TDPEL>/PriceLog.txt"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Thousand Divine Palm Enrapturing Light TDPEL/TDPEL/settings.txt"
        "$<TARGET_FILE_DIR:TDPEL>/settings.txt"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Thousand Divine Palm Enrapturing Light TDPEL/TDPEL/ArcTypeHANDs.txt"
        "$<TARGET_FILE_DIR:TDPEL>/ArcTypeHANDs.txt"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Thousand Divine Palm Enrapturing Light TDPEL/TDPEL/FreeSlot.txt"
        "$<TARGET_FILE_DIR:TDPEL>/FreeSlot.txt"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Thousand Divine Palm Enrapturing Light TDPEL/TDPEL/PerformenceLog.txt"
        "$<TARGET_FILE_DIR:TDPEL>/PerformenceLog.txt"
)
