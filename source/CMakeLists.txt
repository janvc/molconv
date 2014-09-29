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
find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)
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
file(GLOB SOURCES *.cpp)
file(GLOB UI_FORMS *.ui)
file(GLOB RESOURCES *.qrc)

qt5_wrap_ui(UI_SOURCES ${UI_FORMS})
qt5_add_resources(RCC_SOURCES ${RESOURCES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

add_executable(molconv ${SOURCES} ${UI_SOURCES} ${RCC_SOURCES})
target_link_libraries(molconv Qt5::Widgets Qt5::OpenGL ${CHEMKIT_LIBRARIES} ${Boost_LIBRARIES})
