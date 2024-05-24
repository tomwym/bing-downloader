add_executable(json-test json.cpp)
target_link_libraries(json-test ${nlohmann_json_LIBRARIES})

find_package(Boost 1.82 COMPONENTS program_options REQUIRED)
add_executable(po options.cpp)
target_link_libraries(po Boost::program_options)

add_executable(curltest curltest.cpp)
target_link_libraries(curltest lib)