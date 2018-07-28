#ifndef _INCL_BLOCKS
#define _INCL_BLOCKS

/// Blocks
///	 Blocks defines an enumeration of all the blocks available.
enum BLOCK
{
	/// Empty space
	BLOCK_EMPTY = -1,

	/// Top-Level layers
	BLOCK_GRASS = 1,
	BLOCK_SPACE = 10,
	BLOCK_SAND = 14,

	/// Filler layers
	BLOCK_DEFAULT = 11,
	BLOCK_ROCK = 13,
	BLOCK_DIRT = 0,

	/// Bridge/Plank
	BLOCK_BRIDGE = 2,
	BLOCK_PLANK = 12,

	/// Bonus Block
	BLOCK_BONUS = 3,
	BLOCK_BONUS_USED = 4,

	/// Coins
	BLOCK_COIN_BRONZE = 5,
	BLOCK_COIN_GOLD = 6,
	BLOCK_COIN_RARE = 7,
	BLOCK_COIN_SILVER = 8,
	BLOCK_COIN_SPECIAL = 9,

	/// Keys
	BLOCK_KEY_BLUE = 15,
	BLOCK_KEY_GREEN = 16,
	BLOCK_KEY_RED = 17,
	BLOCK_KEY_YELLOW = 18,

	/// Locks
	BLOCK_LOCK_BLUE = 20,
	BLOCK_LOCK_GREEN = 21,
	BLOCK_LOCK_RED = 22,
	BLOCK_LOCK_YELLOW = 23,

	/// Liquids
	BLOCK_LAVA = 19,
	BLOCK_WATER = 24
};

namespace BLOCKS
{
	/// Returns true if the block value is solid and collidable.
	///  @value The value of the block.
	///  @returns True if the block specified is solid; false otherwise.
	bool isBlockSolid(int value)
	{
		switch(value)
		{
		case BLOCK_EMPTY:
		case BLOCK_KEY_BLUE:
		case BLOCK_KEY_GREEN:
		case BLOCK_KEY_RED:
		case BLOCK_KEY_YELLOW:
		case BLOCK_COIN_BRONZE:
		case BLOCK_COIN_GOLD:
		case BLOCK_COIN_RARE:
		case BLOCK_COIN_SILVER:
		case BLOCK_COIN_SPECIAL:
		case BLOCK_PLANK:
			return false;
		default:
			return true;
		}
	}

	/// Returns true if the block value can be passed through.
	///  @value The value of the block.
	///  @returns True if the block specified is not solid; false otherwise.
	bool isBlockPassable(int value)
	{
		switch(value)
		{
		case BLOCK_EMPTY:
		case BLOCK_KEY_BLUE:
		case BLOCK_KEY_GREEN:
		case BLOCK_KEY_RED:
		case BLOCK_KEY_YELLOW:
		case BLOCK_COIN_BRONZE:
		case BLOCK_COIN_GOLD:
		case BLOCK_COIN_RARE:
		case BLOCK_COIN_SILVER:
		case BLOCK_COIN_SPECIAL:
		case BLOCK_PLANK:
			return true;
		default:
			return false;
		}
	}

	/// Returns true if the block value is impassable.
	///  @value The value of the block.
	///  @returns True if the block specified is impassable; false otherwise.
	bool isBlockImpassable(int value)
	{
		return !isBlockPassable(value);
	}	

	/// Returns true if the block value is a platform.
	///  @value The value of the block.
	///  @returns True if the block specified is a platform; false otherwise.
	bool isBlockPlatform(int value)
	{
		return BLOCK_PLANK == value;
	}

	/// Returns true if the block is an objective.
	///  @value The value of the block.
	///  @returns True if an objective, false otherwise
	bool isBlockObjective(int value)
	{
		switch(value)
		{
		case BLOCK_LOCK_BLUE:
		case BLOCK_LOCK_GREEN:
		case BLOCK_LOCK_RED:
		case BLOCK_LOCK_YELLOW:
		case BLOCK_COIN_BRONZE:
		case BLOCK_COIN_GOLD:
		case BLOCK_COIN_RARE:
		case BLOCK_COIN_SILVER:
		case BLOCK_COIN_SPECIAL:
			return true;
		default:
			return false;
		}
	}
}

#endif
