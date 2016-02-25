cmake_minimum_required(VERSION 3.0.2)

project(realty_parser VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -pthread -pedantic -Wall")

find_package(
  libbsoncxx
  REQUIRED
  CONFIG
  PATHS
    "/usr/local/lib/cmake/libbsoncxx-3.0.1-rc0-pre/"
  NO_DEFAULT_PATH)

find_package(
  libmongocxx
  REQUIRED
  CONFIG
  PATHS
    "/usr/local/lib/cmake/libmongocxx-3.0.1-rc0-pre/"
  NO_DEFAULT_PATH)

find_package(CURL REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

message(STATUS "LIBBSONCXX_INCLUDE_DIRS: ${LIBBSONCXX_INCLUDE_DIRS}")

target_include_directories(${PROJECT_NAME} PUBLIC ${LIBBSONCXX_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${LIBMONGOCXX_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${LIBBSONCXX_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${LIBMONGOCXX_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES})
