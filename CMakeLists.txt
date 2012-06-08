cmake_minimum_required (VERSION 2.6)
project (AS)

set(CMAKE_C_FLAGS "-std=c99 -Werror")

include_directories(${AS_SOURCE_DIR}/include)
add_executable(as src/main.c src/emu.c src/tokenizer.c src/list.c src/parser.c)
