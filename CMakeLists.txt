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
# Options
###

option(USE_LOCAL_DEPENDENCIES "Whether we should look first in our local deps dir." ON)

if(WIN32)
	set(INSTALL_WIIUSE_LIBRARY ON)
	set(RUNTIME_LIBRARY_INSTALL_DIR bin)
else()
	option(INSTALL_WIIUSE_LIBRARY "Whether we should install the WiiUse shared library despite being on a platform that should know better." OFF)
	set(RUNTIME_LIBRARY_INSTALL_DIR lib)
endif()

if(NOT DEPENDENCIES_INSTALL_DIR)
	set(DEPENDENCIES_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/install-prefix")
endif()
set(DEPENDENCIES_INSTALL_DIR "${DEPENDENCIES_INSTALL_DIR}" CACHE PATH "Where the dependencies folder installed to - the default is usually correct.")
mark_as_advanced(DEPENDENCIES_INSTALL_DIR)

###
# Perform build configuration of dependencies
###

if(INSTALL_WIIUSE_LIBRARY)
	# Always point to WiiUse, for safety's sake
	set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${RUNTIME_LIBRARY_INSTALL_DIR}")
endif()

# Locally-developed modules dist'ed with this app - always have this first.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(UseBackportedModules)

if(USE_LOCAL_DEPENDENCIES)
	list(APPEND CMAKE_PREFIX_PATH "${DEPENDENCIES_INSTALL_DIR}")
	set(FLTK_DIR "${DEPENDENCIES_INSTALL_DIR}")
endif()

# Don't find Fluid.app the Mac single-site browser
# when we want fluid the FLTK GUI designer/pre-compiler
set(CMAKE_FIND_APPBUNDLE NEVER)
find_package(FLTK REQUIRED)
if(USE_LOCAL_DEPENDENCIES AND NOT APPLE AND NOT WIN32)
	list(APPEND FLTK_LIBRARIES Xft png jpeg)
endif()

find_package(VRPN REQUIRED)

if(NOT APPLE)
	find_package(WiiUse REQUIRED)
endif()

# Try to detect linux distribution for better theme selection
if(NOT WIN32 AND NOT APPLE)
	execute_process(COMMAND lsb_release -d -s
		OUTPUT_VARIABLE _distroID
		OUTPUT_STRIP_TRAILING_WHITESPACE)
	if(_distroID MATCHES ".*Red Hat.*")
		add_definitions(-D_BUILT_ON_REDHAT)
	elseif(_distroID MATCHES ".*Ubuntu.*")
		add_definitions(-D_BUILT_ON_UBUNTU)
	endif()
endif()

###
# Build the project
###
set(EXTRA_LIBS ${FLTK_LIBRARIES} ${VRPN_SERVER_LIBRARIES} ${WIIUSE_LIBRARIES})
include_directories(${FLTK_INCLUDE_DIR} ${VRPN_INCLUDE_DIRS})

# Native file chooser
add_subdirectory(third-party/Fl_Native_File_Chooser-0.86)
list(APPEND EXTRA_LIBS ${FNFC_LIBRARY})
include_directories(${FNFC_INCLUDE_DIRS})

# The app is in the "src" subdirectory
add_subdirectory(src)

if(INSTALL_WIIUSE_LIBRARY)
	# Install the WiiUse DLL
	install(FILES ${WIIUSE_RUNTIME_LIBRARY}
		DESTINATION ${RUNTIME_LIBRARY_INSTALL_DIR})
endif()

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
SET(CPACK_NSIS_MENU_LINKS
    "jconfs/README-jconf.txt" "jconf README"
	"jconfs/" "jconf folder"
    "doc/README.txt" "README"
	"http://www.github.com/rpavlik/wiimote-head-tracker-gui/" "Project homepage"
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
