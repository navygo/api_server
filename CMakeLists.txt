cmake_minimum_required(VERSION 3.10)
project(APIServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含目录
include_directories(include)

# 源文件
set(SOURCES
    src/api_server.cpp
    src/router.cpp
    src/main.cpp
)

# 主可执行文件
add_executable(api_server ${SOURCES})

# Windows特定设置
if(WIN32)
    target_link_libraries(api_server ws2_32)
endif()

# 编译选项
target_compile_options(api_server PRIVATE -Wall -Wextra)
