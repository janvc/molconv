cmake_minimum_required(VERSION 2.8.11)

project(molconv)

set(CMAKE_MODULE_PATH ${molconv_SOURCE_DIR}/cmake)

# Update/build translations
set (FILES_TO_TRANSLATE )
option (UPDATE_TRANSLATIONS "Update source translation translations/*.ts")

#file (GLOB TRANSLATIONS_FILES translations/*.ts)
set(LANGUAGES de)
set(TRANSLATIONS_FILES)
foreach(LANGUAGE ${LANGUAGES})
    set(TS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/translations/molconv_${LANGUAGE}.ts")
    set(TRANSLATIONS_FILES ${TRANSLATIONS_FILES} ${TS_FILE})
  set_source_files_properties(${TS_FILE} PROPERTIES OUTPUT_LOCATION
    "${CMake_BINARY_DIR}${CMAKE_DATA_DIR}/i18n")
endforeach(LANGUAGE ${LANGUAGES})

add_subdirectory(source)

# prevent the generated files from being deleted during make clean
set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM true)
