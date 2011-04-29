/** @file
	@brief

	@date 2011

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/
/*
 Copyright Iowa State University 2011
 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or copy at
 http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef INCLUDED_launchByAssociation_h_GUID_2797fe05_9123_4e96_95fc_48f4cd51828f
#define INCLUDED_launchByAssociation_h_GUID_2797fe05_9123_4e96_95fc_48f4cd51828f

// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
#include <string>
#include <sstream>
#include <cstdlib>

namespace util {

/// @addtogroup FreeFunctions Free Functions
/// @{

	/// @brief In a platform-specific way, launch some document
	/// or location in the default application for doing such things.
	int launchByAssociation(std::string const& location) {
#if defined(_WIN32)
		static const std::string LAUNCH_CMD = "start \"launchByAssociation\" ";
#elif defined(__APPLE__)
		static const std::string LAUNCH_CMD = "open ";
#else
		// Assume we're on a freedesktop.org system with xdg-utils
		static const std::string LAUNCH_CMD = "xdg-open ";
#endif
		std::ostringstream cmd;
		cmd << LAUNCH_CMD << location;
		return system(cmd.str().c_str());
	}

/// @}

} // end of util namespace

#endif // INCLUDED_launchByAssociation_h_GUID_2797fe05_9123_4e96_95fc_48f4cd51828f
