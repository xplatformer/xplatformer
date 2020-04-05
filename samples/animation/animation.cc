/*
File: Animation.cpp
Sample: Animation
Author: Jonathan Beverly
Description: Demos basic animation using the framework.

Notes:
The Animation sample makes use of the Game loop to handle the process of animation. 

The sample provided below is designed to show how to perform a series of basic tasks using the XGameLib library.

The following code is based on demo code provided in the CS349 course. The code has been altered to fit the XGameLib approach.
*/
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"

#include <list>
#include <vector>
#include <cstdlib>
#include <iostream>

#include "lib/Keys.h"
#include "lib/Game.h"
#include "lib/XInfo.h"
#include "lib/Displayable.h"

/// Ball
///  The ball component defines a moving ball displayed on the screen at a coordinate.
class Ball :
	public Displayable {
public:
	/// The ball constructor.
	///  @x The x-coordinate of the ball.
	///  @y The y-coordinate of the ball.
	///  @diameter The diameter of the ball.
	Ball(int x, int y, int diameter) 
	{
		_x = x;
		_y = y;
		_diameter = diameter;
		_direction = 4;
	}

	/// Gets the x-component of the vector.
	///  @returns The horizontal component of the vector.
	int getX(void) 
	{
		return _x;
	}
		
	/// Gets the y-component of the vector.
	///  @returns The vertical component of the vector.
	int getY(void) 
	{
		return _y;
	}
	
	/// Overloaded. Initializes required services and loads any non-graphics resources.
	virtual void initialize(XInfo* xinfo)	
	{
		gball = xinfo->createGraphicContext();
		xinfo->setColor(gball, ColorConstants::COLOR_BLUE);
	}

	/// Overloaded. Draws the Displayable component to the screen.
	virtual void draw(XInfo* xinfo, GameTime* gameTime)
	{
		Display* display = xinfo->getDisplay();
		Pixmap pix = xinfo->getImageBuffer();

		XFillArc(display, pix, gball, _x, _y, _diameter, _diameter, 0, 360*64);
	}

	/// Overloaded. Updates the Displable component based on recent changes.
	virtual void update(XInfo* xinfo, GameTime* gameTime)	
	{ 
		_x = _x + _direction;
		if (random() % 100 == 0 || _x < 0 || _x > 400 - _diameter) 
		{
			_direction = -_direction;
		}
	}

	/// Overloaded. Loads an asset that is needed for the component.
	virtual void load(XInfo* xinfo)	{ }

	/// Overloaded. Disposes all data that was loaded by this Displayable.
	virtual void unload(XInfo* xinfo)	{ }
private:
	int _x;
	int _y;
	int _diameter;
	int _direction;
	GC gball;
};

/// Eye
///  The eye component defines a eye that looks a specific targets.
class Eye :
	public Displayable {
public:
	/// The eye constructor.
	///  @x The x-coordinate of the eye.
	///  @y The y-coordinate of the eye.
	///  @width The width of the eye.
	///  @height The height of the eye.
	Eye(int x, int y, int width, int height)
	{
		_x = x;
		_y = y;
		_width = width;
		_height = height;

		_viewx = x + width / 2;
		_viewy = y + height / 2;
	}

	/// The coordinate for the eye to look at.
	///  @x The horizontal position for the eye to focus on.
	///  @y The vertical position for the eye to focus on.
	void lookat(int x, int y) 
	{
		_viewx = x;
		_viewy = y;
	}

	/// Overloaded. Initializes required services and loads any non-graphics resources.
	virtual void initialize(XInfo* xinfo)
	{
		geye = xinfo->createGraphicContext();
		xinfo->setColor(geye, ColorConstants::COLOR_BLACK);
	}

	/// Overloaded. Draws the Displayable component to the screen.
	virtual void draw(XInfo* xinfo, GameTime* gameTime)
	{
		// The following variables are the display device, the image buffer and the graphics context
		// The image buffer works as a double buffer for writing to the window canvas
		// The graphics context controls how visuals are drawn to the canvas
		Display* display = xinfo->getDisplay();
		Pixmap pix = xinfo->getImageBuffer();

		// draws the eye background
		xinfo->setColor(geye, ColorConstants::COLOR_BLACK);
		XFillArc(display, pix, geye, _x, _y, _width, _height, 0, 360 * 64);

		double cx = _x + _width / 2;
		double cy = _y + _height / 2;						
		double alpha;
			
		if (_viewx > cx) 
		{
			alpha = atan((_viewy - cy) / (_viewx - cx));
		}
		else 
		{
			alpha = M_PI - atan((_viewy - cy) / (cx - _viewx));
		}

		int eyex = cx + cos(alpha) * _width / 4 - _width / 8;
		int eyey = cy + sin(alpha) * _height / 4 - _height / 8;
	
		// draws the eye pupil
		xinfo->setColor(geye, ColorConstants::COLOR_WHITE);
		XFillArc(display, pix, geye, eyex, eyey, _width / 4, _height / 4, 0, 360 * 64);
	}

	/// Overloaded. Updates the Displable component based on recent changes.
	virtual void update(XInfo* xinfo, GameTime* gameTime)	{ }

	/// Overloaded. Loads an asset that is needed for the component.
	virtual void load(XInfo* xinfo)	{ }

	/// Overloaded. Disposes all data that was loaded by this Displayable.
	virtual void unload(XInfo* xinfo)	{ }
private:
	// A vector of XPoint (native XLib point class) for drawing
	int _x;
	int _y;
	int _width;
	int _height;
	int _viewx;
	int _viewy;
	GC geye;
};

/// XAnimation
///  An animation sample that focuses on updating visuals.
class XAnimation :
	public Game
{
public:
	/// Creates an instance of the XAnimation game.
	XAnimation(void)
	{
	}

	/// Cleans up an instance of XAnimation game
	~XAnimation(void)
	{
		free(left_eye);
		free(right_eye);
		free(ball);
	}

	/// Overloaded. Initializes required services and loads any non-graphics resources.
	virtual void initialize(XInfo* xinfo)
	{
		left_eye = new Eye(100, 100, 100, 200);
		right_eye = new Eye(225, 100, 100, 200);
		ball = new Ball(100, 450, 40);

		is_within = false;
		time_swap = 0.0f;

		addComponent(left_eye);
		addComponent(right_eye);
		addComponent(ball);
	}

	/// Overloaded. Updates the Displable component based on recent changes.
	virtual void update(XInfo* xinfo, GameTime* gameTime)
	{
		// retrieves the state monitor devices for both the keyboard and mouse
		KeyboardState* keyboard = xinfo->getKeyboardState();
		MouseState* mouse = xinfo->getMouseState();

		// updates eyes to look at mouse or ball coordinate
		int mx = 0,	my = 0;
		if(is_within)
		{
			mx = mouse->getX();
			my = mouse->getY();
		} 
		else 
		{
			mx = ball->getX();
			my = ball->getY();
		}		

		// looks at coordinate
		left_eye->lookat(mx, my);
		right_eye->lookat(mx, my);

		// changes to look at either ball or mouse
		time_swap -= gameTime->getElapsedDelta();	
		if(keyboard->isKeyDown(KEY_SPACE) && time_swap <= 0.0f)
		{
			time_swap = 5.0f;
			is_within = !is_within;
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
	Eye* left_eye;
	Eye* right_eye;
	Ball* ball;
	bool is_within;
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

	XAnimation game;

	std::cout << "press Q to exit" << std::endl;
	std::cout << "press SPACE to switch look targets" << std::endl;
	game.run(xinfo);
}
