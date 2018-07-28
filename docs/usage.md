# XPlatformer

## Summary

XPlatformer is a simple video game reminiscent of the classic side-scrolling arcade game, using the XLib API. The point of the game is to control a character through a terrain to meet an objective. The project makes use of the XLib API (XOrg) and focus on code that was developed to accomplish tasks for the assignment task.

* Art Assets (Bitmaps)
* Animation  (Player)
* Levels
* User Interaction (Key collection, opening lock boxes)

## Key Controls

The application is controlled by the following key commands.

|**Key**|**Description**|
|---|---|
|Q| Quits the application|
|Left Arrow | Moves the player in the left direction. |
|Right Arrow | Moves the player in the right direction. |
|J| The player jumps. |
|Space| Starts/pauses the game. |
|E| Action command for performing actions. |

## Settings

The command line offers four utilites for modification

|**Command**|**Default**|**Type**|**Range**|**Description**|
|---|---|---|---|---|
|--sun|2.0 |Float|1.5, 3.0|A command argument for modifying the speed of the sun and related sky components. This determines the velocity (or speed) of the sun as it travels across the sky.|
|--fps|30|Integer|30, 45|A command argument for modifying Frames Per Second (FPS).|
|--jump/--j|22.5|Float|20.0, 30.0| A command argument for modifying the jumping velocity of the 'mario' character. It can also be considered as 'jump power'. It defines how much the player should accelerate when jumping. |
|--move/--m|2.0|Float|8.0, 15.0| A command argument for modifying the speed of movement or running of the 'mario' character.|

Examples of Usage:

```bash
./XPlatformer --move=7.5 --sun=3.0 --fps=45 --jump=23.5
```