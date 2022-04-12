cmake_minimum_required(VERSION 3.20)
project(nexus_multiplex VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(nexus_reactor SHARED src/reactor.cpp)
target_include_directories(nexus_reactor PUBLIC include)
