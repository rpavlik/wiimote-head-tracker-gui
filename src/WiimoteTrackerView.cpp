/**	@file	WiimoteTrackerView.cpp
	@brief	View implementation

	@date	2010

	@author
	Ryan Pavlik ( <rpavlik@iastate.edu> http://academic.cleardefinition.com/ ),
	Iowa State University
	Virtual Reality Applications Center and
	Human-Computer Interaction Graduate Program
 */
/*
 Copyright Iowa State University 2009-2010
 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or copy at
 http://www.boost.org/LICENSE_1_0.txt)
 */

// Internal Includes
#include "WiimoteTrackerView.h"

#include "WiimoteTracker.h"

#include <WiimoteTrackerGUI.h>

// Library/third-party includes
#include <FL/fl_ask.H>
#include <FL/Fl_Native_File_Chooser.H>

// Standard includes
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>

WiimoteTrackerView::WiimoteTrackerView(WiimoteTracker * controller) :
		_progress(new StartupProgress(430,360, "Starting Tracking System...")),
		_config(new WiimoteTrackerConfigGUI(420, 160, "Tracker Configuration")),
		_gui(new WiimoteTrackerGUI(520, 560, "Wii Remote Head Tracker")),
		_fc(NULL),
		_controller(controller) {
	assert(_progress);
	assert(_config);
	assert(_gui);
	assert(_controller);

	// Pick an appropriate scheme
#if defined(_APPLE)
	Fl::scheme("plastic");
	
#elif defined(_WIN32)
	#if defined(FL_XP_UP_BOX)
			// If we were built against an XP scheme patched FLTK
			Fl::scheme("xp");
	#elif defined(FL_GLEAM_UP_BOX)
			// If we were built against an Fl_Gleam-patched FLTK
			/Fl::scheme("gleam");
	#endif
	
#elif defined(_BUILT_ON_REDHAT)
	#if defined(FL_XP_UP_BOX)
		// If we were built against an XP scheme patched FLTK
		Fl::scheme("xp");
	#else
		Fl::scheme("gtk+");
	#endif
	
#elif defined(_BUILT_ON_UBUNTU)
	Fl::scheme("gtk+");
#endif
}

WiimoteTrackerView::~WiimoteTrackerView() {
	delete _fc;
	_fc= NULL;

	delete _progress;
	_progress = NULL;

	delete _config;
	_config = NULL;

	delete _gui;
	_gui = NULL;
}

void WiimoteTrackerView::applyNewConfiguration() {
	std::string trackerName = _config->_trackerName->value();
	float distanceInMeters = _config->_ledDistance->value() / 100.0;
	TrackerConfiguration newConfig;
	try {
		newConfig = TrackerConfiguration(distanceInMeters, trackerName);
	} catch (std::exception & e) {
		std::cerr << "Could not create new configuration with parameters " << distanceInMeters << " and '" << trackerName << "'" << std::endl;
		std::cerr << "Exception details: " << e.what() << std::endl;
		updateConfigDisplay();
		return;
	}

	bool ret = _controller->applyNewConfiguration(newConfig);

	if (!ret) {
		std::cerr << "Could not apply new configuration with parameters " << distanceInMeters << " and '" << trackerName << "'" << std::endl;
		std::cerr << "Controller rejected the change" << std::endl;
	}

	updateConfigDisplay();
	return;
}

void WiimoteTrackerView::run() {
	// Set tracker pointers in GUI
	setTracker(_controller, this);

	// Set up progress bars
	setProgressRanges();

	// Prepare the main window
	_gui->updateVersions();

	// Update the message about the report stride
	std::ostringstream s;
	s << "Updated every " << REPORT_STRIDE << " reports";
	_gui->_updateGroup->copy_label(s.str().c_str());

	// Show initial windows
	_gui->show();
	_progress->show();

	refresh_ui();
}


void WiimoteTrackerView::reconfigure() {
	updateConfigDisplay();
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

void WiimoteTrackerView::setProgress(const TrackerComponent cmp, const float completion, const char * message, bool fail) {
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

void WiimoteTrackerView::updateConfigDisplay() {
	/// Update config window
	_config->_trackerName->value(_controller->_activeConfig.getTrackerName().c_str());
	_config->_ledDistance->value(_controller->_activeConfig.getLEDDistance() * 100.0);

	_config->_apply->deactivate();
	_config->_save->activate();

	/// Update main window
	_gui->_trackerName->value(_controller->_activeConfig.getTrackerName().c_str());
	_gui->_ledDistance->value(_controller->_activeConfig.getLEDDistance() * 100.0);
	
	refresh_ui();
}

void WiimoteTrackerView::saveConfig() {
	delete _fc;
	_fc = new Fl_Native_File_Chooser(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
	_fc->title("Save Wii Remote Head Tracker Config File...");
	_fc->filter("Head Tracker Config Files\t*.headtrackconfig");
	_fc->preset_file("default.headtrackconfig");
	int ret = _fc->show();
	if (ret != 0) {
		// No file picked, for one reason or another.
		return;
	}

	std::ofstream confFile(_fc->filename());
	if (!confFile.is_open()) {
		fl_alert("Could not save configuration to file %s - perhaps try another file name or location", _fc->filename());
		return;
	}
	confFile << _controller->_activeConfig;
	confFile.close();
}

void WiimoteTrackerView::openConfig() {
	delete _fc;
	_fc = new Fl_Native_File_Chooser(Fl_Native_File_Chooser::BROWSE_FILE);
	_fc->title("Open Wii Remote Head Tracker Config File...");
	_fc->filter("Head Tracker Config Files\t*.headtrackconfig");

	int ret = _fc->show();
	if (ret != 0) {
		// No file picked, for one reason or another.
		return;
	}

	std::ifstream confFile(_fc->filename());
	if (!confFile.is_open()) { return; }
	TrackerConfiguration newConfig;
	try {
		confFile >> newConfig;
	} catch (std::exception & e) {
		std::cerr << "Could not load configuration from  " << _fc->filename() << std::endl;
		std::cerr << "Exception details: " << e.what() << std::endl;
		fl_alert("Could not load configuration from file %s", _fc->filename());
		return;
	}
	bool controllerRet = _controller->applyNewConfiguration(newConfig);
	if (!controllerRet) {
		std::cerr << "Could not apply new configuration from " << _fc->filename() << std::endl;
		std::cerr << "Controller rejected the change" << std::endl;
		fl_alert("Could not apply configuration from file %s", _fc->filename());
		return;
	}
	updateConfigDisplay();
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
