option(build_examples "Build examples" On)

find_package(Threads)

if(build_examples)
message(STATUS "Building examples")

add_executable(logging_example src/logging_example.cpp)
target_include_directories(logging_example PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(logging_example Threads::Threads)

if(WIN32)
message(STATUS "Building with windows sockets")
set(SOCKET_SOURCES ${CMAKE_SOURCE_DIR}/src/socket/windows_socket.cpp
${CMAKE_SOURCE_DIR}/src/util/util.cpp)
elseif(UNIX)
message(STATUS "Building with linux sockets")
set(SOCKET_SOURCES ${CMAKE_SOURCE_DIR}/src/socket/linux_socket.cpp)
endif()

add_executable(socket_example 
src/socket_example.cpp
${SOCKET_SOURCES})
target_include_directories(socket_example PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(socket_example $<$<PLATFORM_ID:Windows>:wsock32> $<$<PLATFORM_ID:Windows>:ws2_32>)


add_executable(dispatcher_example 
src/dispatcher_example.cpp
${CMAKE_SOURCE_DIR}/src/multiplexer/select_dispatcher.cpp
${CMAKE_SOURCE_DIR}/src/multiplexer/accept_handler.cpp
${CMAKE_SOURCE_DIR}/src/multiplexer/read_handler.cpp
${CMAKE_SOURCE_DIR}/src/multiplexer/multiplexer_factory.cpp
${CMAKE_SOURCE_DIR}/src/multiplexer/select_multiplexer.cpp
${CMAKE_SOURCE_DIR}/src/multiplexer/dispatcher_factory.cpp
${CMAKE_SOURCE_DIR}/src/multiplexer/request_handler_factory.cpp
${CMAKE_SOURCE_DIR}/src/server/server.cpp
${CMAKE_SOURCE_DIR}/src/server/url_router.cpp
${CMAKE_SOURCE_DIR}/src/util/util.cpp
${CMAKE_SOURCE_DIR}/src/util/duration.cpp
${CMAKE_SOURCE_DIR}/src/util/datetime.cpp
${CMAKE_SOURCE_DIR}/src/server/http_parser.cpp
${CMAKE_SOURCE_DIR}/src/server/http_response.cpp
${CMAKE_SOURCE_DIR}/src/server/types.cpp
${SOCKET_SOURCES})
target_include_directories(dispatcher_example  PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(dispatcher_example  $<$<PLATFORM_ID:Windows>:wsock32> $<$<PLATFORM_ID:Windows>:ws2_32>)

add_executable(read_file_example
src/read_file_example
${CMAKE_SOURCE_DIR}/src/util/util.cpp)
target_include_directories(read_file_example PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_custom_command(
        TARGET read_file_example POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/examples/src/test_file.html
        ${CMAKE_CURRENT_BINARY_DIR}/test_file.html)
endif()