#pragma once

/// Standard libraries
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <iostream>

/// Contains standard logging functionality and stored notifications.
namespace Logger
{
	/// Asset Loading Messages
	static const char* LOG_ASSETLOADING = "# Loading Game Assets";
	static const char* LOG_ASSETLOADED = "# Assets Sucessfully Loaded";
	static const char* LOG_ASSETRELEASING = "# Releasing Game Assets";
	static const char* LOG_ASSETRELEASED = "# Game Assets Released";
	static const char* LOG_ASSETERROR = "# An error occurred while attempting to load asset: ";

	/// Game Control Messages
	static const char* LOG_GAMESTART = "# Game Loop Starting"; 
	static const char* LOG_GAMEEND = "# Game Loop Ending"; 
	static const char* LOG_GAMEINIT = "# Game Initializing";
	static const char* LOG_TASKDONE = "# Task Complete";
	static const char* LOG_ERROR = "# An error occurred while attempting to load asset, terminating.";

	/// Argument Messages
	static const char* LOG_ARGINVALID = "# No Arguments Discovered";
	static const char* LOG_ARGCOUNT = "# Discovered Arguments: ";

	/// Console Information Messages
	static const char* INFO_FPS = "# Frames per second (FPS) = ";
	static const char* INFO_MOVE = "# Player move speed = ";
	static const char* INFO_JUMP = "# Player jump speed = ";
	static const char* INFO_SUN = "# World Sun speed = ";

	/// Determines if debug messages should be displayed to standard output. Default is false.
	static bool IS_DEBUG = false;

	/// A utility function for reporting application debug information.
	///  @str The detail message.
	static void application_debug(const char* str)
	{
		if(Logger::IS_DEBUG)
		{
			std::cout << str << std::endl;
		}
	}

	/// A utility function for reporting application debug information.
	///  @str The detail message.
	///  @value1 Argument value for message.
	static void application_debug(const char* str, int value1)
	{
		if(Logger::IS_DEBUG)
		{
			std::cout << str << "[" << value1 << "]" << std::endl;
		}
	}

	/// A utility function for reporting application debug information.
	///  @str The detail message.
	///  @value1 Argument value for message.
	static void application_debug(const char* str, const char* value1)
	{
		if(Logger::IS_DEBUG)
		{
			std::cout << str << "[" << value1 << "]" << std::endl;
		}
	}


	/// A utility function for reporting application error events.
	///  @str The detail message.
	static void application_error(const char* str)
	{
		std::cerr << str << std::endl;
		exit(0);
	}

	/// A utility function for reporting application information.
	///  @str The detail message.
	///  @value1 Argument value for message.
	static void application_info(const char* str, int value1)
	{
		std::cout << str << "[" << value1 << "]" << std::endl;
	}
}
