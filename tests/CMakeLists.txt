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

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../source/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
set(MOLCONV_INCLUDE_DIRS
    ${MOLCONV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)
include_directories(${MOLCONV_INCLUDE_DIRS})

set(test_molecule_SRCS
    test_molecule.cpp
)

set(test_molconvwindow_SRCS
    test_molconvwindow.cpp
)

set(RESOURCES
    molconv.qrc
)

qt5_wrap_ui(UI_SOURCES ${UI_FORMS})
qt5_add_resources(RCC_SOURCES ${RESOURCES})

add_executable(test_molecule ${test_molecule_SRCS})
add_executable(test_molconvwindow ${test_molconvwindow_SRCS})

target_link_libraries(test_molecule molconv-molecule molconv-system Qt5::Test ${CHEMKIT_LIBRARIES})
target_link_libraries(test_molconvwindow molconv-mainwindow molconv-io molconv-gui molconv-system molconv-molecule Qt5::Test ${CHEMKIT_LIBRARIES} ${Boost_LIBRARIES})

add_test(NAME test_molecule COMMAND test_molecule)
add_test(NAME test_molconvwindow COMMAND test_molconvwindow)


