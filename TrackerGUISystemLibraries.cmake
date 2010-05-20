# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu> <abiryan@ryandnet>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

if(NOT BUILD_OWN_DEPENDENCIES AND NOT WIN32 AND NOT APPLE)
		find_package(WiiUse REQUIRED)
endif()
# Don't find Fluid.app the Mac single-site browser
# when we want fluid the FLTK GUI designer/pre-compiler
set(CMAKE_FIND_APPBUNDLE NEVER)

set(FLTK_SKIP_OPENGL TRUE)
set(FLTK_SKIP_FORMS TRUE)
set(FLTK_SKIP_IMAGES TRUE)
find_package(FLTK REQUIRED)

find_package(VRPN REQUIRED)


