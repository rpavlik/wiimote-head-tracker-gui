# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu> <abiryan@ryandnet>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

# Patches included:
# XP-lookalike scheme
# - http://svn.easysw.com/public/fltk/applications/trunk/xp_scheme/
# Fl_Gleam 3.0 scheme
# - http://www.xentalion.com/projects/fltk_gleam/


set(FLTK_DIR "${DEPENDENCY_INSTALL_DIR}")
set(FLTK_DIR "${FLTK_DIR}" PARENT_SCOPE)
if(WIN32)
	ExternalProject_Add(fltk
		URL http://ftp2.easysw.com/pub/fltk/1.1.10/fltk-1.1.10-source.tar.gz
		PATCH_COMMAND
			${CMAKE_COMMAND}
			-E
			copy_directory
			"${CMAKE_CURRENT_SOURCE_DIR}/patched-with-fl-gleam"
			<SOURCE_DIR>
		CMAKE_ARGS
			-DCMAKE_BUILD_TYPE=Release
			-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
			-DBUILD_EXAMPLES=OFF
			-DBUILD_TESTING=OFF
			-Wno-dev
		INSTALL_DIR ${DEPENDENCY_INSTALL_DIR}
	)
	
	set(FLTK_BASE_LIBRARY "${FLTK_DIR}/lib/fltk.lib" PARENT_SCOPE)
	set(FLTK_INCLUDE_DIR "${FLTK_DIR}/include" PARENT_SCOPE)
	set(FLTK_FLUID_EXECUTABLE "${FLTK_DIR}/bin/fluid.exe" PARENT_SCOPE)
elseif(APPLE)
	ExternalProject_Add(fltk
		URL http://ftp2.easysw.com/pub/fltk/1.1.10/fltk-1.1.10-source.tar.gz
		PATCH_COMMAND
			${CMAKE_COMMAND}
			-E
			copy_directory
			"${CMAKE_CURRENT_SOURCE_DIR}/patched-with-fl-gleam"
			<SOURCE_DIR>
		CONFIGURE_COMMAND
			"<SOURCE_DIR>/configure"
			--prefix=<INSTALL_DIR>
		BUILD_IN_SOURCE 1
		INSTALL_DIR ${DEPENDENCY_INSTALL_DIR}
	)
	set(FLTK_BASE_LIBRARY "${FLTK_DIR}/lib/libfltk.a" PARENT_SCOPE)
	set(FLTK_INCLUDE_DIR "${FLTK_DIR}/include" PARENT_SCOPE)
	set(FLTK_FLUID_EXECUTABLE "${FLTK_DIR}/bin/fluid" PARENT_SCOPE)
else()
	ExternalProject_Add(fltk
		URL http://ftp2.easysw.com/pub/fltk/1.1.10/fltk-1.1.10-source.tar.gz
		PATCH_COMMAND
			${CMAKE_COMMAND}
			-E
			copy_directory
			"${CMAKE_CURRENT_SOURCE_DIR}/patched-with-fl-gleam"
			<SOURCE_DIR>
		CONFIGURE_COMMAND
			"<SOURCE_DIR>/configure"
			--prefix=<INSTALL_DIR>
			--enable-xft
			--enable-xdbe
			--enable-localjpeg=no
			--enable-localzlib=no
			--enable-localpng=no
		BUILD_IN_SOURCE 1
		INSTALL_DIR ${DEPENDENCY_INSTALL_DIR}
	)
	set(FLTK_BASE_LIBRARY "${FLTK_DIR}/lib/libfltk.a" PARENT_SCOPE)
	set(FLTK_INCLUDE_DIR "${FLTK_DIR}/include" PARENT_SCOPE)
	set(FLTK_FLUID_EXECUTABLE "${FLTK_DIR}/bin/fluid" PARENT_SCOPE)
endif()
