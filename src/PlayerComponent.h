#pragma once

/// Standard libraries
#include <cstdio>
#include <string>
#include <sstream>
#include <stdlib.h>

/// X11/XLib libraries
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysymdef.h>

/// Framework components
#include "lib/MathHelper.h"
#include "lib/Displayable.h"
#include "lib/Spritesheet.h"
#include "lib/Animation.h"
#include "lib/Constants.h"
#include "lib/Logger.h"

/// Project components
#include "PlayerComponent.h"
#include "WorldComponent.h"
#include "Blocks.h"
#include "Resources.h"
#include "GameConstants.h"

/// The current action that the player is performing.
enum PLAYER_ACTION
{
	/// The player is currently jumping.
	PLAYER_JUMP,

	/// The player is currently walking
	PLAYER_WALK,

	/// The player is currently idle.
	PLAYER_IDLE
};

/// The current direction that the player is facing.
enum PLAYER_DIRECTION
{
	/// The player is facing left.
	PLAYER_LEFT,

	/// The player is facing right.
	PLAYER_RIGHT,

	/// The player is in  direction.
	PLAYER_FRONT
};

/// An animation naming numeric.
enum PLAYER_ANIM_SPOT
{
	PLAYER_CENTER = 0,

	PLAYER_RUN_LEFT = 1,
	PLAYER_STAND_LEFT = 3,
	PLAYER_JUMP_LEFT = 2,

	PLAYER_RUN_RIGHT = 4,
	PLAYER_STAND_RIGHT = 6,
	PLAYER_JUMP_RIGHT = 5
};

/// Player Health States.
enum PLAYER_HEALTH
{
	ALIVE = 1,
	DEAD = 2
};

/// PlayerComponent
///  The player component defines the player.  It handles interaction, state, animation,
///  updating and drawing.
class PlayerComponent :
	public Displayable
{
public:
	/// Initializes a new instance of PlayerComponent.
	///  @worldComp The world component that handles the world the player exists within.
	PlayerComponent(WorldComponent* worldComp)
	{
		world = worldComp;
		player_score = 0;
	}

	/// Disposes of the PlayerComponent instance.
	~PlayerComponent(void)
	{
	}

	/// Overloaded. Draws the Displayable component to the screen.
	virtual void draw(XInfo* xinfo, GameTime* gameTime)
	{
		///Gets the player position as integers
		int plyrx = MATH::ifloor(position->getX());
		int plyry = MATH::ifloor(position->getY());

		// Set clip mask and draw from spritesheet (based on position and current animation index)
		xinfo->setMask(img_mask);
		xinfo->draw(sheet, plyrx, plyry, currentAnimIndex);
		xinfo->clearMask();

		// Creates a stringstream to display the score
		std::stringstream ss_score;
		ss_score<< player_score << std::endl; //add endl for special character
		std::string score_text = ss_score.str();

		// Draw score text to screen
		Rectangle* rect = xinfo->getGraphicBounds();
		xinfo->drawString(score_text, rect->getWidth() - 100, rect->getHeight() - 50, 16766720);
	}

	/// Overloaded. Updates the Displable component based on recent changes.
	virtual void update(XInfo* xinfo, GameTime* gameTime)
	{
		PLAYER_ACTION newState = PLAYER_IDLE;
		PLAYER_DIRECTION newDirection = PLAYER_FRONT;
		actionPressed = 0;

		if (isOnGround)
		{
			movement = 0.0f;

			/// Fix logic of this current situation
			if(xinfo->getKeyboardState()->isKeyDown(KEY_E))
			{
				actionPressed = 1;
			}

			if(xinfo->getKeyboardState()->isKeyDown(KEY_LEFT))
			{
				movement = -1.0f;
				newState = PLAYER_WALK;
				newDirection = PLAYER_LEFT;
			}

			if(xinfo->getKeyboardState()->isKeyDown(KEY_RIGHT))
			{
				movement = 1.0f;
				newState = PLAYER_WALK;
				newDirection = PLAYER_RIGHT;
			}

			// if(xinfo->getKeyboardState()->isKeyDown(KEY_J))
			// {
			// 	movement = ((isPlayerLeft()) ?  -1.0f : 1.0f);
			// 	newState = PLAYER_JUMP;
			// 	newDirection = direction;
			// }
		}

		applyPhysics(gameTime);

		float time = gameTime->getElapsedDelta();
		elapsedTime += time * moveSpeed / 2.5;

		handleAnimation(newState, newDirection);
	}

	/// Overloaded. Loads an asset that is needed for the component.
	virtual void load(XInfo* xinfo)
	{
		bool success = xinfo->loadImage(Resources::ASSET_PLAYERSHEET, &img_player, Resources::ASSET_PLAYERSHEET_MASK, &img_mask);
		if(!success)
		{
			Logger::application_error(Logger::LOG_ASSETERROR);
		}

		sheet = new Spritesheet(img_player, 27, 1, 1);

		float swidth = (float)sheet->getSpriteWidth();
		float sheight = (float)sheet->getSpriteHeight();
		dist_To_special = sqrt(swidth * swidth + sheight * sheight) / 1.5f;

		screenXEdge = (xinfo->getImageWidth() - sheet->getSpriteWidth());
		screenYEdge = (xinfo->getImageHeight() - sheet->getSpriteHeight());
	}

	/// Overloaded. Disposes all data that was loaded by this Displayable.
	virtual void unload(XInfo* xinfo)
	{
		XDestroyImage(img_player);
		XFreePixmap(xinfo->getDisplay(), img_mask);
	}

	/// Overloaded. Initializes required services and loads any non-graphics resources.
	virtual void initialize(XInfo* xinfo)
	{
		gravity = 0.98;

		initAnimation();

		reset();
	}

	/// Resets the player to the initial default game state.
	void reset(void)
	{
		sleep(1);
		state = PLAYER_IDLE;
		direction = PLAYER_FRONT;
		health = ALIVE;

		currAnimation = animations[PLAYER_CENTER];
		currentAnimIndex = currAnimation->getStart();
		elapsedTime = 0.0;

		xVelocity = 0;
		yVelocity = gravity;

		position = new Vector2(10, 10);
		isOnGround = false;

		maxFallSpeed = jumpSpeed * 0.13f;
		maxJumpTime = 0.35f;
		maxMoveSpeed = moveSpeed * 1.0f;
		jumpControlPower = 1.0f;
		jumpTime = 0.0f;

		actionPressed = 0;
		key_blue = key_green = key_red = key_yellow = false;

		setMoveSpeed(GameConstants::PLAYER_DEFAULT_SPEED);
		setJumpSpeed(GameConstants::PLAYER_DEFAULT_JUMP);
	}

	/// Sets the horizontal velocity of the player.
	///  @speed The horizontal velocity to set.
	void setMoveSpeed(float speed)
	{
		moveSpeed = speed;
	}

	/// Gets the horizontal velocity of the player.
	///  @returns The horizontal velocity.
	float getMoveSpeed(void)
	{
		return moveSpeed;
	}

	/// Sets the vertical jump strength of the player.
	///  @speed The jump strength of the player.
	void setJumpSpeed(float speed)
	{
		jumpSpeed = speed;
	}

	/// Gets the vertical jump acceleration of the player.
	///  @returns The vertical jump acceleration.
	float getJumpSpeed(void)
	{
		return jumpSpeed;
	}

	/// Gets the direction of the player.
	///  @returns The player direction.
	PLAYER_DIRECTION getDirection(void)
	{
		return direction;
	}

	/// Gets the current state of the player.
	///  @returns The state of the player.
	PLAYER_ACTION getState(void)
	{
		return state;
	}

	/// Returns true if the player is facing left.
	/// @returns True if the player is facing left; false otherwise.
	bool isPlayerLeft(void)
	{
		return direction == PLAYER_LEFT;
	}

	/// Returns true if the player is facing right.
	/// @returns True if the player is facing right; false otherwise.
	bool isPlayerRight(void)
	{
		return direction == PLAYER_RIGHT;
	}

	/// Returns true if the player is jumping.
	/// @returns True if the player is jumping; false otherwise.
	bool isJumping(void)
	{
		return state == PLAYER_JUMP;
	}

	/// Returns true if the player is currently dead.
	/// @returns True if the player is dead; false otherwise.
	bool isDead(void)
	{
		return health == DEAD;
	}

	/// Returns true if the player is currently idle.
	/// @returns True if the player is idle; false otherwise.
	bool isIdle(void)
	{
		return state == PLAYER_IDLE;
	}

private:
	Rectangle* getBounding(void)
	{
		float left = round(position->getX());
		float top = round(position->getY());
		float width = (float)sheet->getSpriteWidth();
		float height = (float)sheet->getSpriteHeight();

		return new Rectangle(left, top, width, height);
	}

	void applyPhysics(GameTime* gameTime)
	{
		float elapsed = gameTime->getElapsedDelta();
		Vector2 initialPosition(position->getX(), position->getY());

		xVelocity = movement * moveSpeed * elapsed;
		yVelocity += MATH::clamp(yVelocity + gravity * elapsed, -maxFallSpeed, maxFallSpeed);

		if (state == PLAYER_JUMP)
		{
			std::cout << jumpTime << std::endl;
			if (0.0f <= jumpTime && jumpTime <= maxJumpTime)
			{
				jumpTime += elapsed;
				yVelocity = jumpSpeed * (1.0f - (float)pow(jumpTime / maxJumpTime, jumpControlPower));
			}
			else
			{
				jumpTime = 0.0f;
			}
		}
		else
		{
			jumpTime = 0.0f;
		}

		xVelocity = MATH::clamp(xVelocity, -maxMoveSpeed, maxMoveSpeed);

		float xMove = round(xVelocity * elapsed);
		float yMove = round(yVelocity * elapsed);
		position->move(xMove, yMove);

		// If the player is now colliding with the level, separate them.
		handleCollision();

		if (position->getX() == initialPosition.getX())
		{
			xVelocity = 0;
		}

		if (position->getY() == initialPosition.getY())
		{
			yVelocity = 0;
		}
	}


	void handleCollision()
	{
		Rectangle* bounds = getBounding();
		float width = (float)sheet->getSpriteWidth();
		float height = (float)sheet->getSpriteHeight();

		int topOfIt = (world->getWorldHeight() - 1);

		int leftBlock = MATH::ifloor(bounds->getLeft() / width);
		int rightBlock = MATH::iceiling((bounds->getRight() / width)) + 1;
		int topBlock = topOfIt - MATH::iceiling(bounds->getTop() / height);
		int bottomBlock = MATH::iclamp(topOfIt - MATH::ifloor((bounds->getBottom() / height)) - 1, 0, topOfIt);

		for(int y = bottomBlock; y <= topBlock; y++)
		{
			for(int x = leftBlock; x <= rightBlock; x++)
			{
				Rectangle* wRect = world->getWorldBlock(x, y);

				int currVal = world->getBlock(x, y);

				if(!BLOCKS::isBlockSolid(currVal) || BLOCKS::isBlockObjective(currVal))
				{
					float xSDist = bounds->getCenterX() - wRect->getCenterX();
					float ySDist = bounds->getCenterY()- wRect->getCenterY();
					float distTo = sqrt(xSDist * xSDist + ySDist * ySDist);

					if(distTo < dist_To_special)
					{
						handleSpecial(x, y);
					}
					continue;
				}

				Vector2* depth = MATH::getIntersectionDepth(bounds, wRect);
				if (depth->getX() != 0 && depth->getY() != 0)
				{
					float absDepthX = abs(depth->getX());
					float absDepthY = abs(depth->getY());

					// Resolve the collision along the shallow axis.
					if (absDepthY < absDepthX || BLOCKS::isBlockPlatform(currVal))
					{
						// If we crossed the top of a tile, we are on the ground.
						if (previousBottom <= wRect->getTop())
						{
							isOnGround = true;
						}

						// Ignore platforms, unless we are on the ground.
						if (BLOCKS::isBlockImpassable(currVal) || isOnGround)
						{
							// Resolve the collision along the Y axis.
							position->move(0, depth->getY());

							// Perform further collisions with the new bounds.
							bounds = getBounding();

						}
					}
					else if (BLOCKS::isBlockImpassable(currVal))
					{
						// Resolve the collision along the X axis.
						position->move(depth->getX(), 0);

						// Perform further collisions with the new bounds.
						bounds = getBounding();

						yVelocity = 0;
						isOnGround = true;
					}
				}
			}

			// Save the new bounds bottom
			previousBottom = bounds->getBottom();
		}

		// Restrict bounds of object
		position->setX(MATH::clamp(position->getX(), 0, screenXEdge));
		position->setY(MATH::clamp(position->getY(), 0, screenYEdge));
	}

	void initAnimation(void)
	{
		animations[PLAYER_CENTER] = new Animation(0, 0);

		animations[PLAYER_RUN_LEFT] = new Animation(3, 13);
		animations[PLAYER_JUMP_LEFT] = new Animation(25, 25);
		animations[PLAYER_STAND_LEFT] = new Animation(1, 1);

		animations[PLAYER_RUN_RIGHT] =  new Animation(14, 23);
		animations[PLAYER_JUMP_RIGHT] = new Animation(26, 26);
		animations[PLAYER_STAND_RIGHT] = new Animation(2, 2);
	}

	int getAnimIndex(PLAYER_ACTION state, PLAYER_DIRECTION direction)
	{
		if(direction == PLAYER_FRONT)
		{
			return PLAYER_CENTER;
		}

		int index = (direction == PLAYER_LEFT) ? 0 : 3;

		switch(state)
		{
		case PLAYER_JUMP:
			return index + 3;
		case PLAYER_WALK:
			return index + 1;
		case PLAYER_IDLE:
			return index + 2;
		}
		
		return index;
	}

	void handleAnimation(PLAYER_ACTION newState, PLAYER_DIRECTION newDirection)
	{
		if(newState != state || newDirection != direction)
		{
			int index = getAnimIndex(newState, newDirection);
			currAnimation = animations[index];
			currentAnimIndex = currAnimation->getStart();
			elapsedTime = 0;

			state = newState;
			direction = newDirection;
		}

		if(elapsedTime > Animation::ANIMATION_DELAY)
		{
			elapsedTime = ((int)floor(elapsedTime) % Animation::ANIMATION_DELAY);
			currentAnimIndex = currentAnimIndex + 1;
		}

		if(currAnimation->getEnd() < currentAnimIndex)
		{
			currentAnimIndex = currAnimation->getStart();
		}
	}

	void handleSpecial(int gx, int gy)
	{
		int type = world->getBlock(gx, gy);

		switch(type)
		{
		case  BLOCK_COIN_GOLD:
			player_score += 10;
			world->setBlock(gx, gy, BLOCK_EMPTY);
			break;
		case BLOCK_COIN_SILVER:
			player_score += 5;
			world->setBlock(gx, gy, BLOCK_EMPTY);
			break;
		case BLOCK_COIN_BRONZE:
			player_score += 1;
			world->setBlock(gx, gy, BLOCK_EMPTY);
			break;
		case BLOCK_COIN_RARE:
			player_score += 100;
			world->setBlock(gx, gy, BLOCK_EMPTY);
			break;
		case BLOCK_COIN_SPECIAL:
			player_score *= 2;
			world->setBlock(gx, gy, BLOCK_EMPTY);
			break;
		case BLOCK_KEY_BLUE:
			key_blue = true;
			world->setBlock(gx, gy, BLOCK_EMPTY);
			break;
		case BLOCK_KEY_GREEN:
			key_green = true;
			world->setBlock(gx, gy, BLOCK_EMPTY);
			break;
		case BLOCK_KEY_RED:
			key_red = true;
			world->setBlock(gx, gy, BLOCK_EMPTY);
			break;
		case BLOCK_KEY_YELLOW:
			key_yellow = true;
			world->setBlock(gx, gy, BLOCK_EMPTY);
			break;
		case BLOCK_LOCK_BLUE:
			if(key_blue && actionPressed)
			{
				world->setBlock(gx, gy, BLOCK_BONUS_USED);
			}
			break;
		case BLOCK_LOCK_GREEN:
			if(key_green && actionPressed)
			{
				world->setBlock(gx, gy, BLOCK_BONUS_USED);
			}
			break;
		case BLOCK_LOCK_RED:
			if(key_red && actionPressed)
			{
				world->setBlock(gx, gy, BLOCK_BONUS_USED);
			}
			break;
		case BLOCK_LOCK_YELLOW:
			if(key_yellow && actionPressed)
			{
				world->setBlock(gx, gy, BLOCK_BONUS_USED);
			}
			break;
		case BLOCK_LAVA:
			health = DEAD;
			player_score = 0;
			break;
		default:
			break;
		}
	}

	/// Graphics components
	XImage* img_player;
	Pixmap img_mask;
	Spritesheet* sheet;

	/// Edges of the screen
	float screenXEdge;
	float screenYEdge;

	/// Distance to special items (keys/coins)
	float dist_To_special;

	/// Booleans determining is key is available
	bool key_green, key_red, key_blue, key_yellow;
	unsigned int player_score;
	int actionPressed;

	/// The player x/y coordinates
	Vector2* position;
	float movement;
	float maxFallSpeed;
	float maxJumpTime;
	float maxMoveSpeed;
	float jumpControlPower;
	float jumpLaunchVelocity;
	float jumpTime;
	float previousBottom;

	/// Speed/velocity components
	float jumpSpeed;
	float moveSpeed;
	float gravity;
	bool isOnGround;

	/// X/Y velocity components
	float xVelocity;
	float yVelocity;

	/// Reference to the world
	WorldComponent* world;

	/// Player attributes
	PLAYER_DIRECTION direction;
	PLAYER_ACTION state;
	PLAYER_HEALTH health;

	/// Animation variables
	float elapsedTime;
	int currentAnimIndex;
	Animation* currAnimation;
	Animation* animations[GameConstants::PLAYER_ANIMATION_COUNT];
};
