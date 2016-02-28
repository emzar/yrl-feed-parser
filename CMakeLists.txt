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

add_library(
  ${PROJECT_NAME}
  feed/bson.cpp
  feed/feed.cpp
  feed/offer_node.cpp
  feed/parser.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${LibXML++_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${CURL_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC ${LibXML++_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${CURL_LIBRARIES})

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
