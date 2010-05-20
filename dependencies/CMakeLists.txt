# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu> <abiryan@ryandnet>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

cmake_minimum_required(VERSION 2.8)

# Set package properties
project(wiimote-head-tracker-gui-dependencies)

# Locally-developed modules dist'ed with this app - always have this first.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)
include(UseBackportedModules)

include(ExternalProject)
set(DEPENDENCY_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/install-prefix")
add_subdirectory(fltk)
add_subdirectory(wiiuse)
add_subdirectory(vrpn)
