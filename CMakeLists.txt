cmake_minimum_required(VERSION 3.0)
project(WebServer)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O3 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")


set(LIB_SRC
	src/log.cc
	src/util.cc
	)

add_library(src SHARED ${LIB_SRC})
#add_library(WebServer_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES (WebServer_static PROPERTIES OUTPUT_NAME "WebServer")

add_executable(test tests/test.cc)
add_dependencies(test src)
target_link_libraries(test src)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)