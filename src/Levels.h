#ifndef _INCL_LEVELS
#define _INCL_LEVELS

/// Project components
#include "WorldComponent.h"
#include "Blocks.h"

/// Framework components
#include "framework/Logger.h"

///Number of levels available
#define LEVEL_COUNT 3

/// Levels
///		Levels is fairly straightforward.  It is a class that contains functions that set specific indicies of the world to reflect certain blocks.
namespace Levels
{
	/// Function used to define attributes of a level using the WorldComponent. Specifically level one.
	///  @xinfo The graphics information for game.
	///  @world The component control tool of the world.
	void _loadWorldOne(XInfo* xinfo, WorldComponent &world)
	{
		for(int x = 0; x < world.getWorldWidth(); x++)
		{
			if(x >= 3 && x <= 7)
			{
				world.setBlock(x, 1, BLOCK_BRIDGE);
				world.setBlock(x, 0, BLOCK_WATER);

				world.setBlock(x, 4, BLOCK_PLANK);
			}
			else
			{
				world.setBlock(x, 1, BLOCK_GRASS);
				world.setBlock(x, 0, BLOCK_DEFAULT);
			}
		}

		world.setBlock(3, 2, BLOCK_COIN_GOLD);
		world.setBlock(7, 2, BLOCK_COIN_GOLD);

		world.setBlock(5, 2, BLOCK_KEY_RED);
		world.setBlock(9, 2, BLOCK_LOCK_RED); 

		world.loadBackground(xinfo, GameConstants::BG_GRASSLANDS);
	}

	/// Function used to define attributes of a level using the WorldComponent. Specifically level two.
	///  @xinfo The graphics information for game.
	///  @world The component control tool of the world.
	void _loadWorldTwo(XInfo* xinfo, WorldComponent &world)
	{
		int top = 3;
		for(int x = 0; x < top; x++)
		{
			world.setBlock(0, x, BLOCK_ROCK);
			world.setBlock(1, x, BLOCK_ROCK);

			world.setBlock(world.getWorldWidth() - 1, x, BLOCK_ROCK);
			world.setBlock(world.getWorldWidth() - 2, x, BLOCK_ROCK);
		}

		for(int x = 0; x < world.getWorldWidth(); x++)
		{
			if(x > 1 && x < world.getWorldWidth() - 2)
			{
				world.setBlock(x, 0, BLOCK_ROCK);
				world.setBlock(x, 1, BLOCK_LAVA);
			}

			if (x < 4 || x > 6)
			{
				world.setBlock(x, top, BLOCK_ROCK);
			}
		}

		world.setBlock(world.getWorldWidth() - 3, top + 1, BLOCK_COIN_GOLD);
		world.setBlock(5, 3, BLOCK_COIN_GOLD);

		world.loadBackground(xinfo, GameConstants::BG_CASTLE);
	}

	/// Function used to define attributes of a level using the WorldComponent. Specifically level three.
	///  @xinfo The graphics information for game.
	///  @world The component control tool of the world.
	void _loadWorldThree(XInfo* xinfo, WorldComponent &world)
	{
		for(int x = 0; x < world.getWorldWidth(); x++)
		{
			world.setBlock(x, 0, BLOCK_DEFAULT);
			world.setBlock(x, 1, BLOCK_SAND);
		}

		int platfrm = 5;
		for(int x = 0; x < platfrm; x++)
		{
			world.setBlock(0, x, BLOCK_DEFAULT);
		}
		world.setBlock(0, platfrm, BLOCK_SAND);

		int level1 = 5;
		for(int x = 0; x < level1; x++)
		{
			world.setBlock(x + 4, platfrm, BLOCK_SAND);
		}

		for(int x = 0; x < 2; x++)
		{
			world.setBlock(x + 8, 2, BLOCK_SAND);
			world.setBlock(x + 8, 1, BLOCK_DEFAULT);
		}

		for(int y = 0; y < 3; y++)
		{
			world.setBlock(10, y, BLOCK_DEFAULT);
			world.setBlock(11, y, BLOCK_DEFAULT);
		}
		world.setBlock(10, 3, BLOCK_SAND);
		world.setBlock(11, 3, BLOCK_SAND);

		world.setBlock(5, platfrm + 1, BLOCK_KEY_BLUE);
		world.setBlock(5, platfrm, BLOCK_LOCK_GREEN);

		world.setBlock(5, 3, BLOCK_KEY_GREEN);
		world.setBlock(5, 2, BLOCK_LOCK_BLUE);

		world.loadBackground(xinfo, GameConstants::BG_DESERT);
	}

	/// Function used to define attributes of a level using the WorldComponent.
	///  @xinfo The graphics information for game.
	///  @world The component control tool of the world.
	///  @index The index defining which level to set the world too.  Default is world one.
	void setLevel(XInfo* xinfo, WorldComponent &world, int index)
	{
		switch(index)
		{
		case 1:
			_loadWorldOne(xinfo, world);
			break;
		case 2:
			_loadWorldTwo(xinfo, world);
			break;
		case 3:
			_loadWorldThree(xinfo, world);
			break;
		default:
			_loadWorldOne(xinfo, world);
			break;
		}
	}
}

#endif
