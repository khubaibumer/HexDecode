cmake_minimum_required(VERSION 3.13)
project(HexToData C)

set(CMAKE_C_STANDARD 11)

include_directories(include)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
add_executable(HexToData src/main.c src/decoder.c src/program_options.c src/fops.c src/packet.c src/parser.c src/decompressor.c)
