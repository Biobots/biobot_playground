cmake_minimum_required(VERSION 3.14)

project(cpptest)

aux_source_directory(. DIR_SRCS)

add_subdirectory(io io)
add_subdirectory(template template)

#aux_source_directory(io IO_DIR_SRCS)
#add_executable(iotest ${IO_DIR_SRCS})

#aux_source_directory(template TP_DIR_SRCS)
#add_executable(tptest ${TP_DIR_SRCS})