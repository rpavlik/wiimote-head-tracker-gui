/**	@file	WiimoteTracker.cpp
	@brief	Implementation of Controller

	@date	2010

	@author
	Ryan Pavlik ( <rpavlik@iastate.edu> http://academic.cleardefinition.com/ ),
	Iowa State University
	Virtual Reality Applications Center and
	Human-Computer Interaction Graduate Program
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
		std::stringstream pos, rot;
		pos << "(" <<
			t.pos[0] << ", " <<
			t.pos[1] << ", " <<
			t.pos[2] << ")";
		rot << "(" <<
			t.quat[0] << ", " <<
			t.quat[1] << ", " <<
			t.quat[2] << ", " <<
			t.quat[3] << ")";
		static_cast<WiimoteTracker*>(userdata)->setReport(pos.str(), rot.str(), frequency);
		count = 0;
		last_display = now;
	}
}
/// @}

WiimoteTracker::WiimoteTracker() :
		_connection(NULL),
		_wiimote(NULL),
		_tracker(NULL),
		_client(NULL),
		_view(new WiimoteTrackerView(this)),
		_newReport(true),
		_pos("Report not yet received."),
		_rot("Report not yet received."),
		_rate(0) {
}

WiimoteTracker::~WiimoteTracker() {
	teardownConnection();
}


void WiimoteTracker::run() {
	_view->run();
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
	_view->systemIsDown();
}

void WiimoteTracker::startTrackerSystem() {
	_view->systemInTransition();
	_view->setProgress(STG_NOT_STARTED);

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

	// Start up the client
	ret = startClientDevice();
	if (!ret) {
		return;
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
	_wiimote = new vrpn_WiiMote(WIIMOTE_NAME, _connection, 0);

	if (!_wiimote) {
		// error condition creating wiimote
		_view->setProgress(STG_WIIMOTE_ALLOCATE_FAILED);
		return false;
	}

	if (!_wiimote->isValid()) {
		_view->setProgress(STG_WIIMOTE_CONNECT_FAILED);
		return false;
	}

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

	_tracker = new vrpn_Tracker_WiimoteHead(_trackerName.c_str(),
			_connection,
			WIIMOTE_REMOTE_NAME,
			TRACKER_FREQUENCY,
			_ledDistance);

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

	_client = new vrpn_Tracker_Remote(_trackerName.c_str(),
			_connection);

	if (!_client) {
		// error condition creating client device
		_view->setProgress(STG_CLIENT_ALLOCATE_FAILED);
		return false;
	}

	_client->register_change_handler(this, handle_pos);

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
}

bool WiimoteTracker::isSystemRunning() const {
#ifdef VERY_VERBOSE
	std::cout << "In " << __FILE__ << ":" << __LINE__ << "  " << __FUNCTION__ << std::endl;
#endif
	return (_connection && _wiimote && _tracker && _client);
}

void WiimoteTracker::setReport(const std::string & pos, const std::string & rot, const float rate) {
	_pos = pos;
	_rot = rot;
	_rate = rate;
	_newReport = true;
}
