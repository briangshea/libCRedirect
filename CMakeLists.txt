#
# This file is part of libCRedirect.
#
# libCRedirect is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# libCRedirect is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with libCRedirect. If not, see <http://www.gnu.org/licenses/>.
#
# Copyright (C) Brian G Shea <bgshea@gmail.com>
#
cmake_minimum_required(VERSION 3.15...3.29)

set(PROJECT_NAME CRedirect)
project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

#option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(LIB_CREDIRECT_ENABLE_CERR "Enable cerr redirector" ON)
option(LIB_CREDIRECT_ENABLE_CLOG "Enable clog redirector" ON)
option(LIB_CREDIRECT_ENABLE_COUT "Enable cout redirector" ON)
option(LIB_CREDIRECT_AUTOSTART_CERR "Automatically start cerr redirector" OFF)
option(LIB_CREDIRECT_AUTOSTART_CLOG "Automatically start clog redirector" OFF)
option(LIB_CREDIRECT_AUTOSTART_COUT "Automatically start cout redirector" OFF)

set(LIB_CREDIRECT_NAMESPACE "" CACHE STRING "Namespace for the CRedirect library. If empty, no namespace is used.")
set(LIB_CREDIRECT_VERSION_MAJOR 0 CACHE STRING "Major version of the CRedirect library.")
set(LIB_CREDIRECT_VERSION_MINOR 1 CACHE STRING "Minor version of the CRedirect library.")
set(LIB_CREDIRECT_VERSION_PATCH 0 CACHE STRING "Patch version of the CRedirect library.")
set(LIB_CREDIRECT_VERSION_STRING "${LIB_CREDIRECT_VERSION_MAJOR}.${LIB_CREDIRECT_VERSION_MINOR}.${LIB_CREDIRECT_VERSION_PATCH}" CACHE STRING "Full version string of the CRedirect library.")

set(LIB_CREDIRECT_INITIAL_BUFFER_SIZE 1024 CACHE STRING "Initial buffer size for the redirectors. This is the size of the internal buffer used to store redirected output before it is processed.")

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Standard includes
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(CMakeDependentOption)

# Create configuration file
configure_file(${PROJECT_NAME}_config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/${PROJECT_NAME}_config.h @ONLY)

file(GLOB PROJECT_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/*.h*")

add_library(${PROJECT_NAME}
    src/CerrRedirect.cpp
    src/ClogRedirect.cpp
    src/CoutRedirect.cpp
    src/StreamRedirect.cpp
    src/SynchronousStreamBuf.cpp
    ${PROJECT_HEADERS}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${LIB_CREDIRECT_VERSION_STRING}
    SOVERSION ${LIB_CREDIRECT_VERSION_MAJOR}
    PUBLIC_HEADER "${PROJECT_HEADERS}"
)

# Install the library and headers
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

# Export the library for use in dependent projects
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Config.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# Create a package configuration file
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${LIB_CREDIRECT_VERSION_STRING}
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

if(${BUILD_SHARED_LIBS})
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DLIB_CREDIRECT_SHARED)
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}>
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

enable_testing()
add_subdirectory(Tests)
add_subdirectory(Examples)
