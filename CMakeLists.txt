cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_COMPILER "D://mingw64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "D://mingw64/bin/g++.exe")

project("test")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

# used in the AndroidManifest.xml file.

include_directories(./include)
        # 改为 你的 MinGW 安装路径
include_directories(./include/external)

file(GLOB src "src/*.c")

find_library(pthread pthread ${CMAKE_CURRENT_LIST_DIR}/lib)

add_library(llib STATIC
        ${src})

file(GLOB test_files "test/*.c")

foreach(file IN LISTS test_files)
    # Get file name without directory
    get_filename_component(elfname ${file} NAME_WE)
    add_executable(${elfname} ${file})
    target_link_libraries(${elfname} llib pthread)
endforeach()

