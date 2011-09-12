/**
	@file
	@brief Implementation

	@date 2011

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// Internal Includes
#include "VrpnServer.h"

// Library/third-party includes
#include <vrpn_WiiMote.h>
#include <vrpn_ConnectionPtr.h>
#include <vrpn_Tracker_WiimoteHead.h>

#include <vrpn_QAnalogRemote.h>

// Standard includes
#include <cassert>

VrpnServer::VrpnServer(QObject *parent)
	: QObject(parent)
	, _trackerName(QLatin1String("Tracker0"))
	, _wiimoteName(QLatin1String("WiiMote0"))
	, _port(vrpn_DEFAULT_LISTEN_PORT_NO)
	, _ledDistance(0.18) {


}


void VrpnServer::startServer() {
	_container.clear();

	vrpn_ConnectionPtr connection = vrpn_ConnectionPtr::create_server_connection(_port);
	assert(connection);
	_container.add(connection);

	vrpn_WiiMote * wiimote = new vrpn_WiiMote(_wiimoteName.toLatin1(), connection.get(), 0, 1, 1, 1);
	_container.add(wiimote);

	QString wiimoteRemoteName = QLatin1String("*") + _wiimoteName;
	float trackerFrequency = 60.0f;
	vrpn_Tracker_WiimoteHead * tracker = new vrpn_Tracker_WiimoteHead(_trackerName.toLatin1(),
	        connection.get(),
	        wiimoteRemoteName.toLatin1(),
	        trackerFrequency,
	        _ledDistance);
	_container.add(tracker);

	vrpn_QAnalogRemote * anaRemote = new vrpn_QAnalogRemote(_wiimoteName, connection.get());
	connect(anaRemote, SIGNAL(analogReport(QList<double>)), this, SLOT(analogReport(QList<double>)));
	_container.add(anaRemote);

	_container.start();
}

void VrpnServer::startServer(QString const& tracker_name, QString const& wiimote_name, int port, float led_distance) {
	if (!tracker_name.isEmpty()) {
		_trackerName = tracker_name;
	}
	if (!wiimote_name.isEmpty()) {
		_wiimoteName = wiimote_name;
	}
	if (port > 0) {
		_port = port;
	}
	if (led_distance > 0) {
		_ledDistance = led_distance;
	}

}

void VrpnServer::analogReport(QList<double> channels) {
	emit gotBatteryLevel(channels[0]);
}
