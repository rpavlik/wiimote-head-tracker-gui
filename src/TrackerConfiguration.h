/** @file	TrackerConfiguration.h
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
#ifndef _TRACKERCONFIGURATION_H
#define _TRACKERCONFIGURATION_H

// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
#include <string>
#include <iostream>

class TrackerConfiguration {
	public:
		TrackerConfiguration(const float ledDistance = .205, const std::string & trackerName = "Tracker0");

		const float getLEDDistance() const;
		const std::string & getTrackerName() const;

	protected:
		float _ledDistance;
		std::string _trackerName;
		//friend std::istream & operator>>(std::istream & s, TrackerConfiguration & rhs);
};

std::ostream & operator<<(std::ostream & s, const TrackerConfiguration & rhs);
std::istream & operator>>(std::istream & s, TrackerConfiguration & rhs);

// -- inline implementations -- //

inline const float TrackerConfiguration::getLEDDistance() const {
	return _ledDistance;
}

inline const std::string & TrackerConfiguration::getTrackerName() const {
	return _trackerName;
}

#endif // _SYSTEMCOMPONENTS_H
