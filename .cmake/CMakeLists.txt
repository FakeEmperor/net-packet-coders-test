# #####################
cmake_minimum_required(VERSION 3.8)
project(clion)

set(CMAKE_CXX_STANDARD 14)

include_directories(../npct)
include_directories(../include)

file(GLOB SOURCES_UTILS
        "../npct/utils/*.h"
        "../npct/utils/*.cc"
)

file(GLOB SOURCES_NPCT
		# CMD subdir
		"../npct/cmd/*.h"
		"../npct/cmd/*.cc"
		"../npct/cmd/impl/*.h"
		"../npct/cmd/impl/*.cc"
		# DATA subdir
		"../npct/data/*.h"
		"../npct/data/*.cc"
		"../npct/data/impl/*.h"
		"../npct/data/impl/*.cc"
		# UTILS subdir
		"../npct/utils/*.h"
		"../npct/utils/*.cc"

		# first level
		"../npct/*/*.h"
        "../npct/*/*.cc"
		# second level
        "../npct/*/*/*.h"
        "../npct/*/*/*.cc"
        # third level
		"../npct/*/*/*/*.h"
        "../npct/*/*/*/*.cc"
)

file(GLOB SOURCES_TEST
        "../tests/catch.hpp"
)

if(CMAKE_BUILD_TYPE MATCHES Test)
    file(GLOB MAIN_NPCT ${SOURCES_TEST} "../tests/test_main.cc")
else()
    file(GLOB MAIN_NPCT "../example_main.cc")
endif(CMAKE_BUILD_TYPE MATCHES Test)

add_executable(npct ${SOURCES_UTILS} ${SOURCES_NPCT}
        ${MAIN_NPCT})


