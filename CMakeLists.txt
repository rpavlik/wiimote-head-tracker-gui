# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu> <abiryan@ryandnet>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

cmake_minimum_required(VERSION 2.6)

# Set package properties
project(wiimote-head-tracker-gui)

set(CPACK_PACKAGE_VENDOR "Iowa State University")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VERSION
	"${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-src")

###
# Set up options
###

###
# Perform build configuration of dependencies
###

# Locally-developed modules dist'ed with this app - always have this first.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(UseBackportedModules)

find_package(FLTK REQUIRED)
find_package(VRPN REQUIRED)
if(NOT APPLE)
	find_package(WiiUse REQUIRED)
endif()

# Link these libraries
set(EXTRA_LIBS ${FLTK_LIBRARIES} ${VRPN_SERVER_LIBRARIES} ${WIIUSE_LIBRARIES})


include_directories(${FLTK_INCLUDE_DIR} ${VRPN_INCLUDE_DIRS})

###
# Build the project
###

# The app is in the "src" subdirectory
add_subdirectory(src)

if(WIN32)
	# Install the DLL
	install(FILES ${WIIUSE_RUNTIME_LIBRARY}
		DESTINATION bin)
endif()

###
# Set packaging options (for CPack)
###

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
