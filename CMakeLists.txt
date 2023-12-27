cmake_minimum_required(VERSION 3.26)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_BUILD_TYPE "DEBUG")
set(SERVER_TARGET "t_server")
set(CLIENT_TARGET "t_client")
set(TEST_TARGET "t_test")

project(t)

include_directories(include)

file(GLOB SERVER_SOURCES
        include/*.h
        src/server/*.cpp
        src/server/*.cc
        src/common/*.cpp
        src/common/*.cc
)

file(GLOB CLIENT_SOURCES
        include/*.h
        src/client/*.cpp
        src/client/*.cc
        src/common/*.cpp
        src/common/*.cc
)

list(FILTER SERVER_SOURCES EXCLUDE REGEX "^.*/src/server/main.cpp$")
list(FILTER SERVER_SOURCES EXCLUDE REGEX ".*_test.cpp$")

list(FILTER CLIENT_SOURCES EXCLUDE REGEX "^.*/src/client/main.cpp$")
list(FILTER CLIENT_SOURCES EXCLUDE REGEX ".*_test.cpp$")

include_directories(include test)

add_executable(${SERVER_TARGET} ${SERVER_SOURCES} src/server/main.cpp)
add_executable(${CLIENT_TARGET} ${CLIENT_SOURCES} src/client/main.cpp)

message("client sources" ${CLIENT_SOURCES})

find_package(Boost 1.81.0 REQUIRED COMPONENTS program_options)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(glog CONFIG REQUIRED)

set(LIBRARIES Boost::boost Boost::program_options yaml-cpp glog::glog)
set(CLIENT_LIBRARIES ${LIBRARIES})
set(SERVER_LIBRARIES ${LIBRARIES})

target_link_libraries(${SERVER_TARGET} PRIVATE ${SERVER_LIBRARIES})

target_link_libraries(${CLIENT_TARGET} PRIVATE ${CLIENT_LIBRARIES})

enable_testing()
include(GoogleTest)
find_package(GTest CONFIG REQUIRED)

file(GLOB_RECURSE TEST_SOURCES
        *_test.cpp
        *_test.cc
        test/*.h
)

add_executable(
        ${TEST_TARGET}
        ${TEST_SOURCES}
)

set(TEST_LIBRARIES ${LIBRARIES} GTest::gtest_main)

#target_link_libraries(${TEST_TARGET} PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
target_link_libraries(${TEST_TARGET} PRIVATE ${TEST_LIBRARIES})
gtest_discover_tests(${TEST_TARGET})

#add_test(AllTestsInMain ${TEST_TARGET})