/** @file	SoftwareVersions.h
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
#ifndef _SOFTWAREVERSIONS_H
#define _SOFTWAREVERSIONS_H

// Internal Includes
// - none

// Library/third-party includes
#include <FL/Enumerations.H>

// Standard includes
#include <string>
#include <sstream>

// Extern for VRPN version
extern const char *vrpn_MAGIC;

// Declaration for WiiUse version function
extern "C" {
	const char* wiiuse_version();
}

class SoftwareVersions {
	public:
		SoftwareVersions() :
				_appVer("Version " TRACKER_APP_VERSION),
				_vrpnVer(vrpn_MAGIC),
				_wiiuseVer("WiiUse: version "),
				_fltkVer("FLTK: version " ) {
			_wiiuseVer += wiiuse_version();
			std::ostringstream s;
			s << FL_MAJOR_VERSION << "." << FL_MINOR_VERSION << "." << FL_PATCH_VERSION;
			_fltkVer += s.str();
		}

	protected:
		std::string _appVer;
		std::string _vrpnVer;
		std::string _wiiuseVer;
		std::string _fltkVer;

		friend class WiimoteTrackerGUI;
};
#endif // _SOFTWAREVERSIONS_H
