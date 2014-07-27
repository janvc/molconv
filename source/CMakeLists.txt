find_package(Chemkit COMPONENTS io graphics REQUIRED)
include_directories(${CHEMKIT_INCLUDE_DIRS})

find_package(Boost COMPONENTS system filesystem iostreams program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

find_package(Qt4 4.6 COMPONENTS QtCore QtGui QtOpenGL REQUIRED)
set(QT_USE_QTGUI TRUE)
include(${QT_USE_FILE})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(HEADERS
    atom_properties.h
    atom.h
    configuration.h
    exceptions.h
    molecule.h
    utilities.h
)

set(SOURCES
    molecule.cpp
    atom.cpp
    configuration.cpp
    exceptions.cpp
    molconv_window.cpp
    molecule_list.cpp
    molconv.cpp
    utilities.cpp
)

set(MOC_HEADERS
	molconv_window.h
	molecule_list.h
)

set(UI_FORMS
	molconv_window.ui
	molecule_list.ui
)

qt4_wrap_ui(UI_SOURCES ${UI_FORMS})
qt4_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_executable(molconv ${SOURCES} ${MOC_SOURCES} ${UI_SOURCES})
target_link_libraries(molconv ${CHEMKIT_LIBRARIES} ${Boost_LIBRARIES} ${QT_LIBRARIES})