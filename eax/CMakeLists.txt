cmake_minimum_required(VERSION 3.15)

project(eax)

set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -Wall -Werror -Wextra -O0 -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -Wall -Werror -Wextra -O3")

add_subdirectory(src)
