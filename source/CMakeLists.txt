find_package(Chemkit COMPONENTS io graphics REQUIRED)
include_directories(${CHEMKIT_INCLUDE_DIRS})

find_package(Boost COMPONENTS system filesystem iostreams program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

find_package(Qt4 4.6 COMPONENTS QtCore QtGui QtOpenGL REQUIRED)
set(GT_USE_QTGUI TRUE)
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
    molconv.cpp
    utilities.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_executable(molconv ${SOURCES})
target_link_libraries(molconv ${CHEMKIT_LIBRARIES} ${Boost_LIBRARIES} ${QT_LIBRARIES})