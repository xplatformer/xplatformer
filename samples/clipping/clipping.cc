/*
File: Clipping.cpp
Sample: Clipping
Author: Jonathan Beverly
Description: Demos basic keyboard and mouse input systems.

Notes:
The XInput sample makes use of the KeyboardState and MouseState code to handle input.  These classes
are designed specifically as a foundation for XPlatformer.   This means that the keys specified in the
Keys.h file are limited to keys that are used in the XPlatformer game.   In addition, MouseState only
handles mouse position as mouse input was not needed in the application.

The sample provided below is designed to show how to perform a series of basic tasks using the XGameLib library.

The following code is based on demo code provided in the CS349 course. The code has been altered to fit the XGameLib approach.
*/
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"

#include <list>
#include <math.h>
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <sys/time.h>

#include "lib/Keys.h"
#include "lib/Game.h"
#include "lib/XInfo.h"
#include "lib/Displayable.h"

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

/// Pattern
///  The line component defines a sequence of lines to be drawn to the screen.
class Pattern :
	public Displayable {
public:
	/// Returns a random integer that is within a specified range.
	///  @minValue The inclusive lower bound of the random number returned.
	///  @maxValue The inclusive upper bound of the random number returned. maxValue must be greater than or equal to minValue.
	///  @returns A integer greater than or equal to minValue and less than or equal to maxValue.
	int rand_next(int minValue, int maxValue)
	{
		return minValue + (maxValue - minValue) * (double)rand() / RAND_MAX;
	}

	/// Returns the graphics context of the polyline component.
	///  @returns A graphic context.
	GC getLineContext(void)
	{
		return gline;
	}

	/// Overloaded. Initializes required services and loads any non-graphics resources.
	virtual void initialize(XInfo* xinfo)
	{
		gline = xinfo->createGraphicContext();
		xinfo->setColor(gline, ColorConstants::COLOR_BLACK);

		XSetLineAttributes(xinfo->getDisplay(), gline, 5, LineSolid, CapButt, JoinRound);

		// constrains the pattern to a rectangle around the screen
		XSizeHints hints = xinfo->getWindowHints();
		count = 20;
		int bounds = 250;
		max_x = (int)hints.width - bounds;
		max_y = (int)hints.height - bounds;

		// creates a random series of points to be drawn on the screen
		for (int i = 0; i < count; i++) 
		{
			XPoint p;
			p.x = rand_next(bounds, max_x);
			p.y = rand_next(bounds, max_y);

			points.push_back(p);
		}	
	}

	/// Overloaded. Draws the Displayable component to the screen.
	virtual void draw(XInfo* xinfo, GameTime* gameTime)
	{
		// The following variables are the display device, the image buffer and the graphics context
		// The image buffer works as a double buffer for writing to the window canvas
		// The graphics context controls how visuals are drawn to the canvas
		Display* display = xinfo->getDisplay();
		Pixmap pix = xinfo->getImageBuffer();

		// draw a series of lines
		xinfo->setColor(gline, ColorConstants::COLOR_BLACK);
		XDrawLines(display, pix, gline, &points[0], points.size(), CoordModeOrigin);

		// draw code from Drawing sample
		drawComponents(xinfo, gline);
	}

	/// Overloaded. Updates the Displable component based on recent changes.
	virtual void update(XInfo* xinfo, GameTime* gameTime)	{ }

	/// Overloaded. Loads an asset that is needed for the component.
	virtual void load(XInfo* xinfo)	{ }

	/// Overloaded. Disposes all data that was loaded by this Displayable.
	virtual void unload(XInfo* xinfo)	{ }
private:
	// A vector of XPoint (native XLib point class) for drawing
	vector<XPoint> points;
	GC gline;
	int max_x;
	int max_y;
	int count;
};

/// XClipping
///  A clipping sample that focuses on clipping masks on various graphics onto the screen.
class XClipping :
	public Game
{
public:
	/// Creates an instance of the XClipping game.
	XClipping(void)
	{
	}

	/// Overloaded. Initializes required services and loads any non-graphics resources.
	virtual void initialize(XInfo* xinfo)
	{
		pattern = new Pattern();
		clip_rect.x = 0;
		clip_rect.y = 0;
		clip_rect.width = 64;
		clip_rect.height = 64;

		is_clipping = false;
		time_swap = 0.0f;

		goutline = xinfo->createGraphicContext();
		xinfo->setColor(goutline, ColorConstants::COLOR_BLACK);

		addComponent(pattern);
	}

	/// Overloaded. Updates the Displable component based on recent changes.
	virtual void update(XInfo* xinfo, GameTime* gameTime)
	{
		// retrieves the state monitor devices for both the keyboard and mouse
		KeyboardState* keyboard = xinfo->getKeyboardState();
		MouseState* mouse = xinfo->getMouseState();

		// display visuals
		Display* display = xinfo->getDisplay();		
		GC gline = pattern->getLineContext();

		// updates the clipping rectangle with mouse coordinates
		// it is important to note that the rectangle will be behind the speed of mouse
		// this is because of the update loop design in the Game class (event handling & sleep time)
		clip_rect.x = mouse->getX();
		clip_rect.y = mouse->getY();

		// time_swap is used to ensure that the screen does not flicker
		// when switching between clipping states (clipping and not clipping)
		time_swap -= gameTime->getElapsedDelta();	

		// series of actions based on the keyboard actions
		if(keyboard->isKeyDown(KEY_SPACE) && time_swap <= 0.0f)
		{
			time_swap = 5.0f;
			if (is_clipping)
			{		
				XSetClipMask(display, gline, None);
				is_clipping = false;				
			} 
			else 
			{								
				is_clipping = true;
			}
		}	

		if(is_clipping)
		{		
			// if clipping, set clip rectangles (multiple can be set, only one in this case)			
			XSetClipRectangles(display, gline, 0, 0, &clip_rect, 1, Unsorted);
		}
	}

	/// Overloaded. Draws the Displayable component to the screen.
	virtual void draw(XInfo* xinfo, GameTime* gameTime)	
	{
		// draws a rectangle within the bounds of the clipping rectangle
		// this allows the user to see the range on the clipping
		if(is_clipping)
		{
			xinfo->drawRectangle(goutline, clip_rect.x + 1, clip_rect.y + 1, clip_rect.width - 1, clip_rect.height - 1);
		}		
	}

	/// Overloaded. Loads an asset that is needed for the component.
	virtual void load(XInfo* xinfo)	{ }

	/// Overloaded. Disposes all data that was loaded by this Displayable.
	virtual void unload(XInfo* xinfo)	{ }

	/// Overloaded. Handles system level input operations for the game.
	virtual void handleSystemInput(XInfo* xinfo, GameTime* gameTime)
	{
		// quit the game with 'q' is pressed
		if(xinfo->getKeyboardState()->isKeyDown(KEY_Q))
		{
			quit();
		}
	}
private:
	Pattern* pattern;
	XRectangle clip_rect;
	GC goutline;
	bool is_clipping;
	float time_swap;
};

/*
* Start executing here.
*	First initialize xinfo (XWindows manager).
*	Next execute the game (game run)
*	Note: game.run will automatically cleanup the game.
*/
int main (int argc, char* argv[])
{
	XInfo* xinfo = new XInfo();
	xinfo->initialize(argc, argv);

	XClipping game;

	std::cout << "press Q to exit" << std::endl;
	std::cout << "press SPACE to apply clipping mask" << std::endl;
	game.run(xinfo);
}
