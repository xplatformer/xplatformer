#ifndef _INCL_GAME
#define _INCL_GAME

/// Standard libraries
#include <iostream>
#include <list>
#include <cstdlib>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>

/// X11 libraries
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysymdef.h>

/// Project components
#include "KeyboardState.h"
#include "MouseState.h"
#include "XInfo.h"
#include "Displayable.h"
#include "GameTime.h"
#include "Logger.h"
#include "Constants.h"

using namespace std;

/// Game
///	 Game is the base class for an object that can be updated/drawn to the screen.  It includes
///  additional functionality such as initialize/load/unload for a self contained component.
class Game
{
public:
	/// Draws the Displayable component to the screen.
	///  @xinfo The graphics information for game.
	///  @gameTime Time elapsed since the last call to draw.
	virtual void draw(XInfo* xinfo, GameTime* gameTime) = 0;

	/// Updates the Displable component based on recent changes.
	///  @xinfo The graphics information for game.
	///  @gameTime Time elapsed since the last call to draw.
	virtual void update(XInfo* xinfo, GameTime* gameTime) = 0;

	/// Loads an asset that is needed for the component.
	///  @xinfo The graphics information for game.
	virtual void load(XInfo* info) = 0;

	/// Disposes all data that was loaded by this Displayable.
	///  @xinfo The graphics information for game.
	virtual void unload(XInfo* xinfo) = 0;

	/// Initializes required services and loads any non-graphics resources.
	///  @xinfo The graphics information for game.
	virtual void initialize(XInfo* xinfo) = 0;

	/// Handles system level input operations for the game.
	///  @xinfo The graphics information for game.
	///  @gameTime Time elapsed since the last call to draw.
	virtual void handleSystemInput(XInfo* xinfo, GameTime* gameTime) = 0;

	/// Call this method to initialize the game, begin running the game loop, and start processing events for the game.
	///  @xinfo The graphics information for game.	
	void run(XInfo* xinfo)
	{
		XEvent event;

		int inside = 0;
		gameRunning = true;

		unsigned long gameStart = GameTime::getNow();
		unsigned long prevTime = GameTime::getNow();
		Display* dply = xinfo->getDisplay();

		Logger::application_debug(Logger::LOG_GAMEINIT);
		game_initialize(xinfo);
		Logger::application_debug(Logger::LOG_TASKDONE);

		// load assets from each component
		Logger::application_debug(Logger::LOG_ASSETLOADING);	
		game_load(xinfo);
		Logger::application_debug(Logger::LOG_ASSETLOADED);

		xinfo->openw();

		Logger::application_debug(Logger::LOG_GAMESTART);
		while(gameRunning)
		{
			GameTime* gameTime = new GameTime(prevTime, gameStart);
			// handle all the events currently in the queue

			// Although this could possibly block (unending event list) it is
			// unlikely and more of a stress case than a real world scenario
			// At least for the purposes of this assignment
			while(XPending(dply) > 0)
			{
				XNextEvent(dply, &event);
				switch(event.type)
				{
				case KeyRelease:
					handleKeyRelease(xinfo, &event);
					break;
				case KeyPress:
					handleKeyPress(xinfo, &event);
					break;
				case MotionNotify:
					handleMotion(xinfo, &event, inside);
					break;
				case EnterNotify:
					inside = 1;
					break;
				case LeaveNotify:
					inside = 0;
					break;
				case ConfigureNotify:
					handleResize(xinfo, &event);
					break;
				}
			}

			// sleep	
			xinfo->wait(FPS_COEFFICIENT / fps);

			game_update(xinfo, gameTime);
			game_draw(xinfo, gameTime);

			// flush buffer to display
			xinfo->flush();

			// record previous time for game time object
			prevTime = gameTime->getCurrentTime();

			handleSystemInput(xinfo, gameTime);

			free(gameTime);
		}
		Logger::application_debug(Logger::LOG_GAMEEND);

		// unloads assets from each component
		Logger::application_debug(Logger::LOG_ASSETRELEASING);
		game_unload(xinfo);
		Logger::application_debug(Logger::LOG_ASSETRELEASED);

		XCloseDisplay(xinfo->getDisplay());
	}

	/// Setting properties in the game.
	///  @argc The number of arguments given to the game.
	///  @argv The string values of the game.
	void setByCommand(int argc, char *argv[]);

	/// Return true if the game is currently running, false otherwise.
	///  @returns True if running, false otherwise.
	bool isRunning(void)
	{
		return gameRunning;
	}

	/// Terminates execution of the game.
	void quit(void)
	{
		gameRunning = false;
	}

	/// Returns the designated frames per secocond metric of the game.
	///  @returns The frame per second metric.
	int getFps(void)
	{
		return fps;
	}

	/// Sets the frame per second metric of the game.
	///  @value The value to set.
	void setFps(int value)
	{
		fps = value;
	}

	/// Adds a Displayable component to the game.
	///  @displayable The component to add to the game.
	void addComponent(Displayable* displayable)
	{
		// when a component is added, it is then called in the game_ methods.
		components.push_front(displayable);
	}

private:
	static const unsigned long FPS_COEFFICIENT = 1000000;

	/// Draws the Game component to the screen.
	void game_draw(XInfo* xinfo, GameTime* gameTime)
	{
		xinfo->clear();

		draw(xinfo, gameTime);

		list<Displayable *>::const_iterator begin = components.begin();
		list<Displayable *>::const_iterator end = components.end();

		while( begin != end )
		{
			Displayable *d = *begin;
			d->draw(xinfo, gameTime);
			begin++;
		}
	}

	/// Updates the Game component based on recent changes.
	void game_update(XInfo* xinfo, GameTime* gameTime)
	{
		update(xinfo, gameTime);

		list<Displayable*>::const_iterator begin = components.begin();
		list<Displayable*>::const_iterator end = components.end();

		while(begin != end)
		{
			Displayable *d = *begin;		
			d->update(xinfo, gameTime);
			begin++;
		}
	}

	/// Loads assets that are needed for the Game.
	void game_load(XInfo* xinfo)
	{
		load(xinfo);

		list<Displayable*>::const_iterator begin = components.begin();
		list<Displayable*>::const_iterator end = components.end();

		while( begin != end )
		{
			Displayable *d = *begin;
			d->load(xinfo);
			begin++;
		}
	}

	/// Disposes all data that was loaded by this Game.
	void game_unload(XInfo* xinfo)
	{
		list<Displayable*>::const_iterator begin = components.begin();
		list<Displayable*>::const_iterator end = components.end();

		while( begin != end )
		{
			Displayable *d = *begin;
			d->unload(xinfo); 
			begin++;
		}

		unload(xinfo);
	}

	/// Called when the Game needs to be initialized. 
	void game_initialize(XInfo* xinfo)
	{
		setFps(Constants::DEFAULT_FPS);

		initialize(xinfo); 

		list<Displayable*>::const_iterator begin = components.begin();
		list<Displayable*>::const_iterator end = components.end();

		while(begin != end)
		{
			Displayable *d = *begin;
			d->initialize(xinfo);
			begin++;
		}
	}

	/// Handles motion events based on mouse input device.
	void handleMotion(XInfo* xinfo, XEvent* event, int inside)
	{
		if(inside)
		{
			xinfo->getMouseState()->setX(event->xmotion.x);
			xinfo->getMouseState()->setY(event->xmotion.y);
		}
	}

	/// Handles the window resize event.
	void handleResize(XInfo* xinfo, XEvent* event)
	{
		XConfigureEvent xce = event->xconfigure;
		Rectangle* pix = xinfo->getGraphicBounds();

		if (xce.width > pix->getWidth()|| xce.height > pix->getHeight())
		{		
			int xDiff = xce.width - pix->getWidth();
			int yDiff = xce.height - pix->getHeight();

			pix->setPoint(xDiff / 2, yDiff / 2);
		}
	}

	/// Handles a keyboard key press event.
	void handleKeyPress(XInfo* xinfo, XEvent* event)
	{
		XKeyEvent* kEvent = (XKeyEvent*)event;
		xinfo->getKeyboardState()->set((KEYS)kEvent->keycode);
	}

	/// Handles a keyboard key release event.
	void handleKeyRelease(XInfo* xinfo, XEvent* event)
	{
		XKeyEvent* kEvent = (XKeyEvent*)event;
		xinfo->getKeyboardState()->clear((KEYS)kEvent->keycode);
	}

	std::list<Displayable*> components;
	int fps;
	int border;
	int buffersize;
	char* windowTitle;
	int windowWidth;
	int windowHeight;
	bool gameRunning;
};

#endif
