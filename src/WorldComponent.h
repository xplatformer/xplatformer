#ifndef _INCL_WORLDCOMPONENT
#define _INCL_WORLDCOMPONENT

/// Standard libraries
#include <cstdio>
#include <sstream>
#include <cmath>
#include <string>
#include <cstdio>

/// Framework libraries
#include "xgamelib/Constants.h"
#include "xgamelib/Displayable.h"
#include "xgamelib/Spritesheet.h"
#include "xgamelib/Logger.h"

/// Project libraries
#include "WorldComponent.h"
#include "GameConstants.h"
#include "Blocks.h"
#include "Resources.h"

// Retrieves the single index from the two dimensional index.
#define WORLD_INDEX(x, y) (x + worldWidth * y)

/// WorldComponent
///  Central class for all sky game components.
class WorldComponent :
	public Displayable
{
public:
	/// Initializes a new instance of WorldComponent.
	///  @background The identifier for the game background image.
	///  @width The width of the world grid.
	///  @height The height of the world grid.
	WorldComponent(int backgroundId, int width, int height)
	{
		background = backgroundId;
		worldWidth = width;
		worldHeight = height;
		grid = new int[width * height];
	}

	/// Disposes of the SkyComponent instance.
	~WorldComponent(void)
	{
	}

	/// Overloaded. Draws the Displayable component to the screen.
	virtual void draw(XInfo* xinfo, GameTime* gameTime)
	{
		Display* _display = xinfo->getDisplay();
		GC _gc = xinfo->getGraphicContext();

		//background (no need for clipmask)
		XPutImage(_display, xinfo->getImageBuffer(), _gc, img_background, 0, 0,	0, 0, img_background->width, img_background->height);

		int blockWidth = sheet->getSpriteWidth();
		int blockHeight = sheet->getSpriteHeight();

		//XSetClipMask(_display, _gc, img_mask);
		xinfo->setMask(img_mask);	
		for(int x = 0; x < worldWidth; x++)
		{
			int posx = x * blockWidth;
			for(int y = 0; y < worldHeight; y++)
			{
				if(getBlock(x, y) == BLOCK_EMPTY)
				{
					continue;
				}

				int posy = (worldHeight - 1 - y) * blockHeight;
				int blockVal = getBlock(x, y);

				xinfo->draw(sheet, posx, posy, blockVal);
			}
		}
		xinfo->clearMask();
		XSetClipMask(_display, _gc, None);
	}

	/// Overloaded. Updates the Displable component based on recent changes.
	virtual void update(XInfo* xinfo, GameTime* gameTime)
	{
	}

	/// Overloaded. Loads an asset that is needed for the component.
	virtual void load(XInfo* xinfo)
	{
		const char* fileBackground = getWorldBackground(background);
		bool success = xinfo->loadImage(fileBackground, &img_background);
		if(!success)
		{
			Logger::application_error(Logger::LOG_ASSETERROR);
		}

		success = xinfo->loadImage(Resources::ASSET_WORLDSHEET, &img_blocks, Resources::ASSET_WORLDSHEET_MASK, &img_mask);
		if(!success)
		{
			Logger::application_error(Logger::LOG_ASSETERROR);
		}

		sheet = new Spritesheet(img_blocks, 5, 5, 1);
	}

	/// Overloaded. Disposes all data that was loaded by this Displayable.
	virtual void unload(XInfo* xinfo)
	{
		XDestroyImage(img_background);
	}

	/// Overloaded. Initializes required services and loads any non-graphics resources.
	virtual void initialize(XInfo* xinfo)
	{
		clear();
		availableObjects = 0;
	}

	/// Loads a background based on an id.
	///  @xinfo The graphics information for game.
	///  @id The background identifier id.
	void loadBackground(XInfo* xinfo, int id)
	{
		background  = id;
		const char* fileBackground = getWorldBackground(background);
		bool success = xinfo->loadImage(fileBackground, &img_background);

		if(!success)
		{
			Logger::application_debug(Logger::LOG_ASSETERROR, fileBackground);
			Logger::application_error(Logger::LOG_ERROR);
		}
	}

	/// Returns the number of objectives remaining in the level.
	///  @returns The number of objectives within the current level.
	int getObjectiveCount(void)
	{
		return availableObjects;
	}

	/// Returns the total number of objectives currently in the level.
	///  @returns The total number of objectives within the current level.
	int getTotalObjectives(void)
	{
		return totalObjectives;
	}

	/// Returns the index of the world based on grid coordinates.
	///  @x The x-coordinate of the world.
	///  @y The y-coordinate of the world.
	///  @returns The index of the grid coordinate.
	int getWorldIndex(int x, int y)
	{
		return (x + worldWidth * y);
	}

	/// Returns the enumeration block value at the coordinate.
	///  @x The x-coordinate (in world grid coordinates) of the level.
	///  @y The y-coordinate (in world grid coordinates) of the level.
	///  @returns The value at the specific grid point.
	int getBlock(int x, int y)
	{
		return grid[getWorldIndex(x, y)];
	}

	/// Returns the block value at the index.
	///  @index The block index in the world grid.
	///  @returns The value at the specific grid point.
	int getBlock(int index)
	{
		return grid[index];
	}

	/// Returns the x-position of a column.
	///  @value The value of the block.
	///  @returns True if an objective, false otherwise.
	float getWorldX(int x)
	{
		return x * sheet->getSpriteWidth();
	}

	/// Gets the y position of a row.
	///  @y The y-coordinate (in world grid coordinates) of the level.
	///  @returns The item is the information.
	float getWorldY(int y)
	{
		return (worldHeight - 1 - y) * sheet->getSpriteHeight();
	}

	/// Returns the rectangle bounds of a world grid based on grid coordinates.
	///  @x The x-coordinate (in world grid coordinates) of the level.
	///  @y The y-coordinate (in world grid coordinates) of the level.
	///  @returns The rectangle bounds of a world block.
	Rectangle* getWorldBlock(int x, int y)
	{
		float worldX = getWorldX(x);
		float worldY = getWorldY(y);

		return new Rectangle(worldX, worldY, worldWidth, worldHeight);
	}

	/// Gets the width of the world grid.
	///  @returns The grid width of the world.
	int getWorldWidth(void)
	{
		return worldWidth;
	}

	/// Gets the height of the world grid.
	///  @returns The grid height of the world.
	int getWorldHeight(void)
	{
		return worldHeight;
	}

	/// Returns the width of a world block
	///  @returns The width of a world block.
	int getBlockWidth(void)
	{
		return sheet->getSpriteWidth();
	}

	/// Returns the height of a world block.
	///  @returns The height of a world block.
	int getBlockHeight(void)
	{
		return sheet->getSpriteHeight();
	}

	/// Returns true if the value at specified grid coordinate is solid.
	///  @x The x-coordinate (in world grid coordinates) of the level.
	///  @y The y-coordinate (in world grid coordinates) of the level.
	///  @returns True if the block specified by coordinates is solid; false otherwise.
	bool isSolid(int x, int y)
	{
		return grid[getWorldIndex(x, y)] == BLOCK_EMPTY;
	}

	/// Returns true if the value at specified grid coordinate is empty.
	///  @x The x-coordinate (in world grid coordinates) of the level.
	///  @y The y-coordinate (in world grid coordinates) of the level.
	///  @returns True if the block specified by coordinates is empty; false otherwise.
	bool isEmpty(int x, int y)
	{
		return grid[getWorldIndex(x, y)] == BLOCK_EMPTY;
	}

	/// Sets the block at the specified grid position to the value.
	///  @x The x-coordinate (in world grid coordinates) of the level.
	///  @y The y-coordinate (in world grid coordinates) of the level.
	///  @val The value to set to the value.
	int setBlock(int x, int y, int val)
	{
		int index = getWorldIndex(x, y);
		int currVal = grid[index];

		if(!BLOCKS::isBlockObjective(currVal))
		{
			availableObjects--;
		}

		if(!BLOCKS::isBlockObjective(val))
		{
			availableObjects++;
		}

		grid[index] = val;
	}

	/// Sets all blocks to BLOCK_EMPTY.
	void clear(void)
	{
		int count = worldWidth * worldHeight;
		for(int i = 0; i < count; i++)
		{
			grid[i] = BLOCK_EMPTY;
		}
	}

	/// Returns the filepath of the background based on an ID.
	///  @id The id of a background.
	///  @returns The filepath to a background image based on ID.
	static const char* getWorldBackground(int id)
	{
		switch (id)
		{
		case GameConstants::BG_CASTLE: return Resources::ASSET_BG_CASTLE;
		case GameConstants::BG_DESERT: return Resources::ASSET_BG_DESERT;
		case GameConstants::BG_GRASSLANDS: return Resources::ASSET_BG_GRASSLANDS;
		case GameConstants::BG_SHROOM:	return Resources::ASSET_BG_SHROOM;
		default: return Resources::ASSET_BG_CASTLE;
		}
	}

private:
	///Graphics background
	XImage* img_background;
	int background;

	///Graphics components of world
	XImage* img_blocks;
	Pixmap img_mask;
	Spritesheet* sheet;

	///Number of objectives
	int availableObjects;
	int totalObjectives;

	///Grid Components
	int worldWidth;
	int worldHeight;
	int* grid;
};

#endif
