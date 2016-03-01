cmake_minimum_required(VERSION 3.0.2)

project(yrl_feed_parser VERSION 0.0.1 LANGUAGES CXX)

set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -pthread -pedantic -Wall")
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

enable_testing()
find_package(GTest REQUIRED)
find_package(LibXML++ REQUIRED)
find_package(CURL REQUIRED)
find_package(Boost REQUIRED COMPONENTS date_time)

find_package(
  libbsoncxx
  REQUIRED
  CONFIG
    PATHS "/usr/local/lib/cmake/libbsoncxx-3.0.1-rc0-pre/"
  NO_DEFAULT_PATH)

find_package(
  libmongocxx
  REQUIRED
  CONFIG
    PATHS "/usr/local/lib/cmake/libmongocxx-3.0.1-rc0-pre/"
  NO_DEFAULT_PATH)

add_library(
  ${PROJECT_NAME}
  feed/bson.cpp
  feed/feed.cpp
  feed/offer_node.cpp
  feed/offer_parser.cpp
  feed/parser.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${LibXML++_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${CURL_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${LIBBSONCXX_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${LIBMONGOCXX_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC ${LibXML++_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${CURL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBBSONCXX_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBMONGOCXX_LIBRARIES})

configure_file(
  feed/unittests/test.xml "${PROJECT_BINARY_DIR}/test.xml" COPYONLY)

set(TEST_TARGET test_${PROJECT_NAME})

add_executable(
  ${TEST_TARGET} feed/unittests/test_feed_parser.cpp)

target_include_directories(${TEST_TARGET} PRIVATE ${GTEST_INCLUDE_DIRS})

target_link_libraries(
  ${TEST_TARGET} ${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})

add_test(feed_test ${TEST_TARGET})

add_custom_command(
  TARGET ${TEST_TARGET}
  POST_BUILD
  COMMAND ctest --output-on-failure)

set_property(
  TARGET ${TEST_TARGET}
  PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
