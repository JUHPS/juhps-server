cmake_minimum_required(VERSION 3.0)
project(WebServer)

add_definitions(-D_GLIBCXX_USE_CXX11_ABI=1)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O3 -rdynamic -fPIC -ggdb -std=c++14 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")

include_directories(.)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)
link_directories(/usr/local/lib64)


find_package(Boost REQUIRED)
find_package(yaml-cpp REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

set(LIB_SRC
	src/log.cc
	src/util.cc
	src/config.cc
	)

add_library(src SHARED ${LIB_SRC})
#add_library(WebServer_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES (WebServer_static PROPERTIES OUTPUT_NAME "WebServer")


set(LIBS
    src
    yaml-cpp
    )

add_executable(test tests/test.cc)
add_dependencies(test src)
target_link_libraries(test src "${LIBS}")

add_executable(test_config tests/test_config.cc)
add_dependencies(test_config src)
target_link_libraries(test_config src "${LIBS}")


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)