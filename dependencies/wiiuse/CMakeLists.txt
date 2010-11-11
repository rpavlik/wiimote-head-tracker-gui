# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu> <abiryan@ryandnet>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

if(WIN32)
	ExternalProject_Add(wiiuse
		URL http://www.hci.iastate.edu/~rpavlik/downloads/wiiuse/WiiUse-0.13.1-vs9.zip
		CONFIGURE_COMMAND ""
		BUILD_COMMAND ""
		INSTALL_DIR "${DEPENDENCY_INSTALL_DIR}"
		INSTALL_COMMAND ""
	)
	
	ExternalProject_Add_Step(wiiuse mkdir_header
		COMMAND "${CMAKE_COMMAND}"
			-E make_directory "<INSTALL_DIR>/include/")

	ExternalProject_Add_Step(wiiuse mkdir_lib
		COMMAND "${CMAKE_COMMAND}"
			-E make_directory "<INSTALL_DIR>/lib/")
			
	ExternalProject_Add_Step(wiiuse mkdir_dll
		COMMAND "${CMAKE_COMMAND}"
			-E make_directory "<INSTALL_DIR>/bin/")
	
	ExternalProject_Add_Step(wiiuse install_header
		COMMAND "${CMAKE_COMMAND}"
			-E copy_directory "<SOURCE_DIR>/include" "<INSTALL_DIR>/include/"
		DEPENDEES build mkdir_header
		DEPENDERS install
		WORKING_DIRECTORY <BINARY_DIR>)

	ExternalProject_Add_Step(wiiuse install_lib
		COMMAND ${CMAKE_COMMAND}
			-E copy "<SOURCE_DIR>/lib/wiiuse.lib" "<INSTALL_DIR>/lib/"	
		DEPENDEES build mkdir_lib
		DEPENDERS install
		WORKING_DIRECTORY <BINARY_DIR>)
	
	ExternalProject_Add_Step(wiiuse install_dll
		COMMAND ${CMAKE_COMMAND}
			-E copy "<SOURCE_DIR>/bin/wiiuse.dll" "<INSTALL_DIR>/bin/"	
		DEPENDEES build mkdir_dll
		DEPENDERS install
		WORKING_DIRECTORY <BINARY_DIR>)
	
	set(WIIUSE_ROOT_DIR "${DEPENDENCY_INSTALL_DIR}/" PARENT_SCOPE)
	set(WIIUSE_LIBRARY "${DEPENDENCY_INSTALL_DIR}/lib/wiiuse.lib" PARENT_SCOPE)
	set(WIIUSE_LIBRARIES "${WIIUSE_LIBRARY}" PARENT_SCOPE)
	set(WIIUSE_RUNTIME_LIBRARY "${DEPENDENCY_INSTALL_DIR}/bin/wiiuse.dll" PARENT_SCOPE)
	set(WIIUSE_INCLUDE_DIR "${DEPENDENCY_INSTALL_DIR}/include" PARENT_SCOPE)
else()	
	ExternalProject_Add(wiiuse
		URL http://www.hci.iastate.edu/~rpavlik/downloads/wiiuse/wiiuse-0.13.1.tar.gz
		CMAKE_ARGS -DBUILD_EXAMPLE=NO -DBUILD_EXAMPLE_SDL=NO -DINSTALL_EXAMPLES=NO -DCMAKE_BUILD_TYPE=Release
		INSTALL_DIR ${DEPENDENCY_INSTALL_DIR}
	)

	set(WIIUSE_ROOT_DIR "${DEPENDENCY_INSTALL_DIR}" PARENT_SCOPE)
	set(WIIUSE_LIBRARY "${DEPENDENCY_INSTALL_DIR}/lib/libwiiuse.so" PARENT_SCOPE)
	set(WIIUSE_LIBRARIES "${WIIUSE_LIBRARY}" PARENT_SCOPE)
	set(WIIUSE_RUNTIME_LIBRARY "${DEPENDENCY_INSTALL_DIR}/lib/libwiiuse.so" PARENT_SCOPE)
	set(WIIUSE_INCLUDE_DIR "${DEPENDENCY_INSTALL_DIR}/include" PARENT_SCOPE)
endif()
