find_package(Chemkit)

set(HEADERS
    molecule.h
)

set(SOURCES
    molecule.cpp
)

add_library(molconv-molecule SHARED ${SOURCES})
target_link_libraries(molconv-molecule ${CHEMKIT_LIBRARIES} ${Boost_LIBRARIES})

