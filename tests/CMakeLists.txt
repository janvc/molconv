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

include_directories(${molconv_SOURCE_DIR})

set(test_molecule_SRCS
    ../source/molecule.cpp
    test_molecule.cpp
)

set(test_molconv_window_SRCS
    ../source/molconv_window.cpp
    test_molconv_window.cpp
)

set(MOC_HEADERS
    molconv_window.h
)

set(UI_FORMS
    molconv_window.ui
)

set(RESOURCES
    molconv.qrc
)

qt5_wrap_ui(UI_SOURCES ${UI_FORMS})
qt5_add_resources(RCC_SOURCES ${RESOURCES})

add_executable(test_molecule ${test_molecule_SRCS})
add_executable(test_molconv_window ${test_molconv_window_SRCS} ${UI_SOURCES})

target_link_libraries(test_molecule Qt5::Test ${CHEMKIT_LIBRARIES})
target_link_libraries(test_molconv_window Qt5::Test ${CHEMKIT_LIBRARIES})

add_test(NAME test_molecule COMMAND test_molecule)
add_test(NAME test_molconv_window COMMAND test_molconv_window)


