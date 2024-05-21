include(CTest)
add_executable(ALL
                XMLContainerTests.cpp
                FiltersTest.cpp
                ResourceGetterTests.cpp
)
target_link_libraries(ALL lib)
target_link_libraries(ALL ${GTEST_LIBRARIES})
target_link_libraries(ALL GTest::gmock_main)


target_include_directories(ALL PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_test(NAME AllTests COMMAND ALL)