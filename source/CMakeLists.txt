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
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

# Boost libraries are required
find_package(Boost COMPONENTS system iostreams filesystem program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# Chemkit libraries are required
find_package(Chemkit COMPONENTS io graphics gui REQUIRED)
include_directories(${CHEMKIT_INCLUDE_DIRS})

# Eigen3 is required
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# We are using Qt5 for the GUI
find_package(Qt5 COMPONENTS Widgets OpenGL LinguistTools REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5OpenGL_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

FIND_PACKAGE(Qt5LinguistTools)
IF(UPDATE_TRANSLATIONS)
    IF(NOT Qt5_LUPDATE_EXECUTABLE)
        MESSAGE(WARNING "Qt's lupdate not found.")
    ENDIF()
ENDIF()
IF(NOT Qt5_LRELEASE_EXECUTABLE)
    MESSAGE(WARNING "Qt's lrelease not found.")
ENDIF()

set(CMAKE_AUTOMOC ON)
# AUTOUIC confusingly doesn't work
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCES
    main.cpp
    molconv_window.cpp
    moleculeitem.cpp
    groupitem.cpp
    newgroupdialog.cpp
    listofmolecules.cpp
    moleculesettings.cpp
    import_dialog.cpp
    export_dialog.cpp
    multimoldialog.cpp
    setbasisdialog.cpp
    graphicsaxisitem.cpp
    graphicsselectionitem.cpp
    molecule.cpp
    system.cpp
    moleculegroup.cpp
    moleculestack.cpp
    aboutdialog.cpp
    navigatetool.cpp
    selecttool.cpp
)

set(MOC_HEADERS
    molconv_window.h
    listofmolecules.h
    moleculesettings.h
    newgroupdialog.h
    import_dialog.h
    export_dialog.h
    multimoldialog.h
    setbasisdialog.h
    aboutdialog.h
)

set(UI_FORMS
    molconv_window.ui
    listofmolecules.ui
    moleculesettings.ui
    newgroupdialog.ui
    import_dialog.ui
    export_dialog.ui
    multimoldialog.ui
    setbasisdialog.ui
    aboutdialog.ui
)

set(RESOURCES
    molconv.qrc
)

set(FILES_TO_TRANSLATE ${FILES_TO_TRANSLATE} ${HEADERS} ${SOURCES} ${UI_FORMS} ${RESOURCES})

if(TRANSLATIONS_FILES)
  if (UPDATE_TRANSLATIONS)
    qt5_create_translation(QM_FILES ${FILES_TO_TRANSLATE}
  ${TRANSLATIONS_FILES})
  else (UPDATE_TRANSLATIONS)
    qt5_add_translation(QM_FILES ${TRANSLATIONS_FILES})
  endif (UPDATE_TRANSLATIONS)
endif(TRANSLATIONS_FILES)

qt5_wrap_ui(UI_SOURCES ${UI_FORMS})
qt5_add_resources(RCC_SOURCES ${RESOURCES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -ggdb -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra")

add_executable(molconv ${SOURCES} ${UI_SOURCES} ${RCC_SOURCES} ${QM_FILES})
target_link_libraries(molconv Qt5::Widgets Qt5::OpenGL ${CHEMKIT_LIBRARIES} ${Boost_LIBRARIES} ${OPENGL_LIBRARIES})
