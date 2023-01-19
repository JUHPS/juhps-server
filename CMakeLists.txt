cmake_minimum_required(VERSION 3.0)
project(server)

include(cmake/utils.cmake)

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
    src/thread.cc
    )

add_library(src SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(src) #__FILE__
#add_library(WebServer_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES (WebServer_static PROPERTIES OUTPUT_NAME "WebServer")


set(LIBS
    src
    yaml-cpp
    pthread
    )

message("*****", ${LIBS})

add_executable(test_logger tests/test_logger.cc)
add_dependencies(test_logger src)
force_redefine_file_macro_for_sources(test_logger) #__FILE__
target_link_libraries(test_logger src "${LIBS}")

add_executable(test_config tests/test_config.cc)
add_dependencies(test_config src)
force_redefine_file_macro_for_sources(test_config) #__FILE__
target_link_libraries(test_config src "${LIBS}")

add_executable(test_thread tests/test_thread.cc)
add_dependencies(test_thread src)
force_redefine_file_macro_for_sources(test_thread) #__FILE__
target_link_libraries(test_thread src "${LIBS}")

add_executable(test_util tests/test_util.cc)
add_dependencies(test_util src)
force_redefine_file_macro_for_sources(test_util) #__FILE__
target_link_libraries(test_util src "${LIBS}")


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)