cmake_minimum_required(VERSION 3.0)
project(WebServer)

include (cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)

# 指定编译选项
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -std=c++11 -O0 -ggdb -Wall -Werror")

# -rdynamic: 将所有符号都加入到符号表中，便于使用dlopen或者backtrace追踪到符号
# -fPIC: 生成位置无关的代码，便于动态链接
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic -fPIC")

# -Wno-unused-function: 不要警告未使用函数
# -Wno-builtin-macro-redefined: 不要警告内置宏重定义，用于重定义内置的__FILE__宏
# -Wno-deprecated: 不要警告过时的特性
# -Wno-deprecated-declarations: 不要警告使用带deprecated属性的变量，类型，函数
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated -Wno-deprecated-declarations")

include_directories(.)

option(BUILD_TEST "ON for complile test" OFF)

find_package(Boost REQUIRED) 
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

set(LIB_SRC
    jujimeizuo/log.cpp
    jujimeizuo/util.cpp
    jujimeizuo/mutex.cc
    jujimeizuo/env.cc
    jujimeizuo/config.cc
    jujimeizuo/thread.cc
    jujimeizuo/fiber.cc
    jujimeizuo/scheduler.cc
    jujimeizuo/iomanager.cc
    jujimeizuo/timer.cc
    jujimeizuo/fd_manager.cc
    jujimeizuo/hook.cc
    jujimeizuo/address.cc 
    jujimeizuo/socket.cc 
    jujimeizuo/bytearray.cc 
    jujimeizuo/tcp_server.cc 
    jujimeizuo/http/http-parser/http_parser.c 
    jujimeizuo/http/http.cc
    jujimeizuo/http/http_parser.cc 
    jujimeizuo/stream.cc 
    jujimeizuo/streams/socket_stream.cc
    jujimeizuo/http/http_session.cc 
    jujimeizuo/http/servlet.cc
    jujimeizuo/http/http_server.cc 
    )

add_library(jujimeizuo SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(jujimeizuo)

set(LIBS
    jujimeizuo
    pthread
    dl
    yaml-cpp
)

if(BUILD_TEST)
jujimeizuo_add_executable(test_log "tests/test_log.cpp" jujimeizuo "${LIBS}")
jujimeizuo_add_executable(test_util "tests/test_util.cpp" jujimeizuo "${LIBS}")
jujimeizuo_add_executable(test_env "tests/test_env.cc" jujimeizuo "${LIBS}")
jujimeizuo_add_executable(test_config "tests/test_config.cc" jujimeizuo "${LIBS}")
jujimeizuo_add_executable(test_thread "tests/test_thread.cc" jujimeizuo "${LIBS}")
jujimeizuo_add_executable(test_fiber "tests/test_fiber.cc" jujimeizuo "${LIBS}")
jujimeizuo_add_executable(test_fiber2 "tests/test_fiber2.cc" jujimeizuo "${LIBS}")
jujimeizuo_add_executable(test_scheduler "tests/test_scheduler.cc" jujimeizuo "${LIBS}")
jujimeizuo_add_executable(test_iomanager "tests/test_iomanager.cc" jujimeizuo "${LIBS}")
jujimeizuo_add_executable(test_timer "tests/test_timer.cc" jujimeizuo "${LIBS}")
jujimeizuo_add_executable(test_hook "tests/test_hook.cc" jujimeizuo "${LIBS}")
jujimeizuo_add_executable(test_address "tests/test_address.cc" jujimeizuo "${LIBS}")
jujimeizuo_add_executable(test_socket_tcp_server "tests/test_socket_tcp_server.cc" jujimeizuo "${LIBS}")
jujimeizuo_add_executable(test_socket_tcp_client "tests/test_socket_tcp_client.cc" jujimeizuo "${LIBS}")
jujimeizuo_add_executable(test_bytearray "tests/test_bytearray.cc" jujimeizuo "${LIBS}")
jujimeizuo_add_executable(test_tcp_server "tests/test_tcp_server.cc" jujimeizuo "${LIBS}")
jujimeizuo_add_executable(test_http "tests/test_http.cc" jujimeizuo "${LIBS}")
jujimeizuo_add_executable(test_http_parser "tests/test_http_parser.cc" jujimeizuo "${LIBS}")
jujimeizuo_add_executable(test_http_server "tests/test_http_server.cc" jujimeizuo "${LIBS}")
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)