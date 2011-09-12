#set(FLTK_SOURCES WiimoteTrackerGUI.fl)
set(SOURCES
	mainwindow.cpp
	qtmain.cpp
	VrpnServer.cpp)

set(MOCS
	mainwindow.h
	VrpnServer.h)

set(UIS
	mainwindow.ui)

include(${QT_USE_FILE})


# Let QT pre-process the files and add the generated files to the source list
qt4_wrap_ui(GENERATED_SOURCES ${UIS})
qt4_add_resources(GENERATED_SOURCES ${RESOURCES})
qt4_wrap_cpp(GENERATED_SOURCES ${MOCS})

source_group("Generated Sources - Do Not Edit" FILES ${GENERATED_SOURCES})

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${VRPN_INCLUDE_DIRS} ${QVRPN_INCLUDE_DIRS})

add_executable(wiimoteheadtracker
	WIN32
	MACOSX_BUNDLE
	${SOURCES}
	${GENERATED_SOURCES})
target_link_libraries(wiimoteheadtracker ${VRPN_LIBRARIES} ${QVRPN_LIBRARIES} ${QT_LIBRARIES})

if(APPLE)
	set_target_properties(wiimoteheadtracker
		PROPERTIES
		OUTPUT_NAME "Wii Remote Head Tracker")
endif()

install(TARGETS wiimoteheadtracker
		RUNTIME DESTINATION bin
		BUNDLE DESTINATION ./)
