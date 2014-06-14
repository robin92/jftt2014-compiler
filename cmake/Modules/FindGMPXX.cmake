# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.
# NOTE: GMP prefix is understood to be the path to the root of the GMP
# installation library.

find_package (GMP REQUIRED)

set(GMPXX_PREFIX "" CACHE PATH "The path to the prefix of a GMPXX installation")

find_path(GMPXX_INCLUDE_DIR gmpxx.h 
		PATHS ${GMPXX_PREFIX}/include /usr/include /usr/local/include
	)

find_library(GMPXX_LIBRARY NAMES gmpxx 
		PATHS ${GMPXX_PREFIX}/lib /usr/lib /usr/local/lib
	)


if(GMPXX_INCLUDE_DIR AND GMPXX_LIBRARY)
	get_filename_component(GMPXX_LIBRARY_DIR ${GMPXX_LIBRARY} PATH)
	set(GMPXX_FOUND TRUE)
endif()

if(GMPXX_FOUND)
	MESSAGE(STATUS "Found GMPXX: ${GMPXX_LIBRARY}")
else()
   	if(GMPXX_FIND_REQUIRED)
   		message (STATUS "GMPXX Include Dir: ${GMPXX_INCLUDE_DIR}")
      		message(FATAL_ERROR "Could not find GMPXX")
   	endif()
endif()
