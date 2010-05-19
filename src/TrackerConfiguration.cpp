/**	@file	TrackerConfiguration.cpp
	@brief	Implementation of tracker config object

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
#include "TrackerConfiguration.h"
#include "FromString.h"

// Library/third-party includes
// - none

// Standard includes
#include <cassert>
#include <stdexcept>
#include <algorithm>

static const std::string CONFIG_FIRST_LINE("WIIMOTETRACKERCONFIG");
static const std::string CONFIG_SECOND_LINE("DONOTEDITBYHAND");

struct InvalidLEDDistance : public std::invalid_argument {
	InvalidLEDDistance() : std::invalid_argument("LED distance must be positive and less than 1 meter") {}
};

struct InvalidTrackerName : public std::invalid_argument {
	InvalidTrackerName() : std::invalid_argument("Tracker name must be alphanumeric with no spaces") {}
};

struct NotAConfig : public std::runtime_error {
	NotAConfig() : std::runtime_error("Not a Wii Remote Head Tracker configuration!") {}
};

TrackerConfiguration::TrackerConfiguration(const float ledDistance, const std::string & trackerName) :
		_ledDistance(ledDistance),
		_trackerName(trackerName) {
	if (_ledDistance <= 0.0 || _ledDistance > 1.0) {
		throw InvalidLEDDistance();
	}

	if (_trackerName.size() == 0 || _trackerName.find(" ") != std::string::npos) {
		throw InvalidTrackerName();
	}
}

std::ostream & operator<<(std::ostream & s, const TrackerConfiguration & rhs) {
	s << CONFIG_FIRST_LINE << std::endl;
	s << CONFIG_SECOND_LINE << std::endl;
	s << rhs.getLEDDistance() << std::endl;
	s << rhs.getTrackerName() << std::endl;
}
std::istream & operator>>(std::istream & s, TrackerConfiguration & rhs) {
	std::string line;

	std::getline(s, line);
	if (line != CONFIG_FIRST_LINE) {
		throw NotAConfig();
	}

	std::getline(s, line);
	if (line != CONFIG_SECOND_LINE) {
		throw NotAConfig();
	}

	float ledDistance;
	std::string trackerName;

	std::getline(s, line);
	//line >> ledDistance;
	//s >> ledDistance;
	bool ret = fromString<float>(ledDistance, line);
	if (!ret) {
		throw NotAConfig();
	}
	std::getline(s, trackerName);
	std::remove(trackerName.begin(), trackerName.end(), ' ');
	std::remove(trackerName.begin(), trackerName.end(), '\n');
	std::remove(trackerName.begin(), trackerName.end(), '\r');

	rhs = TrackerConfiguration(ledDistance, trackerName);
}
