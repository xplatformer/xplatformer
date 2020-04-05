#include <iostream>
#include <list>
#include <cstdlib>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysymdef.h>

#include "lib/XInfo.h"
#include "lib/KeyboardState.h"
#include "lib/MouseState.h"
#include "lib/Displayable.h"
#include "lib/Game.h"
#include "lib/Constants.h"
#include "lib/Logger.h"

#include "SkyComponent.h"
#include "SkyComponent.h"
#include "WorldComponent.h"
#include "PlayerComponent.h"
#include "Levels.h"
#include "Resources.h"
#include "GameConstants.h"

using namespace std;

/// XPlatformer
///	 Game is the base class for an object that can be updated/drawn to the screen.  It includes
///  additional functionality such as initialize/load/unload for a self contained component.
class XPlatformer :
	public Game
{
public:
	/// Initializes a new instance of Game.
	XPlatformer(void)
	{
	}

	/// Disposes of the Game instance.
	~XPlatformer(void)
	{
	}

	/// Draws the Displayable component to the screen.
	///  @xinfo The graphics information for game.
	///  @gameTime Time elapsed since the last call to draw.
	virtual void draw(XInfo* xinfo, GameTime* gameTime) 
	{
		// if in paused state, draw to screen and wait
		if(isPaused)
		{
			handleHaultMenu(xinfo, gameTime);
		}
		else
		{
		}
	}

	/// Updates the Displable component based on recent changes.
	///  @xinfo The graphics information for game.
	///  @gameTime Time elapsed since the last call to draw.
	virtual void update(XInfo* xinfo, GameTime* gameTime) 
	{
		// if in paused state, nothing to update
		if(!isPaused)
		{
			//Stuff
		}
	}

	/// Loads an asset that is needed for the component.
	///  @xinfo The graphics information for game.
	virtual void load(XInfo* xinfo) 
	{
	}

	/// Disposes all data that was loaded by this Displayable.
	///  @xinfo The graphics information for game.
	virtual void unload(XInfo* xinfo)
	{
	}

	/// Initializes required services and loads any non-graphics resources.
	///  @xinfo The graphics information for game.
	virtual void initialize(XInfo* xinfo)
	{		
		sky = new SkyComponent();
		world = new WorldComponent(GameConstants::BG_GRASSLANDS , 12, 9);
		player = new PlayerComponent(world);

		addComponent(player);
		addComponent(sky);
		addComponent(world);

		world->clear();
		Levels::setLevel(xinfo, *world, level);
	}

	/// Setting properties in the game.
	///  @argc The number of arguments given to the game.
	///  @argv The string values of the game.
	void setByCommand(int argc, char *argv[])		
	{
		///Fake Parameters for testing
		//    char* arg[4];
		//    arg[0] = "--sun=10.0";
		//    arg[1] = "--fps=30";
		//    arg[2] = "--jump=22.5";
		//    arg[3] = "--move=7.5";

		// Arguments
		if(argc == 0 || argc == 1)
		{
			Logger::application_debug(Logger::LOG_ARGINVALID);
			return;
		}

		Logger::application_debug(Logger::LOG_ARGCOUNT, argc);
		for(int i = 0; i < argc; i++)
		{
			// Below code creates a string, finds the
			// index of '='  (to separate command and value)
			// and then evaluates what command/value it is

			std::string cmdparam(argv[i]);
			const char* cmd = cmdparam.c_str();
			int eq = strchr(cmd, '=') - cmd + 1;
			if (eq < 0 || eq >= (int)cmdparam.length())
			{
				continue;
			}

			std::string param = cmdparam.substr (eq);
			if(cmdparam.find("--fps=") == 0)
			{			
				int fpsvalue = atoi(param.c_str());
				Constants::DEFAULT_FPS = fpsvalue;
				Logger::application_info(Logger::INFO_FPS, fpsvalue);
			}
			else
			{
				float value = atof(param.c_str());

				//(cmdparam.find("--jump=") == 0 || cmdparam.find("--j=") == 0)
				//(cmdparam.find("--move=") == 0 || cmdparam.find("--m=") == 0)
				if(isCommand(cmdparam, "--move=", "--m="))
				{
					Logger::application_info(Logger::INFO_MOVE, value);
					GameConstants::PLAYER_DEFAULT_SPEED = value;
				}
				else if(isCommand(cmdparam, "--jump=", "--j="))
				{
					Logger::application_info(Logger::INFO_JUMP, value);
					GameConstants::PLAYER_DEFAULT_JUMP = value;
				}
				else if(cmdparam.find("--sun=") == 0)
				{
					Logger::application_info(Logger::INFO_SUN, value);
					GameConstants::DEFAULT_SKY_SPEED = value;
				}
			}
		}
	}

	/// Overloaded. Handles system level input operations for the game.
	virtual void handleSystemInput(XInfo* xinfo, GameTime* gameTime)
	{	
		///if Q pressed quit
		if(xinfo->getKeyboardState()->isKeyDown(KEY_Q))
		{
			quit();
		}

		///if space pressed pause/resume
		if(xinfo->getKeyboardState()->isKeyDown(KEY_SPACE))
		{
			isPaused = !isPaused;

			//clear the key so it requires another space bar press to activate
			xinfo->getKeyboardState()->clear(KEY_SPACE);
		}

		//if still running and conditions for new level met, move forward
		if(isRunning() && (world->getObjectiveCount() == 0 || player->isDead()))
		{
			handleLevelShift(xinfo, gameTime, player->isDead() ? 1 : 0);
		}
	}

private:
	WorldComponent* world;
	PlayerComponent* player;
	SkyComponent* sky;
	int level = 0;
	bool isPaused;

	/// Determines if the command is within the string.
	///  @cmdparam The command parameter string.
	///  @value1 The command to search for
	///  @value2 The command for search for
	bool isCommand(std::string cmdparam, const char* value1, const char* value2)
	{
		return cmdparam.find(value1)  == 0 || cmdparam.find(value2)  == 0;
	}

	/// Function to perform loading screen and level update.
	void handleLevelShift(XInfo* xinfo, GameTime* gameTime, int type)
	{
		///Starts a loop that runs for X seconds (X wait intervals)
		int waitCount = 3;
		int increment = 0;
		while(waitCount > 0)
		{
			//draw the screen
			//draw(xinfo, gameTime);

			//Draw a loading message over it
			std::stringstream ss;
			switch(type)
			{
			case 1:
				ss << "You died ";
				break;
			default:
				ss << "Loading next world ";
				break;
			}

			for(int i = 0; i < increment; i++)
			{
				ss << ".";
			}
			std::string text = ss.str();

			increment = (increment + 1) % 4;

			xinfo->drawString(text,
				xinfo->getGraphicBounds()->getWidth() / 2 - 175,
				160,
				16766720);

			xinfo->flush();

			//wait for a second to ensure slow 'loading' transition
			xinfo->wait(1);
			waitCount--;
		}

		// resets the player
		player->reset();

		//update level information
		level = (level + 1);
		if(level > LEVEL_COUNT)
		{
			level = 1;
		}

		Levels::setLevel(xinfo, *world, level);
		Logger::application_info("Shifting to new level", level);
	}

	/// Function for displaying pause menu information.
	void handleHaultMenu(XInfo* xinfo, GameTime* gameTime)
	{
		Display* _display = xinfo->getDisplay();
		GC _gc = xinfo->getGraphicContext();
		int _screen = xinfo->getScreen();
		Pixmap _buffer = xinfo->getImageBuffer();

		unsigned long black = BlackPixel(_display, _screen);
		unsigned long white = WhitePixel(_display, _screen);

		//Sets colors and draw rectangle
		XSetForeground(_display, _gc, black);
		XSetBackground(_display, _gc, black);
		XFillRectangle(_display, _buffer, _gc, 150, 25, 500, 200);

		//set colour to white, draw inner rectangle
		XSetForeground(_display, _gc, white);
		XFillRectangle(_display, _buffer, _gc, 155, 30, 490, 190);

		//draw a series of text within the rectangle

		std::string infoTxt(Resources::ASSET_INFO_SPACE);
		xinfo->drawString(infoTxt, 180, 70, black);

		std::string arrowTxt(Resources::ASSET_INFO_MOVEMENT);
		xinfo->drawString(arrowTxt, 180, 120, black);

		std::string jumpTxt(Resources::ASSET_INFO_JUMP);
		xinfo->drawString(jumpTxt, 180, 160, black);

		std::string actionTxt(Resources::ASSET_INFO_ACTION);
		xinfo->drawString(actionTxt, 180, 200, black);
	}
};

/*
Entry point of the application.
* Initializes the XInfo manager
* Updates game by setting commands from command line
* Runs the game
*/
int main(int argc, char *argv[])
{
	XInfo* xinfo = new XInfo();
	XPlatformer game;

	xinfo->setIcon(Resources::ASSET_ICON);
	xinfo->initialize(argc, argv);
	game.setByCommand(argc, argv);
	game.run(xinfo);
}
