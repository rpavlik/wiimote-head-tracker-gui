#set(FLTK_SOURCES WiimoteTrackerGUI.fl)
#set(SOURCES
#	FromString.h
#	main.cpp
#	launchByAssociation.h
#	SoftwareVersions.h
#	SystemComponents.h
#	TrackerConfiguration.h
#	TrackerConfiguration.cpp
#	WiimoteTracker.cpp
#	WiimoteTracker.h
#	WiimoteTrackerView.cpp
#	WiimoteTrackerView.h)

#fltk_wrap_ui(wiimoteheadtracker ${FLTK_SOURCES})

#include_directories(.)
#add_executable(wiimoteheadtracker
#	WIN32
#	MACOSX_BUNDLE
#	${SOURCES}
#	${wiimoteheadtracker_FLTK_UI_SRCS})
#target_link_libraries(wiimoteheadtracker ${EXTRA_LIBS})

#if(APPLE)
#	set_target_properties(wiimoteheadtracker
#		PROPERTIES
#		OUTPUT_NAME "Wii Remote Head Tracker")
#endif()

#install(TARGETS wiimoteheadtracker
#		RUNTIME DESTINATION bin
#		BUNDLE DESTINATION ./)
