/** @file	SystemComponents.h
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
#ifndef _SYSTEMCOMPONENTS_H
#define _SYSTEMCOMPONENTS_H

// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
// - none

enum TrackerComponent {
	CMP_CONNECTION,
	CMP_WIIMOTE,
	CMP_TRACKER,
	CMP_CLIENT
};

enum StartupStage {
	STG_NOT_STARTED,
	STG_CONNECTION_STARTING,
	STG_CONNECTION_FAILED,
	STG_CONNECTION_RUNNING,
	STG_WIIMOTE_STARTING,
	STG_WIIMOTE_ALLOCATE_FAILED,
	STG_WIIMOTE_CONNECT_FAILED,
	STG_WIIMOTE_RUNNING,
	STG_TRACKER_STARTING,
	STG_TRACKER_ALLOCATE_FAILED,
	STG_TRACKER_RUNNING,
	STG_CLIENT_STARTING,
	STG_CLIENT_ALLOCATE_FAILED,
	STG_CLIENT_RUNNING,
	STG_STARTUP_COMPLETE
};

#endif // _SYSTEMCOMPONENTS_H
