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
	if (trackerName.size() == 0) {
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
	_gui->setWorking();
	_gui->updateVersions();

	// Show initial windows
	_gui->show();
	_progress->show();
	refresh_ui();

	// Create connection
	startConnection();

	// Start up the wiimote
	startWiimoteDevice();

	// Start up the tracker
	startTrackerDevice();

	while (mainloop_ui()) {
		if (isSystemRunning()) {
				_wiimote->mainloop();
				_tracker->mainloop();
				_connection->mainloop();
		} else {
			// Set up the progress display
			isSystemRunning(true);
			_progress->show();
		}
		// Sleep for 1ms so we don't eat the CPU
		vrpn_SleepMsecs(1);
	}
}

void WiimoteTracker::reconfigure() {
	_config->show();
	refresh_ui();
}
