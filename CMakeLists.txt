cmake_minimum_required (VERSION 2.6)

project (kompilator)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_BINARY_DIR}/cmake/Modules/")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/bin")
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/lib")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/lib")

# domyślne flagi
option (f_add_zero "aktywuj optymalizację dodawania zera" ON)
option (f_add_one "aktywuj optymalizację dodawania jedynki" ON)
option (f_add_inc "aktywuj zamianę dodawania na inkrementacje" ON)
option (f_sub_zero "aktywuj optymalizację odejmowania zera" ON)
option (f_sub_one "aktywuj optymalizację odejmowania jedynki" ON)
option (f_sub_dec "aktywuj zamianę odejmowania na dekrementacje" ON)
option (f_divide_by_zero "aktywuj optymalizację dzielenia przez 0" ON)
option (f_divide_by_two_powers "aktywuj optymalizację dzielenia przez potęgi dwójki" ON)
option (f_multiply_by_zero "aktywuj optymalizację mnożenia przez 0" ON)
option (f_multiply_by_two_powers "aktywuj optymalizację mnożenia przez potęgi dwójki" ON)
option (f_modulo_zero "aktywuj optymalizację modulo zero" ON)
option (f_modulo_two "aktywuj optymalizację modulo dwa" ON)
option (f_cond_eq_zero "aktywuj optymalizację warunku == 0" ON)
option (f_cond_ne_zero "aktywuj optymalizację warunku != 0" ON)
option (f_cond_gt_zero "aktywuj optymalizację warunku > 0" ON)
option (f_const_expr "aktywuj optymalizację stałych wyrażeń" ON)

# budowanie maszyny 
option (BUILD_VM "zbuduj maszynę wirtualną" OFF)
if (BUILD_VM)
	add_subdirectory (vm)	# zbuduj maszynę wirtualną
endif ()

if (NOT CMAKE_BUILD_TYPE)
	set (CMAKE_BUILD_TYPE "release")
endif (NOT CMAKE_BUILD_TYPE)
message (STATUS "Build type: ${CMAKE_BUILD_TYPE}")

find_package (FLEX REQUIRED)
find_package (BISON REQUIRED)
find_package (GMPXX REQUIRED)

set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)
set (VERSION_PATCH 0)
set (VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

set (SOURCEDIR "${CMAKE_SOURCE_DIR}/src")
include_directories (${SOURCEDIR})

configure_file (${SOURCEDIR}/config.hh.in ${SOURCEDIR}/config.hh)

FLEX_TARGET (scanner ${SOURCEDIR}/scanner.l ${SOURCEDIR}/scanner.lex.cc)
BISON_TARGET (parser ${SOURCEDIR}/parser.y ${SOURCEDIR}/parser.tab.cc)

add_library (genvm
		${SOURCEDIR}/code/generate_number.cc
		${SOURCEDIR}/code/copy_value.cc
		${SOURCEDIR}/code/compare_eq.cc
		${SOURCEDIR}/code/compare_ne.cc
		${SOURCEDIR}/code/compare_lt.cc
		${SOURCEDIR}/code/compare_gt.cc
		${SOURCEDIR}/code/compare_le.cc
		${SOURCEDIR}/code/compare_ge.cc		
		${SOURCEDIR}/code/pad_left.cc
		${SOURCEDIR}/code/is_two_power.cc		
		${SOURCEDIR}/code/add.cc
		${SOURCEDIR}/code/subtract.cc
		${SOURCEDIR}/code/multiply.cc
		${SOURCEDIR}/code/divide.cc
		${SOURCEDIR}/code/modulo.cc
	)

add_executable (kompilator
		${SOURCEDIR}/main.cc
        ${FLEX_scanner_OUTPUTS}
        ${BISON_parser_OUTPUTS}
		${SOURCEDIR}/scanner.lex.cc
		${SOURCEDIR}/ISymbolTable.cc
		${SOURCEDIR}/SymbolTable.cc
		${SOURCEDIR}/Commands.cc
		${SOURCEDIR}/Command.cc
	)
target_link_libraries (kompilator
		genvm
		${GMP_LIBRARY}
		${GMPXX_LIBRARIES}
	)

