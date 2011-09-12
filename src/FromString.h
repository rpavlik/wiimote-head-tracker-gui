/** @file	FromString.h
	@brief	header for converting strings to anything

	Based on http://www.codeguru.com/forum/showthread.php?t=231054

	@date	2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

#pragma once
#ifndef _FROMSTRING_H
#define _FROMSTRING_H

// Internal Includes
// - none

// Library/third-party includes
#include <FL/Enumerations.H>

// Standard includes
#include <string>
#include <sstream>
#include <iostream>

/** @brief Convert a string to a numeric type T
	@param[out] t Output variable
	@param[in] s Input string
	@param[in] f Base (one of std::hex, std::dec, or std::oct)

	@returns true if conversion succeeded.
*/
template <class T>
bool fromString(T& t,
                const std::string& s,
                std::ios_base & (*f)(std::ios_base&) = std::dec) {
	std::istringstream iss(s);
	return !(iss >> f >> t).fail();
}

#endif // _FROMSTRING_H
