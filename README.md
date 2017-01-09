# XPlatformer #
##### Language: C/C++
***
#### Abstract
A simple platforming game developed for an assignment for a user interfaces course.  The purpose of this assignment was to develop a platforming game that was meant to the *X Windows System*.  

***
#### Summary

XPlatformer is a simple video game reminiscent of the classic side-scrolling arcade game, using the XLib API.  The point of the game is to control a character through a terrain to meet an objective. The project makes use of the XLib API (XOrg) and focus on code that was developed to accomplish tasks for the assignment task.  

##### Features Illustrated

* Art Assets (Bitmaps)
	* Art assets are loaded into the game from the Resources/ folder.  Spritesheets are used to store the images and associated X bitmaps are present for the necessary clipmasks.
* Animation  (Player)
	* The player character is animated, showing left/right running sequences in addition to the jump ability.
* Levels
	* Currently there are three levels that will be cycled through.  Offering simplistic worlds tht can be tranversed.
* Addition Input Commands
	* The user is capable of collecting keys, that can be used to open lock boxes for additional points.  This is available by collecting a key, and then pressing E near the associated colour lockbox.
	 
##### Description
The assignment is broken into a series of different files which are used to control certian game components and offer utility above the existing XLib API. It is important to note that this project makes use of resources (art assets) and are required when running.  These are available in both raw and compiled formats in the Resources/ folder.  

##### Key Controls
The application is controlled by the following key commands.

| **Key**			| **Description**		 							|
| ------------- 	| ------------- 								|
| Q  				| Quits the application  						|
| Left Arrow 		| Moves the player in the left direction.  		|
| Right Arrow 		| Moves the player in the right direction.      |
| J					| The player jumps.         					|
| Space				| Starts/pauses the game.       				|
| E					| Action command for performing actions.       	|	

##### Command Lines 

The command line offers four utilites for modification

| **Command**	| **Default**   | **Type** 		| **Range**		 | **Description**	 |
| ------------- | ------------- |-------------  | ------------- | -------------  |
| --sun 			| 2.0 			| Float			| 1.5, 3.0		| A command argument for modifying the speed of the sun and related sky components.  This determines the velocity (or speed) of the sun as it travels across the sky.|
| --fps 			| 30 			| Integer 		| 30, 45 		| A command argument for modifying Frames Per Second (FPS).|
| --jump/--j 	| 22.5 			| Float 		| 20.0, 30.0 	| A command argument for modifying the jumping velocity of the 'mario' character.  It can also be considered as 'jump power'.  It defines how much the player should accelerate when jumping. |
| --move/--m 	| 2.0 			| Float 		| 8.0, 15.0 	| A command argument for modifying the speed of movement or running of the 'mario' character.|

Examples of Usage:

	./XPlatformer --move=7.5 --sun=3.0 --fps=45 --jump=23.5
	
	It is important to note that the arguments are a single phrase with no spaces.  
	As such a command such as --fps requires all aspects to be tightly written

##### Resources
All art assets were acquired from *http://opengameart.org/* in particular from *http://opengameart.org/users/kenney*.  Majority of art assets come from a particular package known as "Platformer Art Deluxe" available at *http://opengameart.org/content/platformer-art-deluxe*.  If you would like to know more about these art assets, look into *http://open.commonly.cc/* or the "Open Bundle" [See *http://www.kenney.nl/*].  The art assets are available with the Creative Commons License (CC0)

---

##### Compile Instructions

To make ("compile and link") an example, use the included makefile with 
the name of cpp file passed as a variable. By default the name variable is
assigned to *XPlatformer*.

	make NAME="XPlatformer"
	
Then, to run:

	./XPlatformer

Or you can even do it without setting the variable:

	make
	make run




