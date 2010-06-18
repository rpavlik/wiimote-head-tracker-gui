
set(LIBS ${FLTK_LIBRARIES})

set(HAVE_CARBON)
if(APPLE)
	include(CheckIncludeFile)
	include(CheckLibraryExists)
	
	find_library(MACOSX_CoreFoundation_FRAMEWORK
		NAMES
		CoreFoundation)
	find_path(MACOSX_CoreFoundation_INCLUDE_DIR
		NAMES
		CoreFoundation/CoreFoundation.h)
		
	find_library(MACOSX_Carbon_FRAMEWORK
		NAMES
		Carbon)
	find_path(MACOSX_Carbon_INCLUDE_DIR
		NAMES
		Carbon/Carbon.h)
	
	find_library(MACOSX_ApplicationServices_FRAMEWORK
		NAMES
		ApplicationServices)
	find_path(MACOSX_ApplicationServices_INCLUDE_DIR
		NAMES
		ApplicationServices/ApplicationServices.h)
		
	mark_as_advanced(MACOSX_CoreFoundation_FRAMEWORK
		MACOSX_CoreFoundation_INCLUDE_DIR
		MACOSX_Carbon_FRAMEWORK
		MACOSX_Carbon_INCLUDE_DIR
		MACOSX_ApplicationServices_FRAMEWORK
		MACOSX_ApplicationServices_INCLUDE_DIR)
	
	set(CMAKE_REQUIRED_LIBRARIES
		${MACOSX_CoreFoundation_FRAMEWORK}
		${MACOSX_Carbon_FRAMEWORK}
		MACOSX_ApplicationServices_FRAMEWORK)
	set(CMAKE_REQUIRED_INCLUDES 
		${MACOSX_CoreFoundation_INCLUDE_DIR}
		${MACOSX_Carbon_INCLUDE_DIR}
		${MACOSX_ApplicationServices_INCLUDE_DIR})
	
	check_include_file("Carbon/Carbon.h" CARBON_INCLUDES)
	check_library_exists(${MACOSX_Carbon_FRAMEWORK}
		NavCreateGetFileDialog
		${MACOSX_Carbon_FRAMEWORK}
		CARBON_COMPILES)
	if(CARBON_INCLUDES) # AND CARBON_COMPILES)
		# TODO: Make this pass when it compiles correctly, and fails when it doesn't
		set(HAVE_CARBON YES)
		list(APPEND LIBS "${MACOSX_CoreFoundation_FRAMEWORK}")
	endif()
endif()

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/FL")
configure_file(FL/Fl_Native_File_Chooser.H.in
	FL/Fl_Native_File_Chooser.H)

# Compile Fl_Native_File_Chooser
add_definitions(-DFLTK1)

set(SOURCES "${CMAKE_CURRENT_BINARY_DIR}/FL/Fl_Native_File_Chooser.H")

if(WIN32)
	list(APPEND SOURCES
		Fl_Native_File_Chooser_WIN32.cxx
		FL/Fl_Native_File_Chooser_WIN32.H)
elseif(HAVE_CARBON)
	list(APPEND SOURCES
		Fl_Native_File_Chooser_MAC.cxx
		FL/Fl_Native_File_Chooser_MAC.H)
else()
	list(APPEND SOURCES
		Fl_Native_File_Chooser_FLTK.cxx
		FL/Fl_Native_File_Chooser_FLTK.H)
endif()

set(FNFC_LIBRARY
	Fl_Native_File_Chooser
	PARENT_SCOPE)
set(FNFC_INCLUDE_DIRS
	"${CMAKE_CURRENT_BINARY_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}"
	PARENT_SCOPE)

include_directories("${CMAKE_CURRENT_BINARY_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}")
add_library(Fl_Native_File_Chooser ${SOURCES})
target_link_libraries(Fl_Native_File_Chooser ${LIBS})
