/** @file	WiimoteTracker.h
	@brief	header

	@date	2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

#pragma once
#ifndef _WIIMOTETRACKER_H
#define _WIIMOTETRACKER_H

// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
#include <string>

class WiimoteTrackerGUI;
class WiimoteTrackerConfigGUI;
class StartupProgress;
class vrpn_Connection;
class vrpn_WiiMote;
class vrpn_Tracker_WiimoteHead;


class WiimoteTracker {
	public:
		WiimoteTracker();
		~WiimoteTracker();

		void setLEDDistance(const float distanceInMeters);
		void setTrackerName(const std::string & trackerName);

		void startConnection();
		void teardownConnection();

		void startWiimoteDevice();
		void teardownWiimoteDevice();

		void startTrackerDevice();
		void teardownTrackerDevice();

		void run();

	protected:
		bool _ready;

		std::string _trackerName;
		float _ledDistance;

		vrpn_Connection * _connection;
		vrpn_WiiMote * _wiimote;
		vrpn_Tracker_WiimoteHead * _tracker;

		StartupProgress * _progress;
		WiimoteTrackerConfigGUI * _config;
		WiimoteTrackerGUI * _gui;
};
#endif // WIIMOTETRACKER
