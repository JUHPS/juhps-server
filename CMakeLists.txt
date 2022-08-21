cmake_minimum_required(VERSION 3.0)
project(WebServer)

set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")

set(LIB_SRC
	WebServer/log.cc
	)

add_library(WebServer SHARED ${LIB_SRC})
#add_library(WebServer_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES (WebServer_static PROPERTIES OUTPUT_NAME "WebServer")

add_executable(test tests/test.cc)
add_dependencies(test WebServer)
target_link_libraries(test WebServer)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)