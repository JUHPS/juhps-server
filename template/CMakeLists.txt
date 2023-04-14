cmake_minimum_required(VERSION 3.0)
project(project_name)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/juphs-server)

include (juphs-server/cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined")

include_directories(.)
include_directories(juphs-server/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/juphs-server)
link_directories(juphs-server/lib)

find_package(Boost REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()


set(LIB_SRC
        template/my_module.cc
    )


add_library(project_name SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(project_name)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
