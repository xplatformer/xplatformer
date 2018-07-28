#ifndef _INCL_GCONSTANTS
#define _INCL_GCONSTANTS

/// Standard libraries
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <string>

/// Contains standard constants for the XPlatformer game and associated files.
namespace GameConstants
{
	enum BG
	{
		/// Background ID for castle.
		BG_CASTLE = 0,

		/// Background ID for desert.
		BG_DESERT = 1,

		/// Background ID for grasslands.
		BG_GRASSLANDS = 2,

		/// Background ID for mushrooms.
		BG_SHROOM = 3
	};

	/// Number of states/animations available.
	static const int PLAYER_ANIMATION_COUNT = 7;

	/// The default horizontal velocity of the player present in the PlayerComponent.
	static int PLAYER_DEFAULT_SPEED = 9;

	/// The default vertical acceleration of the player present in the PlayerComponent.
	static float PLAYER_DEFAULT_JUMP = 3.0f;

	/// The default number of clouds present in the SkyComponent.
	static int SKY_DEFAULT_COUNT = 3;

	/// The default speed of clouds present in the SkyComponent.
	static float DEFAULT_SKY_SPEED = 3.0f;
}

#endif
