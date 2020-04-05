/*
File: Input.cpp
Sample: XInput
Author: Jonathan Beverly
Description: Demos basic keyboard and mouse input systems.

Notes:
The XInput sample makes use of the KeyboardState and MouseState code to handle input.  These classes
are designed specifically as a foundation for XPlatformer.   This means that the keys specified in the
Keys.h file are limited to keys that are used in the XPlatformer game.   In addition, MouseState only
handles mouse position as mouse input was not needed in the application.

The XInput sample is designed to show how to perform a series of basic tasks using the XPlatformer framework
code.  However, it is important to note that a series of samples are based on concepts and ideas from
the CS349 Interfaces course.  Specifically, the idea of Text/Polyline are derived from an example
called "eventloop".

The sample provided below is designed to show how to perform a series of basic tasks using the XGameLib library.

The following code is based on demo code provided in the CS349 course. The code has been altered to fit the XGameLib approach.
*/
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"

#include <list>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "lib/Keys.h"
#include "lib/Game.h"
#include "lib/XInfo.h"
#include "lib/Displayable.h"

/// Text
///  The text component defines a text string displayed on the screen at a coordinate.
class Text :
	public Displayable {
public:
	/// The text constructor.
	///  @x The x-coordinate of the text.
	///  @y The y-coordinate of the text.
	///  @str The string to be drawn.
	///  @color The color to draw the string.
	Text(int x, int y, string str, int color) {
		_x = x;
		_y = y;
		_text = str;
		_color = color;
	}

	/// Overloaded. Draws the Displayable component to the screen.
	virtual void draw(XInfo* xinfo, GameTime* gameTime)
	{
		// some functions are defined within xinfo, enabling quick writing to the screen
		// in this case,  drawString handles writing to the screen
		xinfo->drawString(_text, _x, _y, _color);
	}

	/// Overloaded. Updates the Displable component based on recent changes.
	virtual void update(XInfo* xinfo, GameTime* gameTime)	{ }

	/// Overloaded. Loads an asset that is needed for the component.
	virtual void load(XInfo* xinfo)	{ }

	/// Overloaded. Disposes all data that was loaded by this Displayable.
	virtual void unload(XInfo* xinfo)	{ }

	/// Overloaded. Initializes required services and loads any non-graphics resources.
	virtual void initialize(XInfo* xinfo)	{ }
private:
	// color of the text
	int _color;

	// coordinates of the text
	int _x;
	int _y;

	// string to display
	string _text;
};

/// Polyline
///  The line component defines a sequence of lines to be drawn to the screen.
class Polyline :
	public Displayable {
public:
	/// Adds a point to the polyline.
	///  @x Initial value for the x-component of the vector.
	///  @y Initial value for the y-component of the vector.
	void add_point(int x, int y) {
		XPoint p;
		p.x = x;
		p.y = y;
		points.push_back(p);
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
		xinfo->setColor(gline, ColorConstants::COLOR_GREEN);

		XSetLineAttributes(xinfo->getDisplay(), gline, 5, LineSolid, CapButt, JoinRound);
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
		XDrawLines(display, pix, gline, &points[0], points.size(), CoordModeOrigin);
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
};

/// XInput
///  An input sample that focuses on keyboard input and mouse movement to display various graphics onto the screen.
class XInput :
	public Game
{
public:
	/// Creates an instance of the XInput game.
	XInput(void)
	{
		color = 0;
	}

	/// Overloaded. Initializes required services and loads any non-graphics resources.
	virtual void initialize(XInfo* xinfo)
	{
		polyLine = new Polyline();

		addComponent(polyLine);
	}

	/// Overloaded. Updates the Displable component based on recent changes.
	virtual void update(XInfo* xinfo, GameTime* gameTime)
	{
		// retrieves the state monitor devices for both the keyboard and mouse
		KeyboardState* keyboard = xinfo->getKeyboardState();
		MouseState* mouse = xinfo->getMouseState();

		// series of actions based on the keyboard actions
		if(keyboard->isKeyDown(KEY_SPACE))
		{
			// creates a new text component with the string "SPACEBAR" and specified color
			// this is created at a location relative to the current mouse position
			Text* spaceText = new Text(mouse->getX() - 50, mouse->getY() - 2, "SPACEBAR", color);
			polyLine->add_point(mouse->getX(), mouse->getY());
			addComponent(spaceText);
		}

		// changes the color if the left arrow key is pressed
		if(keyboard->isKeyDown(KEY_LEFT))
		{
			xinfo->setColor(polyLine->getLineContext(), ColorConstants::COLOR_GREEN);
		}

		// changes the color if the right arrow key is pressed
		if(keyboard->isKeyDown(KEY_RIGHT))
		{
			xinfo->setColor(polyLine->getLineContext(), ColorConstants::COLOR_RED);
		}
	}

	/// Overloaded. Draws the Displayable component to the screen.
	virtual void draw(XInfo* xinfo, GameTime* gameTime)	{ }

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
	Polyline* polyLine;
	int color;
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

	XInput game;

	std::cout << "press Q to exit" << std::endl;
	std::cout << "press SPACE to draw at mouse point" << std::endl;
	std::cout << "press Arrow Keys to switch colors" << std::endl;
	game.run(xinfo);
}
