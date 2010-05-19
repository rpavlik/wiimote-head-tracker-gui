Wii Remote Head Tracker (with Integrated GUI)
---------------------------------------------

http://www.github.com/rpavlik/wiimote-headtracker-gui

This application builds on the Wii Remote Head Tracking filter module for VRPN
by providing an easy-to-use, fully-integrated tracking server.

Please see ASME paper WINVR2010-3771 for more information on the tracking module
and the system's design.

If you use and appreciate the tracking server, I'd enjoy hearing from you. If
you've run into issues or have enhancements, you are also welcome to email me:
 Ryan Pavlik <rpavlik@iastate.edu> <abiryan@ryand.net>

Build Dependencies
------------------
- VRPN with Ryan's Tracker_WiimoteHead device (19 May 2010 or newer): VRPN 07.27
  might be compatible, VRPN 07.28 will likely be compatible, or go to
  http://www.github.com/rpavlik/vrpn
  (Public domain + BSL 1.0)
- WiiUse - needed for VRPN (GPL3)
- FLTK 1.1.x (LGPL2 with exceptions)
- Fl_Native_File_Chooser (source included with this package) (LGPL2 with exceptions)
- CMake 2.6 or newer, to generate build files

License (for tracking module and GUI source)
--------------------------------------------
Copyright Iowa State University 2009-2010
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)