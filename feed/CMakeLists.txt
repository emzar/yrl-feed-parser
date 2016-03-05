project(yrl_feed_parser VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(PREFIX_3PP ${CMAKE_SOURCE_DIR}/3pp)
set(PREFIX_3PP_LIB ${PREFIX_3PP}/lib)

get_filename_component(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)

enable_testing()

find_package(GTest REQUIRED)
find_package(LibXML++ REQUIRED)
find_package(CURL REQUIRED)
find_package(Boost REQUIRED)

add_library(3pp::bson SHARED IMPORTED)
set_target_properties(
  3pp::bson PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/3pp/include/libbson-1.0"
  IMPORTED_LOCATION "${_IMPORT_PREFIX}/3pp/lib/libbson-1.0.so"
  IMPORTED_SONAME "libbson-1.0.so.0")

add_library(3pp::mongoc SHARED IMPORTED)
set_target_properties(
  3pp::mongoc PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/3pp/include/libmongoc-1.0"
  IMPORTED_LOCATION "${_IMPORT_PREFIX}/3pp/lib/libmongoc-1.0.so"
  IMPORTED_SONAME "libmongoc-1.0.so.0")

add_library(3pp::bsoncxx SHARED IMPORTED)
set_target_properties(
  3pp::bsoncxx PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/3pp/include/bsoncxx/v_noabi"
  IMPORTED_LOCATION "${_IMPORT_PREFIX}/3pp/lib/libbsoncxx.so"
  IMPORTED_SONAME "libbsoncxx.so._noabi")

add_library(3pp::mongocxx SHARED IMPORTED)
set_target_properties(
  3pp::mongocxx PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/3pp/include/mongocxx/v_noabi"
  IMPORTED_LOCATION "${_IMPORT_PREFIX}/3pp/lib/libmongocxx.so"
  IMPORTED_SONAME "libmongocxx.so._noabi")

add_library(
  ${PROJECT_NAME}
  feed.cpp
  offer_node.cpp
  offer_parser.cpp
  parser.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${LibXML++_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${CURL_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ..)

target_link_libraries(${PROJECT_NAME} PUBLIC ${LibXML++_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${CURL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PUBLIC 3pp::bson)
target_link_libraries(${PROJECT_NAME} PUBLIC 3pp::mongoc)
target_link_libraries(${PROJECT_NAME} PUBLIC 3pp::bsoncxx)
target_link_libraries(${PROJECT_NAME} PUBLIC 3pp::mongocxx)

configure_file(
  unittests/test.xml "${PROJECT_BINARY_DIR}/test.xml" COPYONLY)

set(TEST_TARGET test_${PROJECT_NAME})

add_executable(
  ${TEST_TARGET} unittests/test_feed_parser.cpp)

target_include_directories(${TEST_TARGET} PRIVATE ${GTEST_INCLUDE_DIRS})

target_link_libraries(${TEST_TARGET} PUBLIC ${PROJECT_NAME})
target_link_libraries(${TEST_TARGET} PUBLIC ${GTEST_BOTH_LIBRARIES})

add_test(feed_test ${TEST_TARGET})

add_custom_command(
  TARGET ${TEST_TARGET}
  POST_BUILD
  COMMAND ctest --output-on-failure)

set_property(
  TARGET ${TEST_TARGET}
  PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
