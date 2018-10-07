set(PROJECT_NAME "LearnOpenGL")
#---------------------------------------
macro(SETUP_PROJECT MODE TARGET_PARTNAME STR_TARGET_SOURCES STR_TARGET_LIBS)
	set( TARGET_NAME ${TARGET_PREFIX}${TARGET_PARTNAME})
	string(REPLACE " " ";" LIST_TARGET_SOURCES ${STR_TARGET_SOURCES})
	string(REPLACE " " ";" LIST_TARGET_LIBS ${STR_TARGET_LIBS})
	
	if(${STR_TARGET_SOURCES} STREQUAL " ")
		message(WARNING "Target [${TARGET_NAME}] has no source, so it won't be generated.")
	else()
		if(${MODE} STREQUAL "EXE")
			add_executable( ${TARGET_NAME} ${LIST_TARGET_SOURCES})
			set(INSTALL_DIR "bin/${FOLDER_NAME}/${TARGET_PARTNAME}")
		elseif(${MODE} STREQUAL "LIB")
			add_library(${TARGET_NAME} ${LIST_TARGET_SOURCES})
			set(INSTALL_DIR "lib/Gen")
		else()
			message(FATAL_ERROR "Mode [${MODE}] is not supported, so target [TARGET_NAME] is not generated!")
			set(MODE_NOTSUPPORT " ")
		endif()
		if(NOT DEFINED MODE_NOTSUPPORT)
			if( NOT ${FOLDER_NAME} STREQUAL " ")
				SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES FOLDER ${FOLDER_NAME})
			endif()
			if(NOT ${STR_TARGET_LIBS} STREQUAL " ")
				target_link_libraries( ${TARGET_NAME} ${LIST_TARGET_LIBS} )
			endif()
			install (TARGETS ${TARGET_NAME} DESTINATION ${INSTALL_DIR})
			message(STATUS "Setup Target [${TARGET_NAME}] success")
		endif()
	endif()
endmacro(SETUP_PROJECT TARGET_PARTNAME STR_TARGET_SOURCES STR_TARGET_LIBS)
#---------------------------------------
macro(ADD_ALL_SUBDIR)
	file(GLOB _children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
	set(_dirlist "")
	foreach(_child ${_children})
		if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${_child})
			list(APPEND _dirlist ${_child})
		endif()
	endforeach()
	#SET(${result} ${dirlist})
	foreach(_subdir ${_dirlist})
		add_subdirectory(${_subdir})
	endforeach()
endmacro(ADD_ALL_SUBDIR)
#---------------------------------------
macro(GET_DIR_NAME DIRNAME)
	string(REGEX MATCH "([^/]*)$" TMP ${CMAKE_CURRENT_SOURCE_DIR})
	set(${DIRNAME} ${TMP})
endmacro(GET_DIR_NAME DIRNAME)
#---------------------------------------

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

cmake_minimum_required (VERSION 2.8)
cmake_policy(VERSION 2.8)
project (${PROJECT_NAME})

# The version number.
set (${PROJECT_NAME}_VERSION_MAJOR 1)
set (${PROJECT_NAME}_VERSION_MINOR 0)


set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

configure_file(config/ROOT_PATH.h.in config/ROOT_PATH.h)
message(STATUS "${CMAKE_BINARY_DIR}/config")
include_directories (
	${PROJECT_SOURCE_DIR}/include
	${CMAKE_BINARY_DIR}/config
	# $ENV{XXX_INCLUDE_PATH}
	# ...
)
link_directories (
	${PROJECT_SOURCE_DIR}/lib
	${PROJECT_SOURCE_DIR}/lib/Gen
	# $ENV{XXX_LIB_PATH}
	# ...
)

add_subdirectory (src)