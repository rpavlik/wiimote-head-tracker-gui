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
#ifndef INCLUDED_mainwindow_h_GUID_8ea3b0c5_933e_4bac_9549_985843a922ea
#define INCLUDED_mainwindow_h_GUID_8ea3b0c5_933e_4bac_9549_985843a922ea

// Internal Includes
// - none

// Library/third-party includes
#include <QMainWindow>

// Standard includes
// - none

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	private:
		Ui::MainWindow *ui;
};

#endif // INCLUDED_mainwindow_h_GUID_8ea3b0c5_933e_4bac_9549_985843a922ea
