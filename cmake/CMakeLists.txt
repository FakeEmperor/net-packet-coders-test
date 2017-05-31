# #####################
cmake_minimum_required(VERSION 3.6)
project(clion)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DASIO_STANDALONE)
add_definitions(-D_WIN32_WINNT=0x0600)
add_definitions(-DASIO_DISABLE_SMALL_BLOCK_RECYCLING)
add_definitions(-DNOMINMAX)

set(CMAKE_VERBOSE_MAKEFILE ON)





include_directories(../npct)
include_directories(../include)

file(GLOB_RECURSE SOURCES_INCLUDES
     "../include/*.cc"
     "../include/*.cpp"
     "../include/*.c"
)

message("${SOURCES_INCLUDES}")

list(REMOVE_ITEM SOURCES_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/../include/asio/impl/src.cpp")

file(GLOB_RECURSE SOURCES_UTILS
        "../npct/utils/string.cc"
)

file(GLOB_RECURSE SOURCES_NPCT
		# CMD subdir
		"../npct/cmd/*.cc"
		# DATA subdir
		"../npct/data/*.cc"
		"../npct/coders/*.cc"
		"../npct/net/*.cc"
)

file(GLOB SOURCES_TEST
        "../tests/catch.hpp"
)





if(CMAKE_BUILD_TYPE MATCHES Test)
    file(GLOB MAIN_NPCT ${SOURCES_TEST} "../tests/test_main.cc")
else()
    file(GLOB MAIN_NPCT "../example_main.cc")
endif(CMAKE_BUILD_TYPE MATCHES Test)


add_library(npct_lib OBJECT ${SOURCES_NPCT} ${SOURCES_INCLUDES} ${SOURCES_UTILS} )
add_executable(npct ${MAIN_NPCT} $<TARGET_OBJECTS:npct_lib>)


IF (WIN32)
    target_link_libraries(npct wsock32 ws2_32)
ENDIF()
