/*
File: Drawing.cpp
Sample: Drawing
Author: Jonathan Beverly
Description: Demos drawing functions and graphics contexts.

Notes:
The following is a sample that shows basic graphics usage using the helper files created for use in the XPlatformer project.
In this sample the `XInfo` is primarily used to perform common drawing functions.  `XInfo` wraps the various drawing functions
and automatically handles default management of textual and standard graphic contexts.  The sample specifically shows how to use
drawing commands, colour manipulation and line styles.

The sample provided below is designed to show how to perform a series of basic tasks using the XGameLib library.

The following code is based on demo code provided in the CS349 course. The code has been altered to fit the XGameLib approach.
*/
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"

#include <cstdlib>
#include <iostream>
#include <list>
#include <unistd.h>

#include "lib/XInfo.h"

/// draws a collection of visual components on the screen
void drawComponents(XInfo* xinfo, GC gdraw) 
{
	// gets the display information
	Display* display = xinfo->getDisplay();
	Pixmap buffer = xinfo->getImageBuffer();

	// list of silo object points
	int obj_count = 3;
	XPoint obj_points[] =
	{
		{200, 500},
		{350, 500},
		{500, 500}
	};
	int obj_styles[] =
	{
		LineSolid, 
		LineOnOffDash, 
		LineDoubleDash
	};

	// draw a simple house structure with oval roof  
	int objx, objy;
	xinfo->setColor(gdraw, ColorConstants::COLOR_RED);
	for(int i = 0; i < obj_count; i++)
	{
		XSetLineAttributes(display, gdraw, 5, obj_styles[i], CapButt, JoinRound);
		objx = obj_points[i].x;
		objy = obj_points[i].y;

		XDrawLine(display, buffer, gdraw, objx, objy, objx + 100, objy);
		XDrawLine(display, buffer, gdraw, objx, objy, objx, objy + 100);
		XDrawLine(display, buffer, gdraw, objx + 100, objy, objx + 100, objy + 100); // series of lines
		XDrawArc(display, buffer, gdraw, objx, objy - 50, 100, 100, 0, 180 * 64); // draw arc at 180 degrees
	}

	// draws a series of points that define a triangle, this shape is made of a set of consecutive lines
	int x = 110;
	int y = 250;
	XPoint points[] = {
		{x + 200, y + 50},
		{x + 250, y + 80},
		{x + 200, y + 80},
		{x + 200, y + 50}
	};
	int npoints = sizeof(points) / sizeof(XPoint);

	// draws a collection of lines
	xinfo->setColor(gdraw, ColorConstants::COLOR_GREY);
	XSetLineAttributes(display, gdraw, 5, LineSolid, CapButt, JoinRound);
	XDrawLines(display, buffer, gdraw, points, npoints, CoordModeOrigin);

	// sets the rectangle variables
	int rect_x = 150;
	int rect_y = 50;
	int rect_width = 50;
	int rect_height = 60;
	int rect_spacing = 60;

	// draws a collection of rectangles of different colors
	xinfo->setColor(gdraw, ColorConstants::COLOR_BLUE);
	XFillRectangle(display, buffer, gdraw, rect_x + rect_spacing * 1, rect_y + 50, rect_width, rect_height);
	XDrawRectangle(display, buffer, gdraw, rect_x + rect_spacing * 2, rect_y + 50, rect_width, rect_height);

	xinfo->setColor(gdraw, ColorConstants::COLOR_WHITE);
	XFillRectangle(display, buffer, gdraw, rect_x + rect_spacing * 3, rect_y + 50, rect_width, rect_height);

	xinfo->setColor(gdraw, ColorConstants::COLOR_BLUE);
	XDrawRectangle(display, buffer, gdraw, rect_x + rect_spacing * 4, rect_y + 50, rect_width, rect_height);
	XFillRectangle(display, buffer, gdraw, rect_x + rect_spacing * 5, rect_y + 50, rect_width, rect_height);
}

/*
*   Start executing here.
*	 First initialize window.
*	 Next loop responding to events.
*	 Exit forcing window manager to clean up - cheesy, but easy.
*/
int main(int argc, char* argv[]) 
{
	XInfo* xinfo = new XInfo();
	xinfo->initialize(argc, argv);
	GC gc = xinfo->getGraphicContext();

	// opens a window
	xinfo->openw();

	// let server get set up before sending drawing commands
	sleep(2);

	// grabs variables from xinfo
	Display* display = xinfo->getDisplay();
	Window win = xinfo->getWindow();
	GC gdraw = xinfo->createGraphicContext();
	xinfo->setColor(gc, ColorConstants::COLOR_BLUE);

	XWindowAttributes windowInfo;
	XGetWindowAttributes(display, win, &windowInfo);

	// max x and y coordinate
	unsigned int maxHeight = windowInfo.height - 1;
	unsigned int maxWidth = windowInfo.width - 1;

	// draw series of rectangles on the screen
	int s = 32; 
	xinfo->setColor(gdraw, ColorConstants::COLOR_GREEN);
	xinfo->drawRectangle(gdraw, 0, 0, s, s);
	xinfo->drawRectangle(gdraw, 0, maxHeight - s, s, s); 
	xinfo->fillRectangle(gdraw, maxWidth - s, 0, s, s); 
	xinfo->fillRectangle(gdraw, maxWidth - s, maxHeight - s, s, s);

	// executes the draw components
	drawComponents(xinfo, gdraw);

	// flushes the contents to the screen
	xinfo->flush();

	// wait for user input to quit (a concole event for now)
	std::cout << "press ENTER to exit";
	std::cin.get();

	// closes the xinfo window
	xinfo->close();
}
