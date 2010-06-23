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
/*
 Copyright Iowa State University 2009-2010
 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or copy at
 http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef _WIIMOTETRACKER_H
#define _WIIMOTETRACKER_H

// Internal Includes
#include "SystemComponents.h"
#include "TrackerConfiguration.h"

// Library/third-party includes
// - none

// Standard includes
#include <string>

class vrpn_Connection;
class vrpn_WiiMote;
class vrpn_Tracker_WiimoteHead;
class vrpn_Tracker_Remote;
class vrpn_Analog_Remote;

class WiimoteTrackerView;

#define REPORT_STRIDE 30

class WiimoteTracker {
	public:
		WiimoteTracker();
		~WiimoteTracker();

		void run();

		/// @name VRPN-related methods
		/// @{
		void stopTrackerSystem();
		void startTrackerSystem();

		bool startConnection();
		void teardownConnection();

		bool startWiimoteDevice();
		void teardownWiimoteDevice();

		bool startTrackerDevice();
		void teardownTrackerDevice();

		bool startClientDevice();
		void teardownClientDevice();
		/// @}

		bool isSystemRunning() const;

		bool loadDefaultConfigFile();
		bool applyNewConfiguration(const TrackerConfiguration & config);

		/// @brief Function used by the VRPN callback to store periodic data
		void setReport(const std::string & pos, const std::string & rot, const double rate);

		/// @brief Function used by the VRPN callback to store periodic data
		void setBattery(const double batLevel);

	protected:
		/// @name Configuration data
		/// @{
		TrackerConfiguration _activeConfig;
		/// @}

		/// @name VRPN objects
		/// @{
		vrpn_Connection * _connection;
		vrpn_WiiMote * _wiimote;
		vrpn_Tracker_WiimoteHead * _tracker;
		vrpn_Tracker_Remote * _client;
		vrpn_Analog_Remote * _wiimoteClient;
		/// @}

		/// @brief Pointer to view
		WiimoteTrackerView * _view;

		/// @name Data from selected reports, to display
		/// @{
		bool _newReport;
		std::string _pos;
		std::string _rot;
		float _rate;

		bool _grabBattery;
		std::string _batLevel;
		/// @}

		/// @todo Remove this once configuration adjustment is improved.
		friend class WiimoteTrackerView;
};
#endif // WIIMOTETRACKER
