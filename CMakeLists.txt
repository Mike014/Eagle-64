cmake_minimum_required(VERSION 3.28)

project(Eagle64
    VERSION 0.1.0
    LANGUAGES C CXX ASM_NASM)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:/W4>
                        $<$<COMPILE_LANGUAGE:C,CXX>:/WX>)
endif()

add_subdirectory(core)
enable_testing()
add_subdirectory(tests)
