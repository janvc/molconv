set(HEADERS
    system.h
)

set(SOURCES
    system.cpp
)

add_library(molconv-system SHARED ${SOURCES})
target_link_libraries(molconv-system ${CHEMKIT_LIBRARIES} ${Boost_LIBRARIES})

