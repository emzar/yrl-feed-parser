project(feed VERSION 0.0.1 LANGUAGES CXX)

enable_testing()
find_package(GTest REQUIRED)
find_package(LibXML++ REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(${PROJECT_NAME} feed.cpp offer_node.cpp parser.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${LibXML++_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LibXML++_LIBRARIES})

set(TEST_TARGET test_${PROJECT_NAME})

add_executable(
  ${TEST_TARGET} unittests/test_feed_parser.cpp)

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

