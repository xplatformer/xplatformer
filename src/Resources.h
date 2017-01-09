#ifndef _INCL_RESOURCES
#define _INCL_RESOURCES

/// Standard libraries
#include <string>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <iostream>

// Includes a collection of resources
namespace Resources
{
	/// Icon Assets
	static const char* ASSET_ICON = "../assets/gicon.bitmap";

	/// Sky Assets
	static const char* ASSET_SKYSHEET = "../assets/sky/spritesheet.tga";
	static const char* ASSET_SKYSHEET_MASK = "../assets/sky/spritesheet.xbm";

	/// Player Assets
	static const char* ASSET_PLAYERSHEET = "../assets/player/spritesheet.tga";
	static const char* ASSET_PLAYERSHEET_MASK = "../assets/player/spritesheet.xbm";

	/// World Assets
	static const char* ASSET_WORLDSHEET = "../assets/world/spritesheet.tga";
	static const char* ASSET_WORLDSHEET_MASK = "../assets/world/spritesheet.xbm";

	/// Asset Backgrounds
	static const char* ASSET_BG_CASTLE = "../assets/backgrounds/bg_castle.tga";
	static const char* ASSET_BG_DESERT = "../assets/backgrounds/bg_desert.tga";
	static const char* ASSET_BG_GRASSLANDS = "../assets/backgrounds/bg_grasslands.tga";
	static const char* ASSET_BG_SHROOM = "../assets/backgrounds/bg_shroom.tga";

	/// String Assets
	static const char* ASSET_INFO_SPACE = "Press SPACE to Play";
	static const char* ASSET_INFO_MOVEMENT = "Use Arrows Keys to Move";
	static const char* ASSET_INFO_JUMP = "Use J to Jump";
	static const char* ASSET_INFO_ACTION = "Use E to open boxes (only from above)";
}

#endif
