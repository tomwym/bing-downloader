cmake_minimum_required(VERSION 3.22)

project(
    test-server
    VERSION 1.0
    LANGUAGES CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)

add_subdirectory(src)
add_executable(server src/main.cpp)
add_executable(client test-client/main.cpp)
target_include_directories(client PUBLIC src)
target_link_libraries(server sandbox)
target_link_libraries(client sandbox)

add_subdirectory(tests)

add_executable(FoxServer src/FoxServer.cpp)
target_link_libraries(FoxServer sandbox)


# cmake --build . -j