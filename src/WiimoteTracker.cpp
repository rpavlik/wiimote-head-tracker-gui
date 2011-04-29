/**	@file	WiimoteTracker.cpp
	@brief	Implementation of Controller

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
#include "WiimoteTracker.h"
#include "WiimoteTrackerView.h"

// Library/third-party includes
#include <vrpn_Configure.h>
#include <vrpn_WiiMote.h>
#include <vrpn_Tracker_WiimoteHead.h>

// Standard includes
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

#undef VERBOSE
#undef VERY_VERBOSE


const int	CONNECTION_PORT = vrpn_DEFAULT_LISTEN_PORT_NO;  // Port for connection to listen on
const int	TRACKER_FREQUENCY = 60;

const char * WIIMOTE_NAME = "WiiMote0";
const char * WIIMOTE_REMOTE_NAME = "*WiiMote0";

/// @name VRPN utility function and callback
/// @{
static	double	duration(struct timeval t1, struct timeval t2) {
	return (t1.tv_usec - t2.tv_usec) / 1000000.0 +
	       (t1.tv_sec - t2.tv_sec);
}

static void	VRPN_CALLBACK handle_pos(void* userdata, const vrpn_TRACKERCB t) {
	static bool first = true;
	static int count = 0;
	static struct timeval last_display;
	if (first) {
		vrpn_gettimeofday(&last_display, NULL);
		first = false;
	}
	count++;
	if (count > REPORT_STRIDE) {
		struct timeval now;
		vrpn_gettimeofday(&now, NULL);
		const double interval = duration(now, last_display);
		const double frequency = count / interval;
		std::ostringstream pos, rot;
		pos << "(" << std::fixed << std::setprecision(4) <<
			t.pos[0] << ", " <<
			t.pos[1] << ", " <<
			t.pos[2] << ")";
		rot << "(" << std::fixed << std::setprecision(3) <<
			t.quat[0] << ", " <<
			t.quat[1] << ", " <<
			t.quat[2] << ", " <<
			t.quat[3] << ")";
		static_cast<WiimoteTracker*>(userdata)->setReport(pos.str(), rot.str(), frequency);
		count = 0;
		last_display = now;
	}
}

static void VRPN_CALLBACK handle_wiimote(void* userdata, const vrpn_ANALOGCB a) {
	static_cast<WiimoteTracker*>(userdata)->setBattery(a.channel[0]);
}
/// @}

WiimoteTracker::WiimoteTracker() :
		_activeConfig(),
		_connection(NULL),
		_wiimote(NULL),
		_tracker(NULL),
		_client(NULL),
		_wiimoteClient(NULL),
		_wiimoteOutClient(NULL),
		_view(new WiimoteTrackerView(this)),
		_newReport(true),
		_pos("Report not yet received."),
		_rot("Report not yet received."),
		_rate(0),
		_grabBattery(false),
		_batLevel(""),
		_supportsSensitivity(false) {
}

WiimoteTracker::~WiimoteTracker() {
	teardownConnection();
}

bool WiimoteTracker::loadDefaultConfigFile() {
	if (_connection || _wiimote || _tracker || _client) {
		std::cerr << "Can't load default config file if system is running!" << std::endl;
		return false;
	}

	std::ifstream confFile("default.headtrackconfig");
	if (!confFile.is_open()) {
		return false;
	}
	TrackerConfiguration newConfig;
	try {
		confFile >> newConfig;

		// Can just directly set the _activeConfig since the tracker isn't started yet
		_activeConfig = newConfig;
	} catch (std::exception & e) {
		std::cerr << "Could not load configuration from default.headtrackconfig" << std::endl;
		std::cerr << "Exception details: " << e.what() << std::endl;
		return false;
	}
	
	return true;
}

void WiimoteTracker::run() {
	// Set up view
	_view->run();

	// Attempt to load default config file
	bool defaultConfLoaded = loadDefaultConfigFile();
	if (defaultConfLoaded) {
		std::cerr << "Loaded default config file default.headtrackconfig" << std::endl;
	} else {
		std::cerr << "No valid default config file default.headtrackconfig found - using compiled-in defaults." << std::endl;
	}
	_view->updateConfigDisplay();

	startTrackerSystem();

	while (_view->processView()) {
		if (isSystemRunning()) {
			_wiimote->mainloop();
			_tracker->mainloop();
			_connection->mainloop();
		}
		// Sleep for 1ms so we don't eat the CPU
		vrpn_SleepMsecs(1);
	}
}

void WiimoteTracker::stopTrackerSystem() {
	_view->systemInTransition();
	teardownConnection();

	// Remove old from screen when shutting down tracker
	_pos = " ";
	_rot = " ";
	_rate = 0;
	_batLevel = " ";
	_newReport = true;

	_view->systemIsDown();
}

void WiimoteTracker::startTrackerSystem() {
	_view->systemInTransition();

	bool ret;
	// Create connection
	if (!_connection) {
		_view->setProgress(STG_NOT_STARTED);
		ret = startConnection();
		if (!ret) {
			return;
		}
	}

	// Start up the wiimote
	if (!_wiimote) {
		ret = startWiimoteDevice();
		if (!ret) {
			return;
		}
	}

	// Start up the tracker
	if (!_tracker) {
		ret = startTrackerDevice();
		if (!ret) {
			return;
		}
	}

	// Start up the client
	if (!_client) {
		ret = startClientDevice();
		if (!ret) {
			return;
		}
	}
	_view->systemIsUp();
}


bool WiimoteTracker::startConnection() {
#ifdef VERBOSE
	std::cout << "In " << __FILE__ << ":" << __LINE__ << "  " << __FUNCTION__ << std::endl;
#endif
	_view->setProgress(STG_CONNECTION_STARTING);

	_connection = vrpn_create_server_connection(CONNECTION_PORT);
	if (!_connection) {
		// error condition creating connection
		_view->setProgress(STG_CONNECTION_FAILED);
		return false;
	}

	_view->setProgress(STG_CONNECTION_RUNNING);
	return true;
}


bool WiimoteTracker::startWiimoteDevice() {
#ifdef VERBOSE
	std::cout << "In " << __FILE__ << ":" << __LINE__ << "  " << __FUNCTION__ << std::endl;
#endif

	teardownWiimoteDevice();
	if (!_connection) {
		return false;
	}

	_view->setProgress(STG_WIIMOTE_STARTING);
	_wiimote = new vrpn_WiiMote(WIIMOTE_NAME, _connection, 0, 1, 1, 1);

	if (!_wiimote) {
		// error condition creating wiimote
		_view->setProgress(STG_WIIMOTE_ALLOCATE_FAILED);
		return false;
	}
/*
	if (!_wiimote->isValid()) {
		_view->setProgress(STG_WIIMOTE_CONNECT_FAILED);
		return false;
	}
*/
	_view->setProgress(STG_WIIMOTE_RUNNING);
	return true;
}

bool WiimoteTracker::startTrackerDevice() {
#ifdef VERBOSE
	std::cout << "In " << __FILE__ << ":" << __LINE__ << "  " << __FUNCTION__ << std::endl;
#endif
	teardownTrackerDevice();
	if (!_wiimote || !_connection) {
		return false;
	}
	_view->setProgress(STG_TRACKER_STARTING);

	_tracker = new vrpn_Tracker_WiimoteHead(_activeConfig.getTrackerName().c_str(),
			_connection,
			WIIMOTE_REMOTE_NAME,
			TRACKER_FREQUENCY,
			_activeConfig.getLEDDistance());

	if (!_tracker) {
		// error condition creating tracker device
		_view->setProgress(STG_TRACKER_ALLOCATE_FAILED);
		return false;
	}

	_view->setProgress(STG_TRACKER_RUNNING);
	return true;
}

bool WiimoteTracker::startClientDevice() {
#ifdef VERBOSE
	std::cout << "In " << __FILE__ << ":" << __LINE__ << "  " << __FUNCTION__ << std::endl;
#endif

	teardownClientDevice();
	if (!_wiimote || !_connection || !_tracker) {
		return false;
	}
	_view->setProgress(STG_CLIENT_STARTING);

	_client = new vrpn_Tracker_Remote(_activeConfig.getTrackerName().c_str(),
			_connection);

	if (!_client) {
		// error condition creating client device
		_view->setProgress(STG_CLIENT_ALLOCATE_FAILED);
		return false;
	}

	_client->register_change_handler(this, handle_pos);

	_wiimoteClient = new vrpn_Analog_Remote(WIIMOTE_NAME,
			_connection);

	_wiimoteOutClient = new vrpn_Analog_Output_Remote(WIIMOTE_NAME,
			_connection);
	if (!_wiimoteClient || ! _wiimoteOutClient) {
		// error condition creating client devices
		_view->setProgress(STG_CLIENT_ALLOCATE_FAILED);
		return false;
	}
	_wiimoteClient->register_change_handler(this, handle_wiimote);
	_supportsSensitivity = (_wiimoteOutClient->getNumChannels() >= 2);

	_view->setProgress(STG_CLIENT_RUNNING);
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
}

void WiimoteTracker::teardownTrackerDevice() {
#ifdef VERBOSE
	std::cout << "In " << __FILE__ << ":" << __LINE__ << "  " << __FUNCTION__ << std::endl;
#endif
	teardownClientDevice();
	if (_tracker) {
		delete _tracker;
		_tracker = NULL;
	}
}

void WiimoteTracker::teardownClientDevice() {
#ifdef VERBOSE
	std::cout << "In " << __FILE__ << ":" << __LINE__ << "  " << __FUNCTION__ << std::endl;
#endif
	if (_client) {
		delete _client;
		_client = NULL;
	}
	if (_wiimoteClient) {
		delete _wiimoteClient;
		_wiimoteClient = NULL;
	}
	delete _wiimoteOutClient;
	_wiimoteOutClient = NULL;
}

bool WiimoteTracker::applyNewConfiguration(const TrackerConfiguration & config) {
	// If we were running, we will start running again.
	bool wasRunning(isSystemRunning());
	_view->systemInTransition();
	teardownTrackerDevice();
	_activeConfig = config;
	if (wasRunning) {
		bool ret = startTrackerDevice();
		if (!ret) {
			return false;
		}

		ret = startClientDevice();
		if (!ret) {
			return false;
		}

		_view->systemIsUp();
	} else {
		_view->systemIsDown();
	}
	return true;
}

bool WiimoteTracker::isSystemRunning() const {
#ifdef VERY_VERBOSE
	std::cout << "In " << __FILE__ << ":" << __LINE__ << "  " << __FUNCTION__ << std::endl;
#endif
	return (_connection && _wiimote && _tracker && _client);
}

bool WiimoteTracker::isWiimoteConnected() {
	if (_wiimote && _wiimote->isValid()) {
		return true;
	} else {
		_batLevel.clear();
		return false;
	}
}

void WiimoteTracker::setReport(const std::string & pos, const std::string & rot, const double rate) {
	_pos = pos;
	_rot = rot;
	_rate = rate;
	_newReport = true;
	_grabBattery = true;
}

void WiimoteTracker::setBattery(const double batLevel) {
	if (_grabBattery) {
		_grabBattery = false;
		std::ostringstream bat;
		bat << std::fixed << std::setprecision(1) <<
			batLevel * 100.0 << "%";
		_batLevel = bat.str();
	}
}

bool WiimoteTracker::supportsSensitivityChange() const {
	return _supportsSensitivity;
}

void WiimoteTracker::setSensitivity(int level) {
	if (_supportsSensitivity) {
		_wiimoteOutClient->request_change_channel_value(1, level);
	}
}
