cmake_minimum_required(VERSION 3.12)
add_definitions("-fPIC")

set(CMAKE_CXX_STANDARD 17)
set(VERBOSE)

if (UNIX)
	set(THREADLIB pthread)
elseif(WIN32)
	set(THREADLIB "")
endif(UNIX)

#CONAN

function(configure_conan DIR)
	if(NOT EXISTS ${conan})
		message(FATAL_ERROR "Conan is required.")
	endif()

	message(STATUS "Running conan in " ${DIR})

	execute_process(COMMAND ${conan} install --build=missing ${DIR} RESULT_VARIABLE return_code)

	if(NOT ${return_code} EQUAL 0)
		message(FATAL_ERROR "conan install command failed.")
	endif()

	include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	conan_basic_setup()
	include_directories(${CMAKE_INCLUDE_PATH} ${CONAN_INCLUDE_DIRS} )#include

	set(DEPENDENCIES ${DEPENDENCIES} ${CONAN_LIBS} PARENT_SCOPE)
endfunction(configure_conan)


# include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
# conan_basic_setup()
# include_directories(${CMAKE_INCLUDE_PATH} ${CONAN_INCLUDE_DIRS} include)

configure_conan(${CMAKE_CURRENT_LIST_DIR})

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

#SOURCE FETCHERS

function(get_include LISTNAME)
	set(get_include_dir_local "")
	foreach(dir ${ARGN})
		list(APPEND get_include_dir_local ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/include)
	endforeach(dir)
	set(${LISTNAME} "${get_include_dir_local}" PARENT_SCOPE)
	set(PROJECT_INCLUDE_DIRS ${PROJECT_INCLUDE_DIRS} ${get_include_dir_local} PARENT_SCOPE)
endfunction(get_include)

function(get_src LISTNAME)
	# build list of globbing expression
	set(get_src_dir_local "")
	foreach(dir ${ARGN})
		list(APPEND get_src_dir_local "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/src/*.cpp")
	endforeach(dir)

	# build list of source files
	set(get_src_local "")

	foreach(src ${get_src_dir_local})

		# get list of source for a globbing expression
		file(GLOB get_src_globbed_local "${src}")

		# add the globbed source files to the list
		list(APPEND get_src_local "${get_src_globbed_local}")

	endforeach(src)

	# return the completed list of source files in ${LISTNAME}
	set(${LISTNAME} "${get_src_local}" PARENT_SCOPE)

endfunction(get_src)


#TMP

option(BUILD_EX "Build exemple binaries" OFF)

if (BUILD_EX)
	message(STATUS "Configuring exemple Build")
	get_include(CLIENT_INCLUDE exemple/client exemple/common)
	get_src(CLIENT_SRC exemple/client exemple/common)
	get_include(SERVER_INCLUDE exemple/server exemple/common)
	get_src(SERVER_SRC exemple/server exemple/common)
	include_directories(${CLIENT_INCLUDE} ${SERVER_INCLUDE})
	add_executable(exemple_client ${CLIENT_SRC})
	add_executable(exemple_server ${SERVER_SRC})
	target_link_libraries(exemple_client ${THREADLIB} ${CONANLIBS})
	target_link_libraries(exemple_server ${THREADLIB} ${CONANLIBS})
endif(BUILD_EX)



#DOXYGEN

# check if Doxygen is installed
find_package(Doxygen)

# indicate the documentation build as an option
option(BUILD_DOC "Build documentation" ${DOXYGEN_FOUND})

if (${BUILD_DOC} AND ${DOXYGEN_FOUND})
	# set input and output files
	set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
	set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

	# request to configure the file
	configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
	message(STATUS "Doxygen build started")

	add_custom_target(docs
		COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation with Doxygen"
		VERBATIM
	)
	message(STATUS "Doxygen build complete")
elseif(${BUILD_DOC})
	message("Doxygen need to be installed to generate the doxygen documentation")
endif (${BUILD_DOC} AND ${DOXYGEN_FOUND})
