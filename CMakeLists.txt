
cmake_minimum_required (VERSION 3.20)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()


project(net LANGUAGES CXX)  # 明确指定使用C++语言



set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# 查找所有.hpp文件
file(GLOB ALL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB ALL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Common 
file(GLOB_RECURSE COMMON_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/common/*.h)
file(GLOB_RECURSE COMMON_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/common/*.cpp)

list(APPEND ALL_HEADERS ${COMMON_HEADERS})
list(APPEND ALL_SOURCES ${COMMON_SOURCES})


# 添加protobuf
set(protobuf_BUILD_TESTS OFF)
set(protobuf_WITH_ZLIB OFF)


message(STATUS "All headers : ${ALL_HEADERS}")
message(STATUS "All sources : ${ALL_SOURCES}")


set(ABSL_ENABLE_INSTALL OFF)          # 禁用 Abseil 安装
set(protobuf_INSTALL OFF)             # 禁用 protobuf 安装
set(utf8_range_INSTALL OFF)           # 禁用 utf8_range 安装


set(protobuf_MSVC_STATIC_RUNTIME OFF)   # 使用动态运行时库


add_subdirectory(dep/abseil-cpp)
add_subdirectory(dep/protobuf)


set(protobuf_INSTALL OFF)
set(ABSL_ENABLE_INSTALL OFF)
# 设置路径
set(PROTOC_INCLUDE "dep/protoc/include")
set(GENERATED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/generated")

# 确保生成目录存在
file(MAKE_DIRECTORY ${GENERATED_DIR})

# 查找 protoc 编译器
find_program(PROTOC_EXECUTABLE protoc 
    PATHS "dep/protoc/bin" 
    REQUIRED
)
message(STATUS "protoc path: ${PROTOC_EXECUTABLE}")

# 递归查找所有 .proto 文件
file(GLOB_RECURSE PROTO_FILES "${CMAKE_CURRENT_SOURCE_DIR}/protos/*.proto")
message("Proto files : "${PROTO_FILES})

execute_process(
    COMMAND ${PROTOC_EXECUTABLE}
        --cpp_out=${GENERATED_DIR}
        -I${CMAKE_CURRENT_SOURCE_DIR}/protos
        -I${PROTOC_INCLUDE}
        ${PROTO_FILES}
)

file(GLOB PROTO_SOURCES ${GENERATED_DIR}/*.cc)

add_executable (main ${ALL_SOURCES} ${ALL_HEADERS} ${PROTO_SOURCES})



# 启动PCH
target_precompile_headers(main PRIVATE
    <vector>
    <iostream>
    <string>
)

find_package(Boost REQUIRED filesystem)
find_package(Boost REQUIRED)
# OpenSSL库
# 设置OpenSSL路径 OPENSSL_ROOT_DIR
find_package(OpenSSL REQUIRED)

target_link_libraries(main PRIVATE Boost::filesystem)
target_link_libraries(main PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(main PRIVATE libprotobuf)

target_include_directories(main PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(main PRIVATE ${GENERATED_DIR})

