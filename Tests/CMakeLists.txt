cmake_minimum_required(VERSION 3.15...3.29)

project(CRedirectTest LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(CRedirectTest
    CoutRedirectTest.cpp
)

target_link_libraries(CRedirectTest
    PRIVATE
        CRedirect
)

add_test(
    NAME Test_CoutRedirect 
    COMMAND $<TARGET_FILE:CRedirectTest> 1
)

add_test(
    NAME Test_ClogRedirect 
    COMMAND $<TARGET_FILE:CRedirectTest> 2
)

add_test(
    NAME Test_CerrRedirect 
    COMMAND $<TARGET_FILE:CRedirectTest> 3
)