find_package(GTest REQUIRED)

include(CTest)
add_executable(ServerTests
               BasicTypesTests.cpp
               NaiveSocketComponentsTests.cpp
               HTTPHeaderParseTests.cpp
               NaiveHeaderTests.cpp
)
target_include_directories(ServerTests PUBLIC ${CMAKE_SOURCE_DIR}/test-server/src)
target_link_libraries(ServerTests sandbox)
target_link_libraries(ServerTests ${GTEST_LIBRARIES})
target_link_libraries(ServerTests GTest::gmock_main)

add_test(NAME ServerTestsName COMMAND ServerTests)