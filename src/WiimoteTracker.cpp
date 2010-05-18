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
// - none

WiimoteTracker::WiimoteTracker() :
		_ready(false),
		_trackerName("Tracker0"),
		_ledDistance(0.145),
		_connection(NULL),
		_wiimote(NULL),
		_tracker(NULL),
		_progress(new StartupProgress(20,20,430,250, "Starting Tracking System...")),
		_config(new WiimoteTrackerConfigGUI(20, 20, 350, 160, "Tracker Configuration")),
		_gui(new WiimoteTrackerGUI(20,20,260,90, "Wii Remote Head Tracker")) {

}

WiimoteTracker::~WiimoteTracker() {
	delete _progress;
	delete _config;
	delete _gui;

	teardownConnection();
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
	_progress->setTracker(this);
	_progress->clearConnectionStatus();
	_progress->clearWiimoteStatus();
	_progress->clearTrackerStatus();

	_config->setTracker(this);
	_gui->setTracker(this);

	// Create connection
	startConnection();

	// Start up the wiimote
	startWiimoteDevice();

	// Start up the tracker
	startTrackerDevice();

	while (_gui->mainloop()) {
		if (_wiimote) {
			_wiimote->mainloop();
		}

		if (_tracker) {
			_tracker->mainloop();
		}

		if (_connection) {
			_connection->mainloop();
		}

		// Sleep for 1ms so we don't eat the CPU
		vrpn_SleepMsecs(1);
	}
}
