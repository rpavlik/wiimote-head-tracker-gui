/**	@file	WiimoteTracker_VRPN.cpp
	@brief	implementation methods involving VRPN


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

void WiimoteTracker::startConnection() {
	_progress->show();
	_progress->mainloop();
	_progress->setConnectionStatus(0.2, "Creating server connection");
	_progress->mainloop();
	_connection = vrpn_create_server_connection(CONNECTION_PORT);
	if (!_connection) {
		// error condition creating connection
		_progress->setConnectionStatus(0.2, "Could not create server connection!", true);
		_progress->mainloop();
		return;
	}

	_progress->setConnectionStatus(1, "Running");
	_progress->mainloop();
}


void WiimoteTracker::startWiimoteDevice() {;
	_progress->show();
	_progress->mainloop();
	teardownWiimoteDevice();
	if (!_connection) {
		return;
	}

#ifndef _WIN32
	_progress->setWiimoteStatus(0.2, "Press 1 and 2 on Wiimote now!");
#else
	_progress->setWiimoteStatus(0.2, "Connecting to Wiimote...");
#endif
	_progress->mainloop();

	_wiimote = new vrpn_WiiMote(WIIMOTE_NAME, _connection, 0);

	if (!_wiimote) {
		// error condition creating wiimote
		_progress->setWiimoteStatus(0.2, "Could not allocate Wiimote device!", true);
		_progress->mainloop();
		return;
	}

	if (!_wiimote->isValid()) {
		teardownWiimoteDevice();
		_progress->setWiimoteStatus(0.3, "Could not connect to a Wiimote!", true);
		_progress->mainloop();
		return;
	}

	_progress->setWiimoteStatus(1, "Running");
	_progress->mainloop();
}



void WiimoteTracker::startTrackerDevice() {
	_progress->show();
	_progress->mainloop();
	teardownTrackerDevice();
	if (!_wiimote) {
		return;
	}
	_progress->setTrackerStatus(0.2, "Creating tracker device...");
	_progress->mainloop();

	_tracker = new vrpn_Tracker_WiimoteHead(_trackerName.c_str(),
			_connection,
			WIIMOTE_REMOTE_NAME,
			TRACKER_FREQUENCY,
			_ledDistance);

	if (!_tracker) {
		// error condition creating tracker device
		_progress->setTrackerStatus(0.2, "Could not allocate tracker device!", true);
		_progress->mainloop();
		return;
	}

	_progress->setTrackerStatus(1, "Running");
	_progress->mainloop();

	//_progress->hide();
	_gui->show();
	_gui->mainloop();
}

void WiimoteTracker::teardownConnection() {
	teardownWiimoteDevice();
	if (_connection) {
		delete _connection;
		_connection = NULL;
	}
	_progress->clearConnectionStatus();
}

void WiimoteTracker::teardownWiimoteDevice() {
	teardownTrackerDevice();
	if (_wiimote) {
		delete _wiimote;
		_wiimote = NULL;
	}
	_progress->clearWiimoteStatus();
}

void WiimoteTracker::teardownTrackerDevice() {
	if (_tracker) {
		delete _tracker;
		_tracker = NULL;
	}
	_progress->clearTrackerStatus();
}
