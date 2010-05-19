/** @file	WiimoteTrackerView.h
	@brief	header for View

	@date	2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

#pragma once
#ifndef _WIIMOTETRACKERVIEW_H
#define _WIIMOTETRACKERVIEW_H

// Internal Includes
#include "SystemComponents.h"

// Library/third-party includes
// - none

// Standard includes
#include <string>

const float PROGRESS_EVENT_TIMEOUT = 0.2;
const float PROGRESS_WINDOW_TIMEOUT = 3.0;

class WiimoteTrackerGUI;
class WiimoteTrackerConfigGUI;
class StartupProgress;

class WiimoteTracker; // forward declaration of controller

class WiimoteTrackerView {
	public:
		WiimoteTrackerView(WiimoteTracker * controller);
		~WiimoteTrackerView();

		/// @name GUI-related methods
		/// @{
		void setLEDDistance(const float distanceInMeters);
		void setTrackerName(const std::string & trackerName);

		void run();
		void reconfigure();
		void stopTrackerSystem();
		void startTrackerSystem();

		void setProgress(const StartupStage stg);
		void setProgress(const Component cmp, const float completion, const char * message);

		void systemIsDown();
		void systemIsUp();
		/// @}
		
		bool processView(bool wait = false);

	protected:
		/// @name GUI windows
		/// @{
		StartupProgress * _progress;
		WiimoteTrackerConfigGUI * _config;
		WiimoteTrackerGUI * _gui;
		/// @}
		
		/// Controller pointer
		WiimoteTracker * _controller;
};

#endif // _WIIMOTETRACKERVIEW_H
