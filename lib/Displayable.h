#pragma once

/// Standard libraries
#include <cstdlib>
#include <iostream>

/// Project components
#include "XInfo.h"
#include "GameTime.h"

/// Displayable
///	 Displayable is the base class for an object that can be updated/drawn to the screen.  It includes
///  additional functionality such as initialize/load/unload for a self contained component.
class Displayable
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
	virtual void load(XInfo* xinfo) = 0;

	/// Disposes all data that was loaded by this Displayable.
	///  @xinfo The graphics information for game.
	virtual void unload(XInfo* xinfo) = 0;

	/// Initializes required services and loads any non-graphics resources.
	///  @xinfo The graphics information for game.
	virtual void initialize(XInfo* xinfo) = 0;
};
