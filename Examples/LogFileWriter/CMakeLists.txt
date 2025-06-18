cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME LogFileWriter)

project(${PROJECT_NAME})

option(USE_CERR_REDIRECT "Use CERR redirect" OFF)
option(USE_CLOG_REDIRECT "Use CLOG redirect" ON)

if(USE_CERR_REDIRECT AND USE_CLOG_REDIRECT)
    message(FATAL_ERROR "Cannot use both CERR and CLOG redirects at the same time.")
elseif(NOT USE_CERR_REDIRECT AND NOT USE_CLOG_REDIRECT)
    message(FATAL_ERROR "At least one of CERR or CLOG redirects must be enabled.")
endif()

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/LogFileWriter.cpp
    src/Logging.cpp
)

configure_file(LogFileWriterConfig.h.in src/LogFileWriterConfig.h @ONLY)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}/src
)

target_link_libraries(${PROJECT_NAME} CRedirect)

# Set the C++ standard
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
