/*
File: Null.cpp
Sample: Null
Author: CS349 Course
Description: A test to verify if the XWindow system is working.

The sample provided below is designed to show how to perform a series of basic tasks using the XGameLib library.

The following code is based on demo code provided in the CS349 course. The code has been altered to fit the XGameLib approach.
*/
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"

#include <cstdlib>
#include <iostream>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

Display* display;

int main() {
	// open display (using DISPLAY env var)
	display = XOpenDisplay(""); 

	if (display == NULL) {
		std::cout << "error\n";
		exit(-1);
	} else {
		std::cout << "success!\n";

		// close display
		XCloseDisplay(display);    
	}
}
