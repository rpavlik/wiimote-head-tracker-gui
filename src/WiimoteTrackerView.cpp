/**	@file	WiimoteTrackerView.cpp
	@brief	View implementation

	@date	2010

	@author
	Ryan Pavlik ( <rpavlik@iastate.edu> http://academic.cleardefinition.com/ ),
	Iowa State University
	Virtual Reality Applications Center and
	Human-Computer Interaction Graduate Program
*/

// Internal Includes
#include "WiimoteTrackerView.h"

#include "WiimoteTracker.h"

#include <WiimoteTrackerGUI.h>

// Library/third-party includes
// - none

// Standard includes
#include <cassert>

WiimoteTrackerView::WiimoteTrackerView(WiimoteTracker * controller) :
		_progress(new StartupProgress(430,290, "Starting Tracking System...")),
		_config(new WiimoteTrackerConfigGUI(350, 160, "Tracker Configuration")),
		_gui(new WiimoteTrackerGUI(520,560, "Wii Remote Head Tracker")),
		_controller(controller) {
	assert(_progress);
	assert(_config);
	assert(_gui);
	assert(_controller);
}

WiimoteTrackerView::~WiimoteTrackerView() {
	delete _progress;
	_progress = NULL;

	delete _config;
	_config = NULL;

	delete _gui;
	_gui = NULL;
}

void WiimoteTrackerView::setLEDDistance(const float distanceInMeters) {
	if (distanceInMeters <= 0) {
		// Invalid change
		_config->setLEDDistance(_ledDistance);
		return;
	}
	// This invalidates the tracker
	teardownTrackerDevice();
	_ledDistance = distanceInMeters;
	_config->setLEDDistance(_ledDistance);
}

void WiimoteTrackerView::setTrackerName(const std::string & trackerName) {
	if (trackerName.size() == 0 || trackerName.find(" ") != std::string::npos) {
		// Invalid change
		_config->setTrackerName(_trackerName.c_str());
		return;
	}
	// This invalidates the tracker
	teardownTrackerDevice();
	_trackerName = trackerName;
	_config->setTrackerName(_trackerName.c_str());
}

void WiimoteTrackerView::run() {
	
	// Set tracker pointers in GUI
	_config->setTracker(_controller, this);
	_gui->setTracker(_controller, this);
	_progress->setTracker(_controller, this);
	
	// Prepare the main window
	_gui->updateVersions();
	
	// Show initial windows
	_gui->show();
	_progress->show();
	
	refresh_ui();
}

void WiimoteTrackerView::setProgress(const StartupStages cmp, const float completion, const char * message) {
	switch (cmp) {
		case CMP_CONNECTION:

		break;

		case CMP_WIIMOTE:

		break;

		case CMP_TRACKER:

		break;

		case CMP_CLIENT:

		break;
	}

}
/*
if (!_connection) {
	if (updateProgress) {
		_progress->setConnectionStatus(0.2, "Could not create server connection!", true);
		refresh_ui();
	}
	return false;
}

if (updateProgress) {
	_progress->setConnectionStatus(1, "Running");
}

if (!_wiimote) {
	// error condition creating wiimote
	if (updateProgress) {
		_progress->setWiimoteStatus(0.2, "Could not allocate Wiimote device!", true);
		refresh_ui();
	}
	return false;
}

if (!_wiimote->isValid()) {
	if (updateProgress) {
		_progress->setWiimoteStatus(0.3, "Could not connect to a Wiimote!", true);
		refresh_ui();
	}
	return false;
}

if (updateProgress) {
	_progress->setWiimoteStatus(1, "Running");
}

if (!_tracker) {
	// error condition creating tracker device
	if (updateProgress) {
		_progress->setTrackerStatus(0.2, "Could not allocate tracker device!", true);
		refresh_ui();
	}
	return false;
}

if (updateProgress) {
	_progress->setTrackerStatus(1, "Running");
	_gui->setStatus(true);
	_progress->scheduleClose(PROGRESS_WINDOW_TIMEOUT);
	refresh_ui();
}
*/

void WiimoteTrackerView::systemIsDown() {
	_gui->setStatus(false);

}

void WiimoteTrackerView::systemIsUp() {
	_gui->setStatus(true);
	if (_progress->visible()) {
		_progress->scheduleClose(PROGRESS_WINDOW_TIMEOUT);
	}
	mainloop_ui(PROGRESS_EVENT_TIMEOUT);
}

bool WiimoteTrackerView::processView(bool wait) {
	return mainloop_ui(wait ? PROGRESS_EVENT_TIMEOUT : 0);
}