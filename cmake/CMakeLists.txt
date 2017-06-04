# #####################
cmake_minimum_required(VERSION 3.6)
project(clion)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DASIO_STANDALONE)
add_definitions(-D_WIN32_WINNT=0x0600)
add_definitions(-DASIO_DISABLE_SMALL_BLOCK_RECYCLING)
add_definitions(-DNOMINMAX)

set(CMAKE_VERBOSE_MAKEFILE ON)


link_directories(../lib)


include_directories(../npct)
include_directories(../include)

file(GLOB_RECURSE SOURCES_INCLUDES
     "../include/*.cc"
     "../include/*.cpp"
     "../include/*.c"
)

list(REMOVE_ITEM SOURCES_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/../include/asio/impl/src.cpp")
FOREACH(item ${SOURCES_INCLUDES})
  IF(${item} MATCHES "^${CMAKE_CURRENT_SOURCE_DIR}/../include/log4cpp/")
    LIST(REMOVE_ITEM SOURCES_INCLUDES ${item})
  ENDIF(${item} MATCHES "^${CMAKE_CURRENT_SOURCE_DIR}/../include/log4cpp/")
ENDFOREACH(item)

message("${SOURCES_INCLUDES}")


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
		"../npct/log/*.cc"
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

target_link_libraries(npct log4cpp)
IF (WIN32)
    target_link_libraries(npct wsock32 ws2_32)
ENDIF()
