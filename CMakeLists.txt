cmake_minimum_required(VERSION 3.26)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_BUILD_TYPE "DEBUG")

project(t)

include_directories(include)

file(GLOB SOURCES
        include/*.h
        src/*.cpp
        src/*.cc
)

list(FILTER SOURCES EXCLUDE REGEX "^.*/src/main.cpp$")

include_directories(include test)

add_executable(${PROJECT_NAME} ${SOURCES} src/main.cpp)

find_package(Boost 1.81.0 REQUIRED COMPONENTS program_options)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::boost Boost::program_options)

find_package(yaml-cpp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE yaml-cpp)

find_package(glog CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glog::glog)

set(TEST_TARGET ${PROJECT_NAME}_test)

enable_testing()
include(GoogleTest)
find_package(GTest CONFIG REQUIRED)

file(GLOB TEST_SOURCES
        test/*.cpp
        test/*.cc
        test/*.h
)

add_executable(
        ${TEST_TARGET}
        ${TEST_SOURCES}
        ${SOURCES}
)


#target_link_libraries(${TEST_TARGET} PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
target_link_libraries(${TEST_TARGET} PRIVATE GTest::gtest_main
        Boost::boost
        Boost::program_options
        yaml-cpp
        glog::glog
)
gtest_discover_tests(${TEST_TARGET})

#add_test(AllTestsInMain ${TEST_TARGET})