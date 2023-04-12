cmake_minimum_required(VERSION 3.0)
project(JUHPS)

include (cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)

# 指定编译选项
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -std=c++11 -O0 -ggdb -Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated -Wno-deprecated-declarations")

include_directories(.)

option(BUILD_TEST "ON for complile test" ON)

find_package(Boost REQUIRED) 
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

set(LIB_SRC
    src/log.cc
    src/util.cc
    src/mutex.cc
    src/env.cc
    src/config.cc
    src/thread.cc
    src/fiber.cc
    src/scheduler.cc
    src/iomanager.cc
    src/timer.cc
    src/fd_manager.cc
    src/hook.cc
    src/address.cc 
    src/socket.cc 
    src/bytearray.cc 
    src/tcp_server.cc 
    src/http/http-parser/http_parser.c 
    src/http/http.cc
    src/http/http_parser.cc 
    src/stream.cc 
    src/streams/socket_stream.cc
    src/http/http_session.cc 
    src/http/servlet.cc
    src/http/http_server.cc 
    src/uri.cc 
    src/http/http_connection.cc 
    src/daemon.cc 
    )

add_library(src SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(src)

set(LIBS
    src
    pthread
    dl
    yaml-cpp
)

if(BUILD_TEST)
jujimeizuo_add_executable(test_log "tests/test_log.cpp" src "${LIBS}")
jujimeizuo_add_executable(test_util "tests/test_util.cpp" src "${LIBS}")
jujimeizuo_add_executable(test_env "tests/test_env.cc" src "${LIBS}")
jujimeizuo_add_executable(test_config "tests/test_config.cc" src "${LIBS}")
jujimeizuo_add_executable(test_thread "tests/test_thread.cc" src "${LIBS}")
jujimeizuo_add_executable(test_fiber "tests/test_fiber.cc" src "${LIBS}")
jujimeizuo_add_executable(test_fiber2 "tests/test_fiber2.cc" src "${LIBS}")
jujimeizuo_add_executable(test_scheduler "tests/test_scheduler.cc" src "${LIBS}")
jujimeizuo_add_executable(test_iomanager "tests/test_iomanager.cc" src "${LIBS}")
jujimeizuo_add_executable(test_timer "tests/test_timer.cc" src "${LIBS}")
jujimeizuo_add_executable(test_hook "tests/test_hook.cc" src "${LIBS}")
jujimeizuo_add_executable(test_address "tests/test_address.cc" src "${LIBS}")
jujimeizuo_add_executable(test_socket_tcp_server "tests/test_socket_tcp_server.cc" src "${LIBS}")
jujimeizuo_add_executable(test_socket_tcp_client "tests/test_socket_tcp_client.cc" src "${LIBS}")
jujimeizuo_add_executable(test_bytearray "tests/test_bytearray.cc" src "${LIBS}")
jujimeizuo_add_executable(test_tcp_server "tests/test_tcp_server.cc" src "${LIBS}")
jujimeizuo_add_executable(test_http "tests/test_http.cc" src "${LIBS}")
jujimeizuo_add_executable(test_http_parser "tests/test_http_parser.cc" src "${LIBS}")
jujimeizuo_add_executable(test_http_server "tests/test_http_server.cc" src "${LIBS}")
jujimeizuo_add_executable(test_uri "tests/test_uri.cc" src "${LIBS}")
jujimeizuo_add_executable(test_http_connection "tests/test_http_connection.cc" src "${LIBS}")
jujimeizuo_add_executable(test_daemon "tests/test_daemon.cc" src "${LIBS}")
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)