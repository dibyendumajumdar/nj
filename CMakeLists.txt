###############################################################################
# Copyright (c) 2017, 2017 IBM Corp. and others
#
# This program and the accompanying materials are made available under
# the terms of the Eclipse Public License 2.0 which accompanies this
# distribution and is available at https://www.eclipse.org/legal/epl-2.0/
# or the Apache License, Version 2.0 which accompanies this distribution and
# is available at https://www.apache.org/licenses/LICENSE-2.0.
#
# This Source Code may also be made available under the following
# Secondary Licenses when the conditions for such availability set
# forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
# General Public License, version 2 with the GNU Classpath
# Exception [1] and GNU General Public License, version 2 with the
# OpenJDK Assembly Exception [2].
#
# [1] https://www.gnu.org/software/classpath/license.html
# [2] http://openjdk.java.net/legal/assembly-exception.html
#
# SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
###############################################################################

# TODO: OMR_RTTI flag
# TODO: Version things

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

message(STATUS "Starting with CMake version ${CMAKE_VERSION}")

include(cmake/versions.cmake) # Required for OMR_VERSION

project(nj VERSION ${OMR_VERSION} LANGUAGES CXX C)

set(omr_SOURCE_DIR ${CMAKE_SOURCE_DIR})
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

set_property(GLOBAL PROPERTY USE_FOLDERS TRUE)

include(OmrAssert)
include(OmrFindFiles)
include(OmrHookgen)
include(OmrPlatform)
include(OmrTracegen)
include(OmrSanitizerSupport)

###
### Set up the global platform configuration
###

if(NOT OMR_PLATFORM_GLOBALLY_INITIALIZED)
	omr_platform_global_setup()
endif()
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

###
### Set up install path stuff
###
# Note the paths are a bit odd compared to standard gnu install paths
# This is because we arent expecting to actually be installing globally
# since we don't have a good story for installing OMR system-wide
set(CMAKE_INSTALL_BINDIR "${CMAKE_INSTALL_PREFIX}/bin")
set(CMAKE_INSTALL_LIBDIR "${CMAKE_INSTALL_PREFIX}/lib")
set(OMR_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}" CACHE PATH "Root path where to install OMR")
#set(CMAKE_INSTALL_PREFIX "${OMR_INSTALL_PREFIX}")
#include(GNUInstallDirs)
#set(CMAKE_INSTALL_RPATH "\$ORIGIN")
add_custom_target(omr_install_tooling
	COMMAND ${CMAKE_COMMAND} --build tools --target preinstall
	COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_COMPONENT=tooling -P "${CMAKE_BINARY_DIR}/cmake_install.cmake"
)

###
### Versions and stuff
###

include(cmake/config.cmake)

enable_testing()

include_directories(
	${PROJECT_BINARY_DIR}
	./include/
	./include_core/
	./third_party/
)

add_definitions(
	-DUT_DIRECT_TRACE_REGISTRATION # TODO:  Deal with that stupid jni issue in tracegen
)

# Check for existing omrcfg in the source tree since this can cause alot of headaches
# Also check if we are building in tree while we are at it
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
	if(NOT MSVC_IDE) #MSVC will handle in tree builds ok
		message(WARNING "In tree builds are not recommended")
	endif()
else()
include(cmake/CheckSourceTree.cmake)
add_custom_target(header_check ALL
	${CMAKE_COMMAND} "-Domr_SOURCE_DIR=${omr_SOURCE_DIR}" -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/CheckSourceTree.cmake)
endif()

configure_file(./omrcfg.CMakeTemplate.h omrcfg.h)
configure_file(./omrversionstrings.CMakeTemplate.h omrversionstrings.h)

###
### Native Tooling
###

if(OMR_TOOLS_IMPORTFILE)
	include("${OMR_TOOLS_IMPORTFILE}")
elseif(OMR_TOOLS)
	add_subdirectory(tools)
else()
	message(FATAL_ERROR "OMR: Build tools are required. See OMR_TOOLS and OMR_TOOLS_IMPORTFILE")
endif()


###
### Built-in OMR Applications.
###

###
### Core OMR components
###
add_subdirectory(third_party)
add_subdirectory(compiler)
add_subdirectory(jitbuilder)
add_subdirectory(frontends)
add_subdirectory(util)
add_subdirectory(fvtest)

