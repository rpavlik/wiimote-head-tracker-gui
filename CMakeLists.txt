# 2009-2011 Ryan Pavlik <rpavlik@iastate.edu> <abiryan@ryandnet>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

cmake_minimum_required(VERSION 2.6)

# Set package properties
project(wiimote-head-tracker-gui)

set(CPACK_PACKAGE_VERSION_MAJOR "2")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VERSION
	"${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

add_definitions("-DTRACKER_APP_VERSION=\"${CPACK_PACKAGE_VERSION}\"")

###
# Options
###



###
# Perform build configuration of dependencies
###

set(BUILD_WIIUSE_SHARED_LIB FALSE)
add_subdirectory(wiiuse)
include_directories(${WIIUSE_INCLUDE_DIRS})

set(VRPN_USE_WIIUSE TRUE CACHE INTERNAL "" FORCE)
set(VRPN_GPL_SERVER ON CACHE INTERNAL "" FORCE)
set(VRPN_BUILD_SERVER_LIBRARY TRUE CACHE INTERNAL "" FORCE)
add_subdirectory(vrpn)

set(VRPN_INCLUDE_DIRS "${CMAKE_CURRENT_BINARY_DIR}/vrpn" "${CMAKE_CURRENT_SOURCE_DIR}/vrpn" "${CMAKE_CURRENT_SOURCE_DIR}/vrpn/quat")
set(VRPN_LIBRARIES vrpnserver)
set(VRPN_FOUND TRUE)
add_definitions(-DVRPN_USING_CMAKE)

add_subdirectory(qvrpn)

# Locally-developed modules dist'ed with this app - always have this first.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(UseBackportedModules)

###
# Build the project
###

# The app is in the "src" subdirectory
add_subdirectory(src)

# Install README
install(FILES
	LICENSE_1_0.txt
	README.txt
	DESTINATION
	doc)

# Install licenses
install(DIRECTORY
	licenses
	DESTINATION
	doc)

# Install jconfs
install(DIRECTORY
	jconfs
	DESTINATION
	./)

include(InstallRequiredSystemLibraries)

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
set(CPACK_NSIS_MENU_LINKS
    "jconfs/README-jconf.txt" "jconf README"
	"jconfs/" "jconf folder"
    "doc/README.txt" "README"
	"http://rpavlik.github.com/wiimote-head-tracker-gui/" "Project homepage"
	"http://academic.cleardefinition.com/" "Creator's homepage"
	"http://www.vrac.iastate.edu/" "Iowa State University Virtual Reality Applications Center")

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
