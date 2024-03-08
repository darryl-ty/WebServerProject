cmake_minimum_required(VERSION 3.25)
project(WebServerProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} src/server.cpp)
#add_executable(${PROJECT_NAME} src/client.cpp)
target_link_libraries(${PROJECT_NAME} ws2_32)
