set(FLTK_SOURCES WiimoteTrackerGUI.fl)
set(SOURCES
	main.cpp
	SoftwareVersions.h
	SystemComponents.h
	WiimoteTracker.cpp
	WiimoteTracker.h
	WiimoteTrackerView.cpp
	WiimoteTrackerView.h)

fltk_wrap_ui(wiimoteheadtracker ${FLTK_SOURCES})

include_directories(.)
add_executable(wiimoteheadtracker ${SOURCES} ${wiimoteheadtracker_FLTK_UI_SRCS})
target_link_libraries(wiimoteheadtracker ${EXTRA_LIBS})
