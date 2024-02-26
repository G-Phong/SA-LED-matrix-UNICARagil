cmake_minimum_required(VERSION 3.5)
project(asoa_template)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0054 NEW)
cmake_policy(SET CMP0042 NEW)
cmake_policy(SET CMP0079 NEW)
find_package(asoa_core 0.2.0 REQUIRED)

include_directories("include/")
link_directories("lib/")


file(GLOB EXECS "src/*.cpp")
foreach(src_ ${EXECS})
    get_filename_component(target ${src_} NAME)
    string(REPLACE ".cpp" "" target ${target})
    string(REPLACE "/" "_" target ${target})
    add_executable(${target} ${src_})

    #ASOA Core einbinden
    target_link_libraries(${target} asoa_core)

    message("-- Found target:")
    message("-- " ${target})
endforeach()
