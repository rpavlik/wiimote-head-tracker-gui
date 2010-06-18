/** @file	main.cpp
	@brief	Main entry point for GUI app to provide tracking from a Wii Remote

	@date	2009-2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program

	See ASME paper WINVR2010-3771 for more details on this system.
*/
/*
	Copyright Iowa State University 2009-2010
	Distributed under the Boost Software License, Version 1.0.
	(See accompanying file LICENSE_1_0.txt or copy at
	http://www.boost.org/LICENSE_1_0.txt)
*/

#include <iostream>
#include <string>

#include "WiimoteTracker.h"

int main(int argc, char* argv[]) {
	// Instantiate controller
	WiimoteTracker tracker;

	// Start run loop - will return when all windows closed.
	tracker.run();

	return 0;
}
