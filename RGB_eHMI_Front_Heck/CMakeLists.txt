cmake_minimum_required(VERSION 3.5)
project(asoa_template)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0054 NEW)
cmake_policy(SET CMP0042 NEW)
cmake_policy(SET CMP0079 NEW)
find_package(asoa_core 0.2.0 REQUIRED)

#RGB Library einbinden
set(RGBSERVICE "librgbmatrix.a")
find_library(RGBSERVICE librgbmatrix.a PATHS /lib)
link_libraries(${target} ${RGBSERVICE})
set(RGBSERVICE_INCLUDE_DIRS /include)
include_directories(${target} ${RGBSERVICE_INCLUDE_DIRS})
include_directories("include/")
link_directories("lib/")

# Append all flags to CMAKE_CXX_FLAGS
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include/GraphicsMagick -Wall -g -fno-strict-aliasing -O2 -pthread")
message("Cxx flags are: " ${CMAKE_CXX_FLAGS})

# Append all flags to CMAKE_EXE_LINKER_FLAGS
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/usr/lib -L/usr/lib/X11 -lGraphicsMagick++ -lGraphicsMagick -ljbig -lwebp -lwebpmux -llcms2 -ltiff -lfreetype -ljpeg -lpng16 -lwmflite -lXext -lSM -lICE -lX11 -llzma -lbz2 -lxml2 -lz -lm -lgomp -lpthread")
message("LD flags are: " ${CMAKE_EXE_LINKER_FLAGS})

file(GLOB EXECS "src/*.cpp")
foreach(src_ ${EXECS})
    get_filename_component(target ${src_} NAME)
    string(REPLACE ".cpp" "" target ${target})
    string(REPLACE "/" "_" target ${target})
    add_executable(${target} ${src_})

    #ASOA Core einbinden
    target_link_libraries(${target} asoa_core)

    #RGB Matrix Library Target
    target_link_libraries(${target} ${RGBSERVICE})

    #Magick Library target
    target_link_libraries(${target} -L/usr/lib -L/usr/lib/X11 -lGraphicsMagick++ -lGraphicsMagick -ljbig -lwebp -lwebpmux -llcms2 -ltiff -lfreetype -ljpeg -lpng16 -lwmflite -lXext -lSM -lICE -lX11 -llzma -lbz2 -lxml2 -lz -lm -lgomp -lpthread)

    message("-- Found target:")
    message("-- " ${target})
endforeach()
