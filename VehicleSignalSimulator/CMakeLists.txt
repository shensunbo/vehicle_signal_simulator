cmake_minimum_required(VERSION 3.10)

project(ConsoleCamApp)

# Set C++ standard to C++11 or newer
set(CMAKE_CXX_STANDARD 14) # ACF implementation requires C++17 but provided API and generated code is compliant with C++14
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add debug symbols to the compilation
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# Comment out this code to disable assert() in release version
# add_definitions(-DNDEBUG)

# Add dep inc path
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src/dep)

# add source file need to compile 
set(SOURCES 
    src/ConsoleCamImpl.cpp
    src/MessagelProccessor.cpp
    src/dep/DemoInterfaceAdapter..cpp
    src/main.cpp
)

add_executable(ConsoleCameraApp ${SOURCES})
target_include_directories(ConsoleCameraApp PRIVATE ${INC_DIR} src)

#unit test
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/unit_test_helper.cmake)

set(GTEST_DEP_SOURCES     
    src/ConsoleCamImpl.cpp
    src/MessagelProccessor.cpp
)

set(GTEST_DEP_LIBS     
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/gtest/libgtest.a
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/gtest/libgmock.a
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/gtest/libgtest_main.a
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/gtest/libgmock_main.a
)

# MessagelProccessorTest
add_new_test(
    TEST_NAME
    MessagelProccessorTest
    SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/MessagelProccessor_test.cpp
    DEP_SRCS
        ${GTEST_DEP_SOURCES}
    HEADER_DIR
        ${INC_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    LIB_DEP
        ${GTEST_DEP_LIBS}
)