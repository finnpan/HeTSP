cmake_minimum_required(VERSION 3.15)

# set the project name
project(eax)

include_directories(include)
aux_source_directory(src SRC_LIST)

# add the executable
add_executable(${PROJECT_NAME} ${SRC_LIST})