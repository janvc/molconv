#find_package(Chemkit)
#find_package(Boost)

set(HEADERS
    abstractmoleculegroup.h
    moleculegroup.h
    moleculestack.h
)

set(SOURCES
    abstractmoleculegroup.cpp
    moleculegroup.cpp
    moleculestack.cpp
)

add_library(molconv-moleculegroup SHARED ${SOURCES})
target_link_libraries(molconv-moleculegroup ${CHEMKIT_LIBRARIES} ${Boost_LIBRARIES})

