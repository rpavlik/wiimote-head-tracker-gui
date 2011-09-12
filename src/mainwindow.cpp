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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "VrpnServer.h"

// Library/third-party includes
// - none

// Standard includes
// - none

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, _server(new VrpnServer) {
	ui->setupUi(this);

	connect(_server.data(), SIGNAL(gotBatteryLevel(double)), this, SLOT(gotBatteryLevel(double)));
	_server->startServer();
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::gotBatteryLevel(double level) {
	ui->batteryBar->setValue(int(level * 100));
}
