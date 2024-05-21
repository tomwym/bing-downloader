cmake_minimum_required(VERSION 3.22)

project(
    bing-downloader
    VERSION 1.0
    LANGUAGES CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)

find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(GTest REQUIRED)
find_package(CURL REQUIRED)
find_package(LibXml2 REQUIRED)

include_directories(${LIBXML2_INCLUDE_DIR})

add_subdirectory(src)

add_executable(json-test src/alt/json.cpp)
target_link_libraries(json-test ${nlohmann_json_LIBRARIES})

find_package(Boost 1.82 COMPONENTS program_options REQUIRED)
add_executable(po src/alt/options.cpp)
target_link_libraries(po Boost::program_options)

add_executable(curltest src/alt/curltest.cpp)
target_link_libraries(curltest lib)

add_subdirectory(tests)

# cmake --build . -j
# boost woes
# export BOOST_ROOT="/home/t0/boost/boost_1_82_0"
# https://stackoverflow.com/questions/3016448/how-can-i-get-cmake-to-find-my-alternative-boost-installation
# https://stackoverflow.com/questions/3897839/how-to-link-c-program-with-boost-using-cmake/43885372#43885372