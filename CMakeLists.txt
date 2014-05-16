cmake_minimum_required (VERSION 2.6)

project (kompilator)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/bin")
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/lib")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/lib")

if (NOT CMAKE_BUILD_TYPE) 
    set (CMAKE_BUILD_TYPE "release")
endif (NOT CMAKE_BUILD_TYPE)
message (STATUS "Building ${CMAKE_BUILD_TYPE}")

find_package (FLEX)
if (FLEX_FOUND)
    # nothing to do
endif ()

find_package (BISON)
if (BISON_FOUND)
    # nothing to do
endif ()

set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)
set (VERSION_PATCH 0)
set (VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

set (SOURCEDIR "src")

link_directories (${FLEX_LIBRARIES})


FLEX_TARGET (scanner ${SOURCEDIR}/scanner.l ${SOURCEDIR}/scanner.lex.cc)
BISON_TARGET (parser ${SOURCEDIR}/parser.y ${SOURCEDIR}/parser.tab.cc)

add_executable (kompilator
        ${SOURCEDIR}/main.cc
        ${SOURCEDIR}/parser.tab.cc
        ${SOURCEDIR}/scanner.lex.cc
    )
#target_link_libraries (infix fl)

