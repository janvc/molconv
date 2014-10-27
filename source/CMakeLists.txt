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

file(GLOB HEADERS *.h)
file(GLOB SOURCES
    abstractmoleculegroup.cpp
    config.cpp
    exceptions.cpp
    molconv_window.cpp
    molecule.cpp
    moleculegroup.cpp
    molecules_dock.cpp
    moleculestack.cpp
    open_dialog.cpp
    system.cpp
    utilities.cpp
)


#list(REMOVE_ITEM SOURCES main.cpp)
#list(REMOVE_ITEM SOURCES testprogram.cpp)
file(GLOB UI_FORMS *.ui)
file(GLOB RESOURCES *.qrc)
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
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

add_executable(molconv main.cpp ${SOURCES} ${UI_SOURCES} ${RCC_SOURCES} ${QM_FILES})
add_executable(testprogram testprogram.cpp ${SOURCES})
target_link_libraries(molconv Qt5::Widgets Qt5::OpenGL ${CHEMKIT_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(testprogram Qt5::Widgets Qt5::OpenGL ${CHEMKIT_LIBRARIES} ${Boost_LIBRARIES})
