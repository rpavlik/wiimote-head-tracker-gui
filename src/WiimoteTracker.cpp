/**	@file	WiimoteTracker.cpp
	@brief	implementation involving interaction

	@date	2010

	@author
	Ryan Pavlik ( <rpavlik@iastate.edu> http://academic.cleardefinition.com/ ),
	Iowa State University
	Virtual Reality Applications Center and
	Human-Computer Interaction Graduate Program
*/

// Internal Includes
#include "WiimoteTracker.h"

#include <WiimoteTrackerGUI.h>

// Library/third-party includes
#include <vrpn_Configure.h>
#include <vrpn_WiiMote.h>
#include <vrpn_Tracker_WiimoteHead.h>

// Standard includes
#include <cassert>

WiimoteTracker::WiimoteTracker() :
		_trackerName("Tracker0"),
		_ledDistance(0.145),
		_connection(NULL),
		_wiimote(NULL),
		_tracker(NULL),
		_progress(new StartupProgress(430,290, "Starting Tracking System...")),
		_config(new WiimoteTrackerConfigGUI(350, 160, "Tracker Configuration")),
		_gui(new WiimoteTrackerGUI(520,560, "Wii Remote Head Tracker")) {
	assert(_progress);
	assert(_config);
	assert(_gui);
}

WiimoteTracker::~WiimoteTracker() {
	teardownConnection();

	delete _progress;
	_progress = NULL;

	delete _config;
	_config = NULL;

	delete _gui;
	_gui = NULL;
}

void WiimoteTracker::setLEDDistance(const float distanceInMeters) {
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

void WiimoteTracker::setTrackerName(const std::string & trackerName) {
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

void WiimoteTracker::run() {
	// Set tracker pointers in GUI
	_config->setTracker(this);
	_gui->setTracker(this);
	_progress->setTracker(this);

	// Prepare the main window
	_gui->updateVersions();

	// Show initial windows
	_gui->show();
	_progress->show();
	refresh_ui();

	startTrackerSystem();

	while (mainloop_ui()) {
		if (isSystemRunning()) {
				_wiimote->mainloop();
				_tracker->mainloop();
				_connection->mainloop();
		}
		// Sleep for 1ms so we don't eat the CPU
		vrpn_SleepMsecs(1);
	}
}

void WiimoteTracker::reconfigure() {
	_config->show();
	refresh_ui();
}

void WiimoteTracker::stopTrackerSystem() {
	_gui->setWorking();
	refresh_ui();
	teardownConnection();
	_gui->setStatus(false);
}

void WiimoteTracker::startTrackerSystem() {
	_gui->setWorking();

	bool ret;
	// Create connection
	ret = startConnection();
	if (!ret) {
		return;
	}

	// Start up the wiimote
	ret = startWiimoteDevice();
	if (!ret) {
		return;
	}

	// Start up the tracker
	ret = startTrackerDevice();
	if (!ret) {
		return;
	}
}

void setProgress(const Component cmp, const float completion, const char * message) {
	switch (cmp) {
		case CMP_CONNECTION:

		break;

		case CMP_WIIMOTE:

		break;

		case CMP_TRACKER:

		break;

		case CMP_CLIENT:


	}

}

void WiimoteTracker::systemIsDown() {


}

void WiimoteTracker::systemIsUp() {
	_gui->setStatus(true);
	_progress->scheduleClose(PROGRESS_WINDOW_TIMEOUT);
	mainloop_ui(PROGRESS_EVENT_TIMEOUT);
}
