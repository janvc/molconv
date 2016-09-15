#
# Copyright 2014 - 2016 Jan von Cosel & Sebastian Lenz
#
# This file is part of molconv.
#
# molconv is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# molconv is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have recieved a copy of the GNU General Public License
# along with molconv. If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 2.8.11)

project(molconv)

# Do not allow in-source builds:
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not allowed.")
endif()

set(CMAKE_MODULE_PATH ${molconv_SOURCE_DIR}/cmake)

# Update/build translations
set (FILES_TO_TRANSLATE )
option (UPDATE_TRANSLATIONS "Update source translation translations/*.ts")

#file (GLOB TRANSLATIONS_FILES translations/*.ts)
set(LANGUAGES de)
set(TRANSLATIONS_FILES)
foreach(LANGUAGE ${LANGUAGES})
    set(TS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/translations/molconv_${LANGUAGE}.ts")
    set(TRANSLATIONS_FILES ${TRANSLATIONS_FILES} ${TS_FILE})
  set_source_files_properties(${TS_FILE} PROPERTIES OUTPUT_LOCATION
    "${CMake_BINARY_DIR}${CMAKE_DATA_DIR}/i18n")
endforeach(LANGUAGE ${LANGUAGES})

# debugging output only for debug builds
if(NOT CMAKE_BUILD_TYPE MATCHES "Debug")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif()

add_definitions(-DQT_COMPILING_QSTRING_COMPAT_CPP)

# find out the git revision:
function(git_version dir version_var)
    if (EXISTS ${dir}/.git)
        execute_process(COMMAND git rev-parse --short HEAD
            WORKING_DIRECTORY ${dir}
            OUTPUT_VARIABLE var)
        string(REGEX REPLACE "\n" "" var "${var}")
        set(${version_var} "${var}" PARENT_SCOPE)
    endif(EXISTS ${dir}/.git)
endfunction()

git_version(${CMAKE_SOURCE_DIR} SCM_REVISION)

add_subdirectory(source)

# prevent the generated files from being deleted during make clean
set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM true)
