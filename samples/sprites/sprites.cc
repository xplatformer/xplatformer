/*
File: Sprites.cpp
Sample: Sprites
Author: Jonathan Beverly
Description: Demos using sprite drawing functions.

Notes:
The following is a sample that shows basic sprite handling using the helper files created for use in  the XPlatformer project.  
This sample is used for the purpose of demonstrating the usage of `Spritesheet` and `XInfo` in sprite operations. Specifically 
it demonstrates how to use `XInfo` to load bitmaps or spritesheets and make use of them.  
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
#include "lib/MathHelper.h"
#include "lib/Animation.h"

const float PLAYER_SPEED = 2.0f;

enum PLAYER_ANIMATION
{
	PLAYER_CENTER = 0,
		
	PLAYER_RUN_LEFT = 1,
	PLAYER_STAND_LEFT = 2,

	PLAYER_RUN_RIGHT = 3,
	PLAYER_STAND_RIGHT = 4,
};

/// Player
///  The player component defines a character sprite displayed on the screen at a coordinate.
class Player :
	public Displayable {
public:
	/// The text constructor.
	///  @x The x-coordinate of the text.
	///  @y The y-coordinate of the text.
	///  @sprites The spritesheet for the player.
	Player(int x, int y) {
		_x = x;
		_y = y;
	}

	/// Overloaded. Draws the Displayable component to the screen.
	virtual void draw(XInfo* xinfo, GameTime* gameTime)
	{
		///Gets the player position as integers
		int plyrx = MATH::ifloor(_x);
		int plyry = MATH::ifloor(_y);

		// Set clip mask and draw from spritesheet (based on position and current animation index)
		xinfo->setMask(img_mask);
		xinfo->draw(sheet, plyrx, plyry, currentAnimIndex);
		xinfo->clearMask();
	}

	/// Overloaded. Updates the Displable component based on recent changes.
	virtual void update(XInfo* xinfo, GameTime* gameTime)
	{
		// retrieves the state monitor devices for both the keyboard and mouse
		KeyboardState* keyboard = xinfo->getKeyboardState();
		PLAYER_ANIMATION newState = PLAYER_CENTER;

		// changes the color if the left arrow key is pressed
		if(keyboard->isKeyDown(KEY_LEFT))
		{
			newState = PLAYER_RUN_LEFT;
			_x -= PLAYER_SPEED;
		}

		// changes the color if the right arrow key is pressed
		if(keyboard->isKeyDown(KEY_RIGHT))
		{
			newState = PLAYER_RUN_RIGHT;
			_x += PLAYER_SPEED;
		} 

		_x = MATH::clamp(_x, 0, 600);

		handleAnimation(newState);

		elapsedTime += gameTime->getElapsedDelta() * PLAYER_SPEED;
	}

	/// Overloaded. Loads an asset that is needed for the component.
	virtual void load(XInfo* xinfo)	
	{
		bool success = xinfo->loadImage("assets/player/spritesheet.tga", &img_player, "assets/player/spritesheet.xbm", &img_mask);
		if(!success)
		{
			Logger::application_error(Logger::LOG_ASSETERROR);
		}

		sheet = new Spritesheet(img_player, 27, 1, 1);
	}

	/// Overloaded. Disposes all data that was loaded by this Displayable.
	virtual void unload(XInfo* xinfo)	
	{

	}

	/// Overloaded. Initializes required services and loads any non-graphics resources.
	virtual void initialize(XInfo* xinfo)	
	{
		animations[PLAYER_CENTER] = new Animation(0, 0);
		animations[PLAYER_RUN_LEFT] = new Animation(3, 13);
		animations[PLAYER_STAND_LEFT] = new Animation(1, 1);
		animations[PLAYER_RUN_RIGHT] = new Animation(14, 23);
		animations[PLAYER_STAND_RIGHT] = new Animation(2, 2);

		currAnimation = animations[PLAYER_CENTER];
		currentAnimIndex = currAnimation->getStart();
		elapsedTime = 0.0f;		
	}

	void handleAnimation(PLAYER_ANIMATION newState)
	{		
		if(newState != animState)
		{
			int index = newState;
			currAnimation = animations[index];
			currentAnimIndex = currAnimation->getStart();		

			elapsedTime = 0.0f;
			animState = newState;
		}

		if(elapsedTime > Animation::ANIMATION_DELAY)
		{
			elapsedTime = ((int)floor(elapsedTime) % Animation::ANIMATION_DELAY);
			currentAnimIndex = currentAnimIndex + 1;
		}

		if(currAnimation->getEnd() < currentAnimIndex)
		{
			currentAnimIndex = currAnimation->getStart();
		}
	}
private:
	// coordinates of the player
	float _x;
	float _y;

	/// Edges of the screen
	float screenXEdge;
	float screenYEdge;

	/// Animation variables
	float elapsedTime;
	int currentAnimIndex;
	PLAYER_ANIMATION animState;
	Animation* currAnimation;
	Animation* animations[5];

	/// Graphics components
	XImage* img_player;
	Pixmap img_mask;
	Spritesheet* sheet;
};

/// World
///  The line component defines a sequence of lines to be drawn to the screen.
class World :
	public Displayable {
public:
	World(void)
	{
	}

	~World(void)
	{		
	}

	/// Overloaded. Initializes required services and loads any non-graphics resources.
	virtual void initialize(XInfo* xinfo)
	{
		
	}

	/// Overloaded. Draws the Displayable component to the screen.
	virtual void draw(XInfo* xinfo, GameTime* gameTime)
	{
		Display* _display = xinfo->getDisplay();
		GC _gc = xinfo->getGraphicContext();

		XPutImage(_display, xinfo->getImageBuffer(), _gc, img_background,
			0, 0,
			0, 0,
			img_background->width, img_background->height);
	}

	/// Overloaded. Updates the Displable component based on recent changes.
	virtual void update(XInfo* xinfo, GameTime* gameTime)	
	{

	}

	/// Overloaded. Loads an asset that is needed for the component.
	virtual void load(XInfo* xinfo)	
	{
		bool success = xinfo->loadImage("assets/backgrounds/bg_grasslands.tga", &img_background);
		if(!success)
		{
			Logger::application_error(Logger::LOG_ASSETERROR);
		}
	}

	/// Overloaded. Disposes all data that was loaded by this Displayable.
	virtual void unload(XInfo* xinfo)	{ }
private:
	///Graphics background
	XImage* img_background;
};

/// XSprite
///  An input sample that focuses on keyboard input and mouse movement to display various graphics onto the screen.
class XSprite :
	public Game
{
public:
	/// Creates an instance of the XInput game.
	XSprite(void)
	{

	}

	/// Overloaded. Initializes required services and loads any non-graphics resources.
	virtual void initialize(XInfo* xinfo)
	{
		world = new World();
		player = new Player(50, 530);

		addComponent(player);
		addComponent(world);		
	}

	/// Overloaded. Updates the Displable component based on recent changes.
	virtual void update(XInfo* xinfo, GameTime* gameTime)
	{
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
	Player* player;
	World* world;
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

	XSprite game;

	std::cout << "press Q to exit" << std::endl;
	std::cout << "press Arrow Keys to move" << std::endl;
	game.run(xinfo);
}
