cmake_minimum_required(VERSION 3.10)
project(SAT_DPLL LANGUAGES CXX)

file(GLOB SAT_DPLL_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_executable(sat_dpll_app ${SAT_DPLL_SOURCES})

target_include_directories(sat_dpll_app
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(sat_dpll_app PRIVATE Allocator)

include(/home/kali/Desktop/pvs-studio-7.35.89650.344-x86_64/lib/pvs-studio/plugins/pvs-studio-cmake-module/PVS-Studio.cmake)

pvs_studio_add_target(TARGET sat_dpll.analyze ALL
                      OUTPUT FORMAT json
                      ANALYZE sat_dpll_app
                      LOG sat_dpll.err)
