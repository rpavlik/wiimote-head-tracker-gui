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
#include "SystemComponents.h"

// Library/third-party includes
// - none

// Standard includes
#include <string>

class vrpn_Connection;
class vrpn_WiiMote;
class vrpn_Tracker_WiimoteHead;
class vrpn_Tracker_Remote;

class WiimoteTrackerView;

class WiimoteTracker {
	public:
		WiimoteTracker();
		~WiimoteTracker();
		
		void run();

		/// @name VRPN-related methods
		/// @{
		bool startConnection();
		void teardownConnection();

		bool startWiimoteDevice();
		void teardownWiimoteDevice();

		bool startTrackerDevice();
		void teardownTrackerDevice();

		bool startClientDevice();
		void teardownClientDevice();
		/// @}

		bool isSystemRunning(bool updateProgress = false);

	protected:
		/// @name Configuration data
		/// @{
		std::string _trackerName;
		float _ledDistance;
		/// @}

		/// @name VRPN objects
		/// @{
		vrpn_Connection * _connection;
		vrpn_WiiMote * _wiimote;
		vrpn_Tracker_WiimoteHead * _tracker;
		vrpn_Tracker_Remote * _client;
		/// @}
		
		/// @brief Pointer to view
		WiimoteTrackerView * _view;
};
#endif // WIIMOTETRACKER
