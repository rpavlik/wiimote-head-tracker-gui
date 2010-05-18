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
#include <iostream>

#undef VERBOSE
#undef VERY_VERBOSE


const int	CONNECTION_PORT = vrpn_DEFAULT_LISTEN_PORT_NO;  // Port for connection to listen on
const int	TRACKER_FREQUENCY = 60;
const int	DEBUG_DISPLAY_INTERVAL = 3; // # of seconds between status displays

const float HAVE_CONNECTION = 1.0;
const float HAVE_WIIMOTE = 2.0;
const float HAVE_TRACKER = 3.0;

const char * WIIMOTE_NAME = "WiimoteForHead";
const char * WIIMOTE_REMOTE_NAME = "*WiimoteForHead";

const float PROGRESS_EVENT_TIMEOUT = 0.2;
const float PROGRESS_WINDOW_TIMEOUT = 3.0;

bool WiimoteTracker::startConnection() {
#ifdef VERBOSE
	std::cout << "In " << __FILE__ << ":" << __LINE__ << "  " << __FUNCTION__ << std::endl;
#endif
	_gui->setWorking();
	_progress->show();
	_progress->setConnectionStatus(0.2, "Creating server connection");
	mainloop_ui(PROGRESS_EVENT_TIMEOUT);

	_connection = vrpn_create_server_connection(CONNECTION_PORT);
	if (!_connection) {
		// error condition creating connection
		_progress->setConnectionStatus(0.2, "Could not create server connection!", true);
		mainloop_ui(PROGRESS_EVENT_TIMEOUT);
		return false;
	}

	_progress->setConnectionStatus(1, "Running");
	mainloop_ui(PROGRESS_EVENT_TIMEOUT);
	return true;
}


bool WiimoteTracker::startWiimoteDevice() {
#ifdef VERBOSE
	std::cout << "In " << __FILE__ << ":" << __LINE__ << "  " << __FUNCTION__ << std::endl;
#endif
	_gui->setWorking();
	teardownWiimoteDevice();
	if (!_connection) {
		return false;
	}
	_progress->show();
	mainloop_ui(PROGRESS_EVENT_TIMEOUT);

#ifndef _WIN32
	_progress->setWiimoteStatus(0.2, "Press 1 and 2 on Wiimote now!");
#else
	_progress->setWiimoteStatus(0.2, "Connecting to Wiimote...");
#endif
	refresh_ui();

	_wiimote = new vrpn_WiiMote(WIIMOTE_NAME, _connection, 0);

	if (!_wiimote) {
		// error condition creating wiimote
		_progress->setWiimoteStatus(0.2, "Could not allocate Wiimote device!", true);
		mainloop_ui(PROGRESS_EVENT_TIMEOUT);
		return false;
	}

	if (!_wiimote->isValid()) {
		_progress->setWiimoteStatus(0.3, "Could not connect to a Wiimote!", true);
		mainloop_ui(PROGRESS_EVENT_TIMEOUT);
		return false;
	}

	_progress->setWiimoteStatus(1, "Running");
	refresh_ui();
	return true;
}

bool WiimoteTracker::startTrackerDevice() {
#ifdef VERBOSE
	std::cout << "In " << __FILE__ << ":" << __LINE__ << "  " << __FUNCTION__ << std::endl;
#endif
	_gui->setWorking();
	teardownTrackerDevice();
	if (!_wiimote || !_connection) {
		return false;
	}
	_progress->show();
	_progress->setTrackerStatus(0.2, "Creating tracker device...");
	mainloop_ui(PROGRESS_EVENT_TIMEOUT);

	_tracker = new vrpn_Tracker_WiimoteHead(_trackerName.c_str(),
			_connection,
			WIIMOTE_REMOTE_NAME,
			TRACKER_FREQUENCY,
			_ledDistance);

	if (!_tracker) {
		// error condition creating tracker device
		_progress->setTrackerStatus(0.2, "Could not allocate tracker device!", true);
		mainloop_ui(PROGRESS_EVENT_TIMEOUT);
		return false;
	}

	_progress->setTrackerStatus(1, "Running");
	_gui->setStatus(true);
	_progress->scheduleClose(PROGRESS_WINDOW_TIMEOUT);
	mainloop_ui(PROGRESS_EVENT_TIMEOUT);
	return true;
}

bool WiimoteTracker::startTrackerDevice() {
#ifdef VERBOSE
	std::cout << "In " << __FILE__ << ":" << __LINE__ << "  " << __FUNCTION__ << std::endl;
#endif
	_gui->setWorking();
	teardownTrackerDevice();
	if (!_wiimote || !_connection) {
		return false;
	}
	_progress->show();
	_progress->setTrackerStatus(0.2, "Creating tracker device...");
	mainloop_ui(PROGRESS_EVENT_TIMEOUT);

	_tracker = new vrpn_Tracker_WiimoteHead(_trackerName.c_str(),
			_connection,
			WIIMOTE_REMOTE_NAME,
			TRACKER_FREQUENCY,
			_ledDistance);

	if (!_tracker) {
		// error condition creating tracker device
		_progress->setTrackerStatus(0.2, "Could not allocate tracker device!", true);
		mainloop_ui(PROGRESS_EVENT_TIMEOUT);
		return false;
	}

	_progress->setTrackerStatus(1, "Running");

	return true;
}

void WiimoteTracker::teardownConnection() {
#ifdef VERBOSE
	std::cout << "In " << __FILE__ << ":" << __LINE__ << "  " << __FUNCTION__ << std::endl;
#endif
	teardownWiimoteDevice();
	if (_connection) {
		delete _connection;
		_connection = NULL;
	}
	_progress->clearConnectionStatus();
}

void WiimoteTracker::teardownWiimoteDevice() {
#ifdef VERBOSE
	std::cout << "In " << __FILE__ << ":" << __LINE__ << "  " << __FUNCTION__ << std::endl;
#endif
	teardownTrackerDevice();
	if (_wiimote) {
		delete _wiimote;
		_wiimote = NULL;
	}
	_progress->clearWiimoteStatus();
}

void WiimoteTracker::teardownTrackerDevice() {
#ifdef VERBOSE
	std::cout << "In " << __FILE__ << ":" << __LINE__ << "  " << __FUNCTION__ << std::endl;
#endif
	if (_tracker) {
		delete _tracker;
		_tracker = NULL;
	}
	_progress->clearTrackerStatus();
}

bool WiimoteTracker::isSystemRunning(bool updateProgress) {
#ifdef VERY_VERBOSE
	std::cout << "In " << __FILE__ << ":" << __LINE__ << "  " << __FUNCTION__ << std::endl;
#endif
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

	// If we made it here it's all working
	return true;
}
