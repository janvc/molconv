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

include_directories(${MOLCONV_INCLUDE_DIRS})

set(gui_SOURCES
    moleculeitem.cpp
    groupitem.cpp
    newgroupdialog.cpp
    listofmolecules.cpp
    moleculesettings.cpp
    importdialog.cpp
    exportdialog.cpp
    multimoldialog.cpp
    setbasisdialog.cpp
    graphicsaxisitem.cpp
    graphicsselectionitem.cpp
    aboutdialog.cpp
    navigatetool.cpp
    selecttool.cpp
    moleculeinfo.cpp
    settingsdialog.cpp
)

set(MOC_HEADERS
    listofmolecules.h
    moleculesettings.h
    newgroupdialog.h
    importdialog.h
    exportdialog.h
    multimoldialog.h
    setbasisdialog.h
    aboutdialog.h
    moleculeinfo.h
    settingsdialog.h
)

set(UI_FORMS
    listofmolecules.ui
    moleculesettings.ui
    newgroupdialog.ui
    importdialog.ui
    exportdialog.ui
    multimoldialog.ui
    setbasisdialog.ui
    aboutdialog.ui
    moleculeinfo.ui
    settingsdialog.ui
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

add_library(molconv-gui SHARED ${gui_SOURCES} ${UI_SOURCES})
target_link_libraries(molconv-gui Qt5::Widgets Qt5::OpenGL Qt5::Xml ${OPENGL_LIBRARIES})

