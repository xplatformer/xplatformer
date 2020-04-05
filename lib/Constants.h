#pragma once

/// Standard libraries
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <string>

/// Contains standard constants for the XPlatformer game and associated files.
namespace Constants
{
	/// Divisors for reducing time to miliseconds.
	static const float TIME_DIVISOR = 100.0f;

	/// The length of the keys count.
	static const int KEY_COUNT = 8;
	
	/// The default FPS of the platformer game.
	static int DEFAULT_FPS = 30;
}
