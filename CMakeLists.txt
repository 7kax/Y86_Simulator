cmake_minimum_required (VERSION 3.0)

set(PROJECT_NAME y86_backend)
project (${PROJECT_NAME})

set (CMAKE_CXX_STANDARD 20)

# 设置输出文件目录
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

# 获取所有源文件
aux_source_directory(src SRC_DIR)

# 添加头文件目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 编译所有源文件, 生成可执行文件
add_executable (${PROJECT_NAME} ${SRC_DIR})
