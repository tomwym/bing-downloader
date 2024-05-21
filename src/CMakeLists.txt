add_library(lib
                XMLCacheManager.cpp
                XMLContainer.cpp
                XMLQueries.cpp
                CurlWriteChunks.cpp
                filters/YearFilter.cpp
                filters/MonthFilter.cpp
                filters/DayFilter.cpp
                filters/PassConditionalFilter.cpp)
target_link_libraries(lib ${nlohmann_json_LIBRARIES})
target_link_libraries(lib ${CURL_LIBRARIES})
target_link_libraries(lib ${LIBXML2_LIBRARIES})

add_executable(bing-downloader main.cpp)
target_link_libraries(bing-downloader ${nlohmann_json_LIBRARIES})
target_link_libraries(bing-downloader ${CURL_LIBRARIES})
target_link_libraries(bing-downloader ${LIBXML2_LIBRARIES})
target_link_libraries(bing-downloader lib)

