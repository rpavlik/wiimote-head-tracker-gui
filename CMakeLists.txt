# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu> <abiryan@ryandnet>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

cmake_minimum_required(VERSION 2.6)

# Set package properties
project(wiimote-head-tracker-gui)

set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0pre")
set(CPACK_PACKAGE_VERSION
	"${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

add_definitions("-DTRACKER_APP_VERSION=\"${CPACK_PACKAGE_VERSION}\"")

###
# Perform build configuration of dependencies
###
if(NOT "${CMAKE_VERSION}" VERSION_LESS 2.8.0)
	option(BUILD_OWN_DEPENDENCIES "Whether we should build our own dependencies rather than using system-wide ones." ON)
endif()

# Locally-developed modules dist'ed with this app - always have this first.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(UseBackportedModules)

# If we are using our own library builds, include
# the CMake script to build them.
if(BUILD_OWN_DEPENDENCIES)
	include(TrackerGUIExternalProjects.cmake)
endif()

include(TrackerGUISystemLibraries.cmake)

set(EXTRA_LIBS ${FLTK_LIBRARIES} ${VRPN_SERVER_LIBRARIES} ${WIIUSE_LIBRARIES})
include_directories(${FLTK_INCLUDE_DIR} ${VRPN_INCLUDE_DIRS})

###
# Build the project
###

# Native file chooser
add_subdirectory(third-party/Fl_Native_File_Chooser-0.86)
list(APPEND EXTRA_LIBS ${FNFC_LIBRARY})
include_directories(${FNFC_INCLUDE_DIRS})

# The app is in the "src" subdirectory
add_subdirectory(src)

if(WIN32)
	# Install the WiiUse DLL
	install(FILES ${WIIUSE_RUNTIME_LIBRARY}
		DESTINATION bin)
endif()

# Install README and licenses
install(FILES
	LICENSE_1_0.txt
	README.txt
	DESTINATION
	doc)
install(DIRECTORY
	licenses
	DESTINATION
	doc)

###
# Set packaging options (for CPack)
###
set(CPACK_PACKAGE_NAME "Wii Remote Head Tracker GUI")
set(CPACK_PACKAGE_VENDOR "Iowa State University Virtual Reality Applications Center")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-src")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/all_licenses_for_installer.txt")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.txt")
set(CPACK_PACKAGE_EXECUTABLES wiimoteheadtracker "Wii Remote Head Tracker")

# Choose desired package generators
if(APPLE)
	set(CPACK_GENERATOR DragNDrop)
	set(CPACK_SOURCE_GENERATOR ZIP)
elseif(WIN32)
	set(CPACK_SOURCE_GENERATOR ZIP)
else()
	set(CPACK_SOURCE_GENERATOR TARGZ)
endif()

# Include the packaging system now that we have it all set up
include(CPack)
