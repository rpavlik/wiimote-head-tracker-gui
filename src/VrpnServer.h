/** @file
	@brief Header

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

#pragma once
#ifndef INCLUDED_VrpnServer_h_GUID_01d1ba2b_8769_470a_8ff6_29b83972d00f
#define INCLUDED_VrpnServer_h_GUID_01d1ba2b_8769_470a_8ff6_29b83972d00f

#ifndef NDEBUG
#define VRPN_MAINLOOPOBJECT_VERBOSE
#endif

// Internal Includes
// - none

// Library/third-party includes
#include <QObject>
#include <vrpn_QMainloopContainer.h>

// Standard includes
// - none

class VrpnServer : public QObject {
		Q_OBJECT
	public:
		VrpnServer(QObject * parent = NULL);
		~VrpnServer() {}

		QString trackerName() const {
			return _trackerName;
		}

		QString wiimoteName() const {
			return _wiimoteName;
		}

		int port() const {
			return _port;
		}

		float ledDistance() const {
			return _ledDistance;
		}

	public slots:
		void startServer();
		void stopServer();

	private slots:
		void analogReport(QList<double> channels);

	signals:
		void gotBatteryLevel(double level);
		void starting();
		void started();
		void stopped();

	private:
		QString _trackerName;
		QString _wiimoteName;
		int _port;
		float _ledDistance;

		vrpn_QMainloopContainer _container;
};

#endif // INCLUDED_VrpnServer_h_GUID_01d1ba2b_8769_470a_8ff6_29b83972d00f
