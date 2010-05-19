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
#include <iostream>
#include <sstream>

WiimoteTrackerView::WiimoteTrackerView(WiimoteTracker * controller) :
		_progress(new StartupProgress(430,360, "Starting Tracking System...")),
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
		_config->setLEDDistance(_controller->_ledDistance);
		return;
	}
	// This invalidates the tracker
	_controller->teardownTrackerDevice();
	_controller->_ledDistance = distanceInMeters;
	_config->setLEDDistance(_controller->_ledDistance);
}

void WiimoteTrackerView::setTrackerName(const std::string & trackerName) {
	if (trackerName.size() == 0 || trackerName.find(" ") != std::string::npos) {
		// Invalid change
		_config->setTrackerName(_controller->_trackerName.c_str());
		return;
	}
	// This invalidates the tracker
	_controller->teardownTrackerDevice();
	_controller->_trackerName = trackerName;
	_config->setTrackerName(_controller->_trackerName.c_str());
}

void WiimoteTrackerView::run() {

	// Set tracker pointers in GUI
	_config->setTracker(_controller, this);
	_gui->setTracker(_controller, this);
	_progress->setTracker(_controller, this);



	// Prepare the main window
	_gui->updateVersions();

	// Update the message about the report stride
	std::stringstream s;
	s << "Updated every " << REPORT_STRIDE << " reports";
	_gui->_updateGroup->copy_label(s.str().c_str());

	// Show initial windows
	_gui->show();
	_progress->show();

	refresh_ui();
}


void WiimoteTrackerView::reconfigure() {
	_config->show();
	refresh_ui();
}

void WiimoteTrackerView::setProgressRanges() {
	_progress->_connectionProgress->minimum(0.0);
	_progress->_connectionProgress->maximum(1.0);

	_progress->_wiimoteProgress->minimum(0.0);
	_progress->_wiimoteProgress->maximum(1.0);

	_progress->_trackerProgress->minimum(0.0);
	_progress->_trackerProgress->maximum(1.0);

	_progress->_clientProgress->minimum(0.0);
	_progress->_clientProgress->maximum(1.0);
}

void WiimoteTrackerView::setProgress(const StartupStage stg) {
	_progress->show();

	// Use switch statement and intentional fall-through to
	// reset all later components to "not started"
	switch (stg) {
		case STG_NOT_STARTED:
			setProgress(CMP_CONNECTION, 0.0, "Not started");

		case STG_CONNECTION_STARTING:
		case STG_CONNECTION_FAILED:
		case STG_CONNECTION_RUNNING:
			setProgress(CMP_WIIMOTE, 0.0, "Not started");

		case STG_WIIMOTE_STARTING:
		case STG_WIIMOTE_ALLOCATE_FAILED:
		case STG_WIIMOTE_CONNECT_FAILED:
		case STG_WIIMOTE_RUNNING:
			setProgress(CMP_TRACKER, 0.0, "Not started");

		case STG_TRACKER_STARTING:
		case STG_TRACKER_ALLOCATE_FAILED:
		case STG_TRACKER_RUNNING:
			setProgress(CMP_CLIENT, 0.0, "Not started");

		case STG_CLIENT_STARTING:
		case STG_CLIENT_ALLOCATE_FAILED:
		case STG_CLIENT_RUNNING:
		case STG_STARTUP_COMPLETE:
			break;
	}

	// Set current component
	switch (stg) {
		case STG_NOT_STARTED:
			break;

		case STG_CONNECTION_STARTING:
			setProgress(CMP_CONNECTION, 0.0, "Creating server connection...");
			break;

		case STG_CONNECTION_FAILED:
			setProgress(CMP_CONNECTION, 0.2, "Could not create server connection", true);
			break;

		case STG_CONNECTION_RUNNING:
			setProgress(CMP_CONNECTION, 1.0, "Running");
			break;

		case STG_WIIMOTE_STARTING:
#ifndef _WIN32
			setProgress(CMP_WIIMOTE, 0.2, "Press 1 and 2 on Wiimote now!");
#else
			setProgress(CMP_WIIMOTE, 0.2, "Connecting to Wiimote...");
#endif
			break;

		case STG_WIIMOTE_ALLOCATE_FAILED:
			setProgress(CMP_WIIMOTE, 0.2, "Could not allocate Wiimote device", true);
			break;

		case STG_WIIMOTE_CONNECT_FAILED:
			setProgress(CMP_WIIMOTE, 0.3, "Could not connect to a Wiimote", true);
			break;

		case STG_WIIMOTE_RUNNING:
			setProgress(CMP_WIIMOTE, 1.0, "Running");
			break;

		case STG_TRACKER_STARTING:
			setProgress(CMP_TRACKER, 0.2, "Creating tracker device...");
			break;

		case STG_TRACKER_ALLOCATE_FAILED:
			setProgress(CMP_TRACKER, 0.2, "Could not allocate tracker device", true);
			break;

		case STG_TRACKER_RUNNING:
			setProgress(CMP_TRACKER, 1.0, "Running");
			break;

		case STG_CLIENT_STARTING:
			setProgress(CMP_CLIENT, 0.2, "Starting client device...");
			break;

		case STG_CLIENT_ALLOCATE_FAILED:
			setProgress(CMP_CLIENT, 0.2, "Could not allocate client device", true);
			break;

		case STG_CLIENT_RUNNING:
		case STG_STARTUP_COMPLETE:
			setProgress(CMP_CLIENT, 1.0, "Running");
			break;
	}
	mainloop_ui(PROGRESS_EVENT_TIMEOUT);
}

void WiimoteTrackerView::setProgress(const Component cmp, const float completion, const char * message, bool fail) {
	Fl_Progress * pbar = NULL;
	Fl_Button * b = NULL;
	switch (cmp) {
		case CMP_CONNECTION:
			pbar = _progress->_connectionProgress;
			b = _progress->_connectionReset;
			break;

		case CMP_WIIMOTE:
			pbar = _progress->_wiimoteProgress;
			b = _progress->_wiimoteReset;
			break;

		case CMP_TRACKER:
			pbar = _progress->_trackerProgress;
			b = _progress->_trackerReset;
			break;

		case CMP_CLIENT:
			pbar = _progress->_clientProgress;
			b = _progress->_clientReset;
			break;
	}
	assert(pbar);
	assert(b);
	pbar->value(completion);
	pbar->copy_label(message);
	if (fail) {
		b->activate();
	} else {
		b->deactivate();
	}

}

void WiimoteTrackerView::systemIsDown() {
	_gui->setStatus(false);
	refresh_ui();
}

void WiimoteTrackerView::systemIsUp() {
	_gui->setStatus(true);
	if (_progress->visible()) {
		_progress->scheduleClose(PROGRESS_WINDOW_TIMEOUT);
	}
	refresh_ui();
}

void WiimoteTrackerView::systemInTransition() {
	_gui->setWorking();
	refresh_ui();

}

bool WiimoteTrackerView::processView(bool wait) {
	if (_controller->_newReport) {
		_gui->_pos->value(_controller->_pos.c_str());
		_gui->_rot->value(_controller->_rot.c_str());
		_gui->_rate->value(_controller->_rate);
		_controller->_newReport = false;
	}
	return mainloop_ui(wait ? PROGRESS_EVENT_TIMEOUT : 0);
}
