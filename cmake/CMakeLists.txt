# #####################
cmake_minimum_required(VERSION 3.8)
project(clion)

set(CMAKE_CONFIGURATION_TYPES "Debug;Test;Release" CACHE STRING "" FORCE)
set(CMAKE_CXX_STANDARD 17)

# add options to configure project
option(LOG_ENABLE_LOGFCPP "Enable logging via log4cpp. Only one may be selected" ON)


IF(LOG_ENABLE_LOGFCPP)
    include(${CMAKE_CURRENT_SOURCE_DIR}/../build_deps/log4cpp/CMakeLists.txt)
    add_definitions(-DLOG4CPP_HAVE_INT64_T)
    add_definitions(-DNPCT_LOG_ENABLE_LOG4CPP)
ENDIF(LOG_ENABLE_LOGFCPP)

# add necessary options
add_definitions(-DASIO_STANDALONE)
IF(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0600)
ENDIF(WIN32)
add_definitions(-DASIO_DISABLE_SMALL_BLOCK_RECYCLING)
add_definitions(-DNOMINMAX)

set(CMAKE_VERBOSE_MAKEFILE ON)

# add library directory
link_directories(../lib)
# add include directories
include_directories(../npct)
include_directories(../include)
# add sources to compile from external libraries
file(GLOB_RECURSE SOURCES_INCLUDES
     "../include/*.cc"
     "../include/*.cpp"
     "../include/*.c"
)
# remove all files, which are not required to compile
list(REMOVE_ITEM SOURCES_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/../include/asio/impl/src.cpp")
FOREACH(item ${SOURCES_INCLUDES})
    IF(${item} MATCHES "^${CMAKE_CURRENT_SOURCE_DIR}/../include/log4cpp/")
        LIST(REMOVE_ITEM SOURCES_INCLUDES ${item})
    ENDIF(${item} MATCHES "^${CMAKE_CURRENT_SOURCE_DIR}/../include/log4cpp/")
ENDFOREACH(item)
# print all files which will be compiled from external libraries
message("${SOURCES_INCLUDES}")

# [utils] add utils files to compile
file(GLOB_RECURSE SOURCES_UTILS
        "../npct/utils/*.cc"
)

# [core] add core files to compile
file(GLOB_RECURSE SOURCES_NPCT
		# CMD subdir
		"../npct/cmd/*.cc"
		# DATA subdir
		"../npct/data/*.cc"
		"../npct/coders/*.cc"
		"../npct/net/*.cc"
		"../npct/log/*.cc"
)

# [test] add test files to compile
file(GLOB SOURCES_TEST
        "../tests/catch.hpp"
)

# if test build - link main() from tests
if(CMAKE_BUILD_TYPE MATCHES Test)
    file(GLOB MAIN_NPCT ${SOURCES_TEST} "../tests/test_main.cc")
else()
    file(GLOB MAIN_NPCT "../example_main.cc")
endif(CMAKE_BUILD_TYPE MATCHES Test)





# EXPERIMENTAL: add all include files to project dependencies
file(GLOB_RECURSE HEADERS_NPCT
        "../npct/*.h"
        "../npct/*.tcc"

        )

# compile all into one object-file and compile executable
add_library(npct_lib OBJECT ${SOURCES_NPCT} ${SOURCES_INCLUDES} ${SOURCES_UTILS} ${HEADERS_NPCT} )
add_executable(npct ${MAIN_NPCT} $<TARGET_OBJECTS:npct_lib>)

# if release version - link all libs statically
message(STATUS "Copying release dir...")
if(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static" )
    message(STATUS "Copying release dir...")
    add_custom_command(TARGET npct POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/../configs" $<TARGET_FILE_DIR:npct>/configs)
    add_custom_command(TARGET npct POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/../tests/client" $<TARGET_FILE_DIR:npct>/tests)
endif(CMAKE_BUILD_TYPE MATCHES Release)

# also - link necessary libaries
IF(LOG_ENABLE_LOGFCPP)
    add_dependencies(npct ${LOG4CPP_LIBRARY_NAME})
    target_link_libraries(npct ${LOG4CPP_LIBRARY_NAME})
ENDIF(LOG_ENABLE_LOGFCPP)

# add networking for win32
IF (WIN32)
    target_link_libraries(npct wsock32 ws2_32)
ENDIF()