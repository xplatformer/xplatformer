# XPlatformer Framework #
***
#### Description
Developed as a part of the XPlatformer project, the *References* folder contains a series of helper classes and functions that act as wrappers for commonly used XLib functions or sequences of operations.   Specifically the framework assists in the handling of window operations, graphics context and visual drawing.   The framework more broadly assists in handling input, logging, mathematics and game organization.

***
##### Framework Files

| File				| Description		 																					|
| ------------- 	| ------------- 																						|
| Animation.h   	| The Animation class is designed to store the start and end index of an animation.  					|
| Constants.h 		| Contains standard constants for the XPlatformer game and associated files.  							|
| Displayable.h 	| Displayable is the base class for an object that can be updated/drawn to the screen.       			|
| Game.h 			| Game is the base class for an object that can be updated/drawn to the screen.         				|
| GameTime.h 		| The GameTime class is designed to store information about time relating to the frames drawn.       	|
| gicon.bitmap 		| gicon is the standard application icon used by XPlatformer											|
| KeyboardState.h	| Represents the state of keystrokes recorded by a keyboard input device.           					|
| Keys.h 			| Identifies a particular key on a keyboard.  															|
| Logger.h			| Contains standard logging functionality and stored notifications.    									|
| MathHelper.h 		| Contains commonly used functions and precalculated values.        									|
| MouseState.h 		| Represents the state of a mouse input device, including mouse cursor position and buttons pressed.  	|
| Rectangle.h 		| Defines a rectangle.       																			|
| Spritesheet.h 	| A uniform sheet of sprites that can be drawn individually.        								 	|
| Vector2.h 		| Defines a vector with two components.     															|
| XInfo.h 			| Game is the base class for an object that.  														 	|

---

##### Compile Instructions

To make ("compile and link") using the framework, use the included makefile as an example.  In order to properly link the line `-I../References` must be included in the compile path. For example, to make null.cpp:

	g++ -o null null.cpp -L/usr/X11R6/lib -lX11 -I../References -lstdc++

Then, to run:

	./null


