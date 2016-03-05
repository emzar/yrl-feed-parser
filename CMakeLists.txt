cmake_minimum_required(VERSION 3.0.2)

project(yrl VERSION 0.0.1 LANGUAGES CXX)

set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -pthread -pedantic -Wall")
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

add_subdirectory(feed)
add_subdirectory(app)
