# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu> <abiryan@ryandnet>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

cmake_minimum_required(VERSION 2.8)

# Set package properties
project(wiimote-head-tracker-gui-dependencies)

include(ExternalProject)
set(DEPENDENCY_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/install-prefix")
add_subdirectory(fltk)
add_subdirectory(wiiuse)
add_subdirectory(vrpn)
