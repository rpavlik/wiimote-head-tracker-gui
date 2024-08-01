Wii Remote Head Tracker (with Integrated GUI)
---------------------------------------------

http://www.github.com/rpavlik/wiimote-head-tracker-gui 

Status
------

This should still work fine, but I have no capacity to maintain this
in any reasonable fashion. You are welcome to fork and build on it
pursuant to the license, but I will not be able to help with it.

**This repo is unmaintained**


Introduction
------------

This application builds on the Wii Remote Head Tracking filter module 
for VRPN by providing an easy-to-use, fully-integrated tracking server. 

Please acknowledge use of this tracking system in academic papers and 
publications by citing the following paper:

  Pavlik, R.A., and Vance, J.M., 2010, "A Modular Implementation of Wii 
  Remote Head Tracking for Virtual Reality," ASME Paper No. 
  WINVR2010-3771. Proc. 2010 World Conference on Innovative Virtual 
  Reality. 


Build Dependencies
------------------

Build instructions (primarily tailored toward Linux) can be found in INSTALL.
The process for Windows is very similar (cmake-gui will probably be more
comfortable than command line, and you'll get Visual Studio solution files
to build instead of makefiles).  (Plus, there's binaries for Windows, so
building isn't as common.)

- CMake 2.6 or newer (the latest version is recommended), to generate build
files for VRPN and this app

- These dependencies can be built automatically by the build system:

  - VRPN with Rylie's Tracker_WiimoteHead device (19 May 2010 or newer): 
  VRPN 07.27 might be compatible, VRPN 07.28 will likely be compatible, or 
  go to http://www.github.com/vrpn/vrpn (Public domain + BSL 1.0)

  - WiiUse - needed for VRPN (GPL3)

  - FLTK 1.1.x and FLUID UI designer/compiler (LGPL2 with exceptions) 

  - Fl_Native_File_Chooser (source included with this package) (LGPL2 with 
  exceptions) 



License (for tracking module and GUI source)
--------------------------------------------
Copyright Iowa State University 2009-2010
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)

See all_licenses.txt for a compilation of licenses for all software used 
by this application. 
