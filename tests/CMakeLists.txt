enable_testing()

option(build_tests "Build all tests" OFF)

if(build_tests)
message(STATUS "Building webserver tests")

add_executable(util_tests util/util_tests.cpp
${CMAKE_SOURCE_DIR}/src/util/util.cpp
${CMAKE_SOURCE_DIR}/src/util/datetime.cpp)
target_include_directories(util_tests PRIVATE 
${CMAKE_SOURCE_DIR}/googletest/googletest/include
${CMAKE_SOURCE_DIR}/include)
target_link_libraries(util_tests GTest::gtest GTest::gtest_main)

add_executable(util_http_tests http_parser/http_parser_tests.cpp
${CMAKE_SOURCE_DIR}/src/util/util.cpp
${CMAKE_SOURCE_DIR}/src/server/http_parser.cpp
${CMAKE_SOURCE_DIR}/src/server/http_response.cpp
${CMAKE_SOURCE_DIR}/src/util/datetime.cpp)
target_include_directories(util_http_tests PRIVATE 
${CMAKE_SOURCE_DIR}/googletest/googletest/include
${CMAKE_SOURCE_DIR}/include)
target_link_libraries(util_http_tests GTest::gtest GTest::gtest_main)

include(GoogleTest)

endif()