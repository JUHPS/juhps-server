cmake_minimum_required(VERSION 3.0)
project(server)

include(cmake/utils.cmake)

add_definitions(-D_GLIBCXX_USE_CXX11_ABI=1)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O3 -rdynamic -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated  -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations -ldl")
set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations -ldl")

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
    src/address.cc
    src/bytearray.cc
    src/config.cc
    src/env.cc
    src/daemon.cc
    src/fd_manager.cc
    src/fiber.cc
    src/http/http.cc
    src/http/http_connection.cc
    src/http/http_parser.cc
    src/http/http_session.cc
    src/http/http_server.cc
    src/http/servlet.cc
    src/http/servlets/config_servlet.cc
    src/http/servlets/status_servlet.cc
    src/http/session_data.cc
    src/http/ws_connection.cc
    src/http/ws_session.cc
    src/http/ws_server.cc
    src/http/ws_servlet.cc
    src/hook.cc
    src/iomanager.cc
    src/library.cc
	src/log.cc
    src/module.cc
    src/mutex.cc
    src/protocol.cc
    src/rock/rock_protocol.cc
    src/rock/rock_server.cc
    src/rock/rock_stream.cc
    src/scheduler.cc
    src/socket.cc
    src/stream.cc
    src/streams/async_socket_stream.cc
    src/streams/socket_stream.cc
    src/streams/load_balance.cc
    src/streams/service_discovery.cc
    src/streams/zlib_stream.cc
    src/tcp_server.cc
    src/timer.cc
    src/thread.cc
	src/util.cc
    src/util/crypto_util.cc
    src/util/json_util.cc
    src/util/hash_util.cc
    src/worker.cc
    src/application.cc
    src/zk_client.cc
    )

add_library(src SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(src) #__FILE__
#add_library(WebServer_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES (WebServer_static PROPERTIES OUTPUT_NAME "WebServer")


set(LIBS
    src
    yaml-cpp
    pthread
    jsoncpp
    ${ZLIB_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    ${PROTOBUF_LIBRARIES}
    zookeeper_mt
    )


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

add_executable(test_fiber tests/test_fiber.cc)
add_dependencies(test_fiber src)
force_redefine_file_macro_for_sources(test_fiber) #__FILE__
target_link_libraries(test_fiber src "${LIBS}")

add_executable(test_scheduler tests/test_scheduler.cc)
add_dependencies(test_scheduler src)
force_redefine_file_macro_for_sources(test_scheduler) #__FILE__
target_link_libraries(test_scheduler src "${LIBS}")

add_executable(test_iomanager tests/test_iomanager.cc)
add_dependencies(test_iomanager src)
force_redefine_file_macro_for_sources(test_iomanager) #__FILE__
target_link_libraries(test_iomanager src "${LIBS}")

add_executable(test_hook tests/test_hook.cc)
add_dependencies(test_hook src)
force_redefine_file_macro_for_sources(test_hook) #__FILE__
target_link_libraries(test_hook src "${LIBS}")

add_executable(test_address tests/test_address.cc)
add_dependencies(test_address src)
force_redefine_file_macro_for_sources(test_address) #__FILE__
target_link_libraries(test_address src "${LIBS}")

add_executable(test_socket tests/test_socket.cc)
add_dependencies(test_socket src)
force_redefine_file_macro_for_sources(test_socket) #__FILE__
target_link_libraries(test_socket src "${LIBS}")

add_executable(test_bytearray tests/test_bytearray.cc)
add_dependencies(test_bytearray src)
force_redefine_file_macro_for_sources(test_bytearray) #__FILE__
target_link_libraries(test_bytearray src "${LIBS}") 

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
