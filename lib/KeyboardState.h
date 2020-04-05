#pragma once

/// Project components
#include "Constants.h"
#include "Logger.h"
#include "Keys.h"

/// KeyboardState
///	 Represents the state of keystrokes recorded by a keyboard input device.
class KeyboardState
{
public:
	/// Initializes a new instance of the KeyboardState class.
	KeyboardState(void)
	{
		for(int i = 0; i < Constants::KEY_COUNT; i++)
		{
			keys[i] = 0;
		}
	}

	/// Returns whether a specified key is currently being pressed.
	///  @key Enumerated value that specifies the key to query.
	///  @returns True if the key specified by key is pressed; false otherwise.
	bool isKeyDown(KEYS key)
	{
		return getInternalKey(key);
	}

	/// Returns whether a specified key is currently not pressed.
	///  @key Enumerated value that specifies the key to query.
	///  @returns True if the key specified by key is not pressed; false otherwise.
	bool isKeyUp(KEYS key)
	{
		return !getInternalKey(key);
	}

	/// Sets the specified key to be down.
	///  @key Enumerated value that specifies the key to query.
	void set(KEYS key)
	{		
		setInternalKey(key, true);
	}

	/// Clears the specific key to be up.
	///  @key Enumerated value that specifies the key to query.
	void clear(KEYS key)
	{
		setInternalKey(key, false);
	}

	/// Resets the keyboard state.
	void reset(void)
	{
		for(int i = 0; i < Constants::KEY_COUNT; i++){
			keys[i] = 0;
		}
	}

private:
	/// Gets the key mask related to the key.
	///  @key Enumerated value that specifies the key to query.
	///  @returns The mask index of the specified key.
	int getInternalIndex(KEYS key)
	{
		int value = (int)key;
		int index = value >> 5;

		if(index < 0 || index >= Constants::KEY_COUNT)
		{
			return -1;
		}

		return index;
	}

	/// Gets the key mask.
	///  @key Enumerated value that specifies the key to query.
	///  @returns The key mask of the specified key.
	unsigned int getKeyMask(KEYS key)
	{
		int value = (int)key;
		unsigned int keyVal = 1;
		unsigned int mask = keyVal << (value & 0x1f);

		return mask;
	}

	/// Switches the key to the value property.
	///  @key Enumerated value that specifies the key to query.
	///  @value The state of the key.
	void setInternalKey(KEYS key, bool value)
	{
		int index = getInternalIndex(key);
		unsigned int mask = getKeyMask(key);

		if(value) {
			keys[index] |= mask;
		} else {			
			keys[index] &= ~mask;
		}
	}

	/// Returns the state of the specified key.
	///  @key Enumerated value that specifies the key to query.
	///  @returns True if the key specified by key is pressed; false otherwise.
	bool getInternalKey(KEYS key)
	{
		int index = getInternalIndex(key);
		unsigned int mask = getKeyMask(key);
		unsigned int element = keys[index];

		return (element & mask) != 0;
	}

	/// Stores the keyboard key states.
	unsigned int keys[Constants::KEY_COUNT];
};
