cmake_minimum_required(VERSION 3.15)

# set the project name
project(lkh)

set(CMAKE_CPP_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(CMAKE_CPP_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

include_directories(include)
aux_source_directory(src SRC_LIST)

# add the executable
add_executable(${PROJECT_NAME} ${SRC_LIST})