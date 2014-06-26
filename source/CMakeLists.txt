find_package(Boost COMPONENTS system filesystem program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

message("eigen directories: " ${EIGEN3_INCLUDE_DIRS})

set(HEADERS
    atom_properties.h
    atom.h
    configuration.h
    exceptions.h
    molecule.h
    utilities.h
)

set(SOURCES
    atom.cpp
    configuration.cpp
    exceptions.cpp
    molconv.cpp
    molecule.cpp
    utilities.cpp
)

add_executable(molconv ${SOURCES})
target_link_libraries(molconv ${Boost_LIBRARIES})