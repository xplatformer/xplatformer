/*
File: XWindow.cpp
Sample: XWindow
Author: Jonathan Beverly
Description: Opens a single blank window using the XPlatformer framework.

Notes:
The XWindow sample makes use of the XInfo controller class.  The purpose of XInfo is to act as a controller
for handling a single xwindow.  It controls opening/closing/graphics and other aspects.  It removes a large 
portion of the busy work associated with XLib functions and as a wrapper.

The sample provided below is designed to show how to perform a series of basic tasks using the XGameLib library.

The following code is based on demo code provided in the CS349 course. The code has been altered to fit the XGameLib approach.
*/
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"

#include <list>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

#include "lib/XInfo.h"

/*
* Start executing here.
*	First initialize xinfo (XWindows manager).
*	Next open window and prepare for commands.
*	Use xinfo to close window.
*/
int main(int argc, char* argv[])
{
	// initializes and creates new instance of xinfo window manager
	// xinfo manages xwindow related tasks (graphics/window management)
	XInfo* xinfo = new XInfo();
	xinfo->initialize(argc, argv);

	// opens the window that is controlled by xinfo
	xinfo->openw();

	// let server get set up before sending drawing commands
	sleep(2);

	// wait for user input to quit (using simple console command)
	std::cout << "press ENTER to exit";
	std::cin.get();

	// closes the window controlled by XInfo
	xinfo->close();
}