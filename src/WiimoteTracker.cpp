/**	@file	WiimoteTracker.cpp
	@brief	implementation


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

const int	CONNECTION_PORT = vrpn_DEFAULT_LISTEN_PORT_NO;  // Port for connection to listen on
const int	TRACKER_FREQUENCY = 60;
const int	DEBUG_DISPLAY_INTERVAL = 3; // # of seconds between status displays

const float HAVE_CONNECTION = 1.0;
const float HAVE_WIIMOTE = 2.0;
const float HAVE_TRACKER = 3.0;

const char * WIIMOTE_NAME = "WiimoteForHead";
const char * WIIMOTE_REMOTE_NAME = "*WiimoteForHead";

WiimoteTracker::WiimoteTracker() :
		_ready(false),
		_trackerName("Tracker0"),
		_ledDistance(0.145),
		_connection(NULL),
		_wiimote(NULL),
		_tracker(NULL),
		_gui(new WiimoteTrackerGUI(433,227,480,185, "Wii Remote Head Tracker")) {

}

WiimoteTracker::~WiimoteTracker() {
	delete _gui;
	teardownConnection();
}

void WiimoteTracker::startConnection() {
	_connection = vrpn_create_server_connection(CONNECTION_PORT);
	if (!_connection) {
		// error condition creating connection
		return;
	}

	_gui->setProgress(HAVE_CONNECTION);
	_gui->mainloop();
}

void WiimoteTracker::teardownConnection() {
	teardownWiimoteDevice();
	if (_connection) {
		delete _connection;
		_connection = NULL;
	}
}

void WiimoteTracker::startWiimoteDevice() {
	_gui->refreshDisplay();

	teardownWiimoteDevice();
	if (!_connection) {
		return;
	}

	_wiimote = new vrpn_WiiMote(WIIMOTE_NAME, _connection, 0);

	if (!_wiimote) {
		// error condition creating wiimote
		return;
	}

	if (!_wiimote->isValid()) {
		// Could not connect to a wiimote
		teardownWiimoteDevice();
		return;
	}

	_gui->setProgress(HAVE_WIIMOTE);
	_gui->mainloop();
}

void WiimoteTracker::teardownWiimoteDevice() {
	teardownTrackerDevice();
	if (_wiimote) {
		delete _wiimote;
		_wiimote = NULL;
	}
}

void WiimoteTracker::startTrackerDevice() {
	_gui->refreshDisplay();

	teardownTrackerDevice();
	if (!_wiimote) {
		return;
	}
	_tracker = new vrpn_Tracker_WiimoteHead(_trackerName.c_str(),
			_connection,
			WIIMOTE_REMOTE_NAME,
			TRACKER_FREQUENCY,
			_ledDistance);

	if (!_tracker) {
		// error condition creating tracker device
		return;
	}
	_gui->setProgress(HAVE_TRACKER);
	_gui->mainloop();
}

void WiimoteTracker::teardownTrackerDevice() {
	if (_tracker) {
		delete _tracker;
		_tracker = NULL;
	}
}

void WiimoteTracker::setLEDDistance(const float distanceInMeters) {
	if (distanceInMeters <= 0) {
		// Invalid change
		_gui->setLEDDistance(_ledDistance);
		return;
	}
	// This invalidates the tracker
	teardownTrackerDevice();
	_ledDistance = distanceInMeters;
}
void WiimoteTracker::setTrackerName(const std::string & trackerName) {
	if (trackerName.size() == 0) {
		// Invalid change
		_gui->setTrackerName(_trackerName.c_str());
		return;
	}
	// This invalidates the tracker
	teardownTrackerDevice();
	_trackerName = trackerName;
}

void WiimoteTracker::run() {
	_gui->setTracker(this);
	_gui->setProgress(0);
	_gui->show();
	_gui->mainloop();

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
