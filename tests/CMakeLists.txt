
include_directories(${molconv_SOURCE_DIR})

set(ExampleTest_SRCS
    ../source/molecule.cpp
    testexample.cpp
)

add_executable(ExampleTest ${ExampleTest_SRCS})
target_link_libraries(ExampleTest Qt5::Test ${CHEMKIT_LIBRARIES})
add_test(NAME ExampleTest COMMAND ExampleTest)



