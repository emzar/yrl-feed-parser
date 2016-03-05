project(yrl_feed LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(
  ${PROJECT_NAME}
  main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC yrl_feed_parser)
