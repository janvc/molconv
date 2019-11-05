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

include_directories(${molconv_SOURCE_DIR})

set(test_molecule_SRCS
    ../source/molecule.cpp
    test_molecule.cpp
)

set(test_molconvwindow_SRCS
    ../source/molecule.cpp
    ../source/system.cpp
    ../source/importdialog.cpp
    ../source/exportdialog.cpp
    ../source/newgroupdialog.cpp
    ../source/setbasisdialog.cpp
    ../source/listofmolecules.cpp
    ../source/molconvwindow.cpp
    ../source/moleculesettings.cpp
    ../source/moleculeinfo.cpp
    ../source/moleculeitem.cpp
    ../source/groupitem.cpp
    ../source/moleculegroup.cpp
    ../source/graphicsaxisitem.cpp
    ../source/graphicsselectionitem.cpp
    ../source/multimoldialog.cpp
    ../source/moleculestack.cpp
    ../source/aboutdialog.cpp
    ../source/navigatetool.cpp
    ../source/selecttool.cpp
    test_molconvwindow.cpp
)

set(MOC_HEADERS
    molconvwindow.h
)

set(UI_FORMS
    ../source/importdialog.ui
    ../source/exportdialog.ui
    ../source/newgroupdialog.ui
    ../source/setbasisdialog.ui
    ../source/listofmolecules.ui
    ../source/moleculesettings.ui
    ../source/moleculeinfo.ui
    ../source/multimoldialog.ui
    ../source/aboutdialog.ui
    ../source/molconvwindow.ui
)

set(RESOURCES
    molconv.qrc
)

qt5_wrap_ui(UI_SOURCES ${UI_FORMS})
qt5_add_resources(RCC_SOURCES ${RESOURCES})

add_executable(test_molecule ${test_molecule_SRCS})
add_executable(test_molconvwindow ${test_molconvwindow_SRCS} ${UI_SOURCES})

target_link_libraries(test_molecule Qt5::Test ${CHEMKIT_LIBRARIES})
target_link_libraries(test_molconvwindow Qt5::Widgets Qt5::Xml Qt5::Test ${CHEMKIT_LIBRARIES} ${Boost_LIBRARIES})

add_test(NAME test_molecule COMMAND test_molecule)
add_test(NAME test_molconvwindow COMMAND test_molconvwindow)


