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
# Locally-developed modules dist'ed with this app - always have this first.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(UseBackportedModules)

option(USE_LOCAL_DEPENDENCIES "Whether we should look first in our local deps dir." ON)

if(NOT DEPENDENCIES_INSTALL_DIR)
	set(DEPENDENCIES_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/install-prefix")
endif()

set(DEPENDENCIES_INSTALL_DIR "${DEPENDENCIES_INSTALL_DIR}" CACHE PATH "Where the dependencies folder installed to - the default is usually correct.")
if(USE_LOCAL_DEPENDENCIES)
	list(APPEND CMAKE_PREFIX_PATH "${DEPENDENCIES_INSTALL_DIR}")
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


set(EXTRA_LIBS ${FLTK_LIBRARIES} ${VRPN_SERVER_LIBRARIES} ${WIIUSE_LIBRARIES})
include_directories(${FLTK_INCLUDE_DIR} ${VRPN_INCLUDE_DIRS})

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
