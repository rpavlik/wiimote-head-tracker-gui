# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu> <abiryan@ryandnet>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

if(WIN32)
	ExternalProject_Add(wiiuse
		URL http://github.com/downloads/rpavlik/vrpn/wiiuse_v0.12_win.tar.gz
		CONFIGURE_COMMAND ""
		BUILD_COMMAND ""
		INSTALL_DIR "${DEPENDENCY_INSTALL_DIR}"
		INSTALL_COMMAND ${CMAKE_COMMAND}
			-E copy_directory ./ "<INSTALL_DIR>/wiiuse"
	)
	set(WIIUSE_ROOT_DIR "${DEPENDENCY_INSTALL_DIR}/wiiuse/" PARENT_SCOPE)
	set(WIIUSE_LIBRARY "${DEPENDENCY_INSTALL_DIR}/wiiuse/wiiuse.lib" PARENT_SCOPE)
	set(WIIUSE_LIBRARIES "${WIIUSE_LIBRARY}" PARENT_SCOPE)
	set(WIIUSE_RUNTIME_LIBRARY "${DEPENDENCY_INSTALL_DIR}/wiiuse/wiiuse.dll" PARENT_SCOPE)
	set(WIIUSE_INCLUDE_DIR "${DEPENDENCY_INSTALL_DIR}/wiiuse" PARENT_SCOPE)
else()
	execute_process(COMMAND sh -c
		"${CMAKE_C_COMPILER} -v 2>&1|grep ^Target:|cut -d' ' -f2"
		OUTPUT_VARIABLE _platformString
		OUTPUT_STRIP_TRAILING_WHITESPACE)
	set(_objdir release-${_platformString})
	
	set(PATCHARGS)
	if(NOT APPLE)
		# See if we need the patch
		include(CheckStructHasMember)
		check_struct_has_member(sockaddr_l2 l2_cid <bluetooth/l2cap.h> SOCKADDRL2_HAS_L2CID)
		if(SOCKADDRL2_HAS_L2CID)
			set(PATCHARGS PATCH_COMMAND patch
			-p0
			-i ${CMAKE_CURRENT_SOURCE_DIR}/fix-wiiuse-kernel-2.6.30.diff)
		endif()
	endif()
	
	ExternalProject_Add(wiiuse
		URL http://iweb.dl.sourceforge.net/project/wiiuse/wiiuse/v0.12/wiiuse_v0.12_src.tar.gz
		CONFIGURE_COMMAND ""
		${PATCHARGS}
		BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} wiiuse
		BUILD_IN_SOURCE 1
		INSTALL_DIR ${DEPENDENCY_INSTALL_DIR}
		INSTALL_COMMAND ""	
	)

	ExternalProject_Add_Step(wiiuse install_header
		COMMAND "${CMAKE_COMMAND}"
			-E copy "<SOURCE_DIR>/src/wiiuse.h" "<INSTALL_DIR>/include/"
		DEPENDEES build
		DEPENDERS install
		WORKING_DIRECTORY <BINARY_DIR>)

	ExternalProject_Add_Step(wiiuse install_lib
		COMMAND ${CMAKE_COMMAND}
			-E copy "<SOURCE_DIR>/src/${_objdir}/libwiiuse.so" "<INSTALL_DIR>/lib/"	
		DEPENDEES build
		DEPENDERS install
		WORKING_DIRECTORY <BINARY_DIR>)
		
	set(WIIUSE_ROOT_DIR "${DEPENDENCY_INSTALL_DIR}" PARENT_SCOPE)
	set(WIIUSE_LIBRARY "${DEPENDENCY_INSTALL_DIR}/lib/libwiiuse.so" PARENT_SCOPE)
	set(WIIUSE_LIBRARIES "${WIIUSE_LIBRARY}" PARENT_SCOPE)
	set(WIIUSE_RUNTIME_LIBRARY "${DEPENDENCY_INSTALL_DIR}/lib/libwiiuse.so" PARENT_SCOPE)
	set(WIIUSE_INCLUDE_DIR "${DEPENDENCY_INSTALL_DIR}/include" PARENT_SCOPE)
endif()
