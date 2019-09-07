cmake_minimum_required(VERSION 3.14)

project(clwin)

AUX_SOURCE_DIRECTORY(src DIR_SRCS)
include_directories(inc)
link_directories(external)
find_library(CL_PATH OpenCL)
#link_libraries(OpenCL)

add_executable(main ${DIR_SRCS})
target_link_libraries(main ${CL_PATH})