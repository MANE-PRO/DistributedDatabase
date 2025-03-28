cmake_minimum_required(VERSION 3.20)
project(backend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
find_library(MYSQLCLIENT mysqlclient REQUIRED)

find_package(Protobuf CONFIG REQUIRED)
#include(FetchContent)
#FetchContent_Declare(
#  gRPC
#  GIT_REPOSITORY https://github.com/grpc/grpc
#  GIT_TAG        v1.28.0
#)
#set(FETCHCONTENT_QUIET OFF)
#FetchContent_MakeAvailable(gRPC)
#add_dependencies(protolib gRPC::grpc_cpp_plugin)
find_package(gRPC CONFIG REQUIRED)
add_library(protolib proto/database_service.proto)
target_link_libraries(protolib gRPC::grpc++)
target_link_libraries(protolib gRPC::grpc++ protobuf::libprotobuf)
target_include_directories(protolib PUBLIC)
target_include_directories(protolib PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/proto")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/proto")
protobuf_generate(TARGET protolib LANGUAGE cpp PROTOC_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/proto" IMPORT_DIRS proto)
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET protolib LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}" PROTOC_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/proto" IMPORT_DIRS proto)

 
add_executable(server src/server.cc)
target_link_libraries(server ${MYSQLCLIENT} pthread protolib)
