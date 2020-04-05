#pragma once

/// Standard libraries
#include <stdio.h>
#include <sys/time.h>

/// Project components
#include "Constants.h"

/// GameTime
///	 The GameTime class is designed to store information about time relating to the frames drawn.  It provides
///	 details such as time elapsed since last update or the current frame time.
class GameTime
{
public:
	/// Creates a new instance of GameTime.
	GameTime(void)
	{
		unsigned long now = getNow();
		_now = now;
		_prev = now;

		_totalGametime = 0;
		_elapsed = 0;
	}

	/// Creates a new instance of GameTime.
	///  @previousTime The elapsed game time since the last update.
	///  @totalGameTime The amount of game time since the start of the game. 
	GameTime(unsigned long previousTime, unsigned long totalGameTime)
	{
		_now = getNow();
		_prev = previousTime;

		_elapsed = (_now - _prev);
		_totalGametime = totalGameTime;
	}

	/// Gets the current clock time.
	///  @returns The current time value.
	unsigned long getCurrentTime(void)
	{
		return _now;
	}

	/// The amount of elapsed game time since the last update.
	///  @returns The elapsed game time.
	unsigned long getElapsedTime(void)
	{
		return _elapsed;
	}

	/// The clock time at the start of the game.
	///  @returns The total game time.
	unsigned long getTotalTime(void)
	{
		return _totalGametime;
	}

	/// Gets the timestamp associated with the previous frame.
	///  @returns The previous frame clock time.
	unsigned long getPreviousTime(void)
	{
		return _prev;
	}

	/// Gets the elapsed time as a computed millisecond format.
	///  @returns The elapsed delta component.
	float getElapsedDelta(void)
	{
		return (float)_elapsed / Constants::TIME_DIVISOR;
	}

	/// Get the current time in microseconds.
	///  @returns The time value in microseconds.
	static unsigned long getNow()
	{
		timeval tv;
		gettimeofday(&tv, 0);

		// Gets the current seconds * 1000 + microseconds / 1000
		unsigned long time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
		return time;
	}

private:
	unsigned long _now;
	unsigned long _prev;
	unsigned long _elapsed;
	unsigned long _totalGametime;
};
