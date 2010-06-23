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
/*
 Copyright Iowa State University 2009-2010
 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or copy at
 http://www.boost.org/LICENSE_1_0.txt)
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
class Fl_Native_File_Chooser;

class WiimoteTracker; // forward declaration of controller

class WiimoteTrackerView {
	public:
		WiimoteTrackerView(WiimoteTracker * controller);
		~WiimoteTrackerView();

		/// @name Startup methods
		/// @{
		void setProgressRanges();

		void run();
		/// @}

		/// @name System status signals
		/// @{
		void setProgress(const StartupStage stg);
		void setProgress(const TrackerComponent cmp, const double completion, const char * message, bool fail = false);

		void systemIsDown();
		void systemIsUp();

		/// @brief Marks the GUI with "please wait"
		void systemInTransition();
		/// @}

		/// @name Configuration operations
		/// @{
		void reconfigure();
		void applyNewConfiguration();

		void updateConfigDisplay();

		void saveConfig();
		void openConfig();
		/// @}

		/// @brief Call to event loop, non-blocking by default
		bool processView(bool wait = false);

	protected:
		/// @name GUI windows
		/// @{
		StartupProgress * _progress;
		WiimoteTrackerConfigGUI * _config;
		WiimoteTrackerGUI * _gui;
		Fl_Native_File_Chooser * _fc;
		/// @}

		/// Controller pointer
		WiimoteTracker * _controller;
};

#endif // _WIIMOTETRACKERVIEW_H
