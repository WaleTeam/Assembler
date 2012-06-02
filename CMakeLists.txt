cmake_minimum_required (VERSION 2.6)
project (AS)
include_directories(${AS_SOURCE_DIR}/include)
add_executable(as src/main.c src/emu.c)
