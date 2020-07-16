#
# Copyright 2014 - 2019 Jan von Cosel & Sebastian Lenz
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

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

set(MOLCONV_INCLUDE_DIRS
    ${MOLCONV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)
include_directories(${MOLCONV_INCLUDE_DIRS})
link_directories(${LIBRARY_OUTPUT_PATH})

set(SOURCES
    main.cpp
)

set(RESOURCES
    molconv.qrc
)

qt5_add_resources(RCC_SOURCES ${RESOURCES})

add_subdirectory(molecule)
add_subdirectory(system)
add_subdirectory(gui)

add_executable(molconv ${SOURCES} ${RCC_SOURCES} ${QM_FILES})
target_link_libraries(molconv molconv-gui molconv-system molconv-molecule ${CHEMKIT_LIBRARIES} ${Boost_LIBRARIES})

