# Boost libraries are required
find_package(Boost COMPONENTS system iostreams filesystem program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# Chemkit libraries are required
find_package(Chemkit COMPONENTS io graphics REQUIRED)
include_directories(${CHEMKIT_INCLUDE_DIRS})

# Eigen3 is required
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# We are using Qt5 for the GUI
find_package(Qt5 COMPONENTS Widgets OpenGL LinguistTools REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5OpenGL_INCLUDE_DIRS})

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
    config.cpp
    exceptions.cpp
    main.cpp
    molconv_window.cpp
    molecules_dock.cpp
    moleculesettings.cpp
    open_dialog.cpp
    utilities.cpp
)

set(MOC_HEADERS
    molconv_window.h
    molecules_dock.h
    moleculesettings.h
    open_dialog.h
)

set(UI_FORMS
    molconv_window.ui
    molecules_dock.ui
    moleculesettings.ui
    open_dialog.ui
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
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

add_subdirectory(molconv-molecule)
add_subdirectory(molconv-moleculegroup)
add_subdirectory(molconv-system)
add_subdirectory(include)


add_executable(molconv ${SOURCES} ${UI_SOURCES} ${RCC_SOURCES} ${QM_FILES})
add_executable(testprogram testprogram.cpp)
target_link_libraries(molconv molconv-molecule molconv-moleculegroup molconv-system Qt5::Widgets Qt5::OpenGL ${CHEMKIT_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(testprogram molconv-molecule molconv-moleculegroup molconv-system Qt5::Widgets Qt5::OpenGL ${CHEMKIT_LIBRARIES} ${Boost_LIBRARIES})
