#ifndef _INCL_SKY
#define _INCL_SKY

/// Standard libraries
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <list>

/// Framework libraries
#include "xgamelib/Displayable.h"
#include "xgamelib/Logger.h"

/// Project libraries
#include "SkyComponent.h"
#include "GameConstants.h"
#include "Resources.h"

using namespace std;

/// CloudComponent
///  Central class for all sky game components.
class CloudComponent { 
public:
	/// The horizontal movement speed of the cloud.
	float speed;   

	/// The horiontal position of the cloud.
	float x;

	/// The vertical position of the cloud.
	float y;
};

/// SkyComponent
///  Central class for all sky game components.
class SkyComponent : public Displayable
{
public:
	/// Initializes a new instance of SkyComponent.
	SkyComponent()
	{
		sun_speed = GameConstants::DEFAULT_SKY_SPEED;
		ccount = GameConstants::SKY_DEFAULT_COUNT;
	}

	/// Disposes of the SkyComponent instance.
	~SkyComponent(void)
	{
	}

	/// Overloaded. Draws the Displayable component to the screen.
	virtual void draw(XInfo* xinfo, GameTime* gameTime)
	{
		int sx = (int)floor(sun_x);
		int sy = (int)floor(sun_y);

		//set clipmask for spritesheet
		xinfo->setMask(img_mask);

		int sunx = (int)floor(sun_x);
		int suny = (int)floor(sun_y);
		int count = ccount;

		xinfo->draw(sheet, sunx, suny, 3);

		int i = 0;
		list<CloudComponent*>::const_iterator begin = clouds.begin();
		list<CloudComponent*>::const_iterator end = clouds.end();

		while(begin != end)
		{
			CloudComponent* comp = *begin;
			int cldx = (int)floor(comp->x);
			int cldy = (int)floor(comp->y);

			xinfo->draw(sheet, cldx, cldy, i);

			begin++;
			i++;
		}

		//remove spritesheet from clipmask
		xinfo->clearMask();
	}

	/// Overloaded. Updates the Displable component based on recent changes.
	virtual void update(XInfo* xinfo, GameTime* gameTime)
	{
		float time = gameTime->getElapsedDelta();
		int outerBound = xinfo->getGraphicBounds()->getWidth() + END_BOUND;

		float sun_xMove = time * sun_speed;
		float xsun = sun_x;
		xsun += sun_xMove;

		//check if sun is outside edge of screen
		//reset sun position to left side of screen
		if(xsun > outerBound)
		{
			xsun = WORLD_END;
		}
		sun_x = xsun;

		int i = 0;
		list<CloudComponent*>::const_iterator begin = clouds.begin();
		list<CloudComponent*>::const_iterator end = clouds.end();

		while(begin != end)
		{
			CloudComponent* comp = *begin;
			float cloud_xMove = time * comp->speed;
			float xpos = comp->x;
			xpos += cloud_xMove;

			if(xpos > outerBound)
			{
				xpos = WORLD_END; //reset point
			}
			comp->x = xpos;

			begin++;
			i++;
		}
	}

	/// Overloaded. Loads an asset that is needed for the component.
	virtual void load(XInfo* xinfo)
	{
		//sky spritesheet
		bool success = xinfo->loadImage(Resources::ASSET_SKYSHEET, &img_sky, Resources::ASSET_SKYSHEET_MASK, &img_mask);
		if(!success)
		{
			Logger::application_error(Logger::LOG_ASSETERROR);
		}

		sheet = new Spritesheet(img_sky, 2, 2, 1);
	}

	/// Overloaded. Disposes all data that was loaded by this Displayable.
	virtual void unload(XInfo* xinfo)
	{
		XFreePixmap(xinfo->getDisplay(), img_mask);
		XDestroyImage(img_sky);
	}

	/// Overloaded. Initializes required services and loads any non-graphics resources.
	virtual void initialize(XInfo* xinfo)
	{
		sun_x = 0;
		sun_y = 20;

		for(int i = 0; i < ccount; i++)
		{
			CloudComponent* cComponent = createComponent(sun_speed);

			// Cloud speed can be within range of [70% sun speed, 150% sun speed]
			clouds.push_front(cComponent);
		}
	}

	/// Creates a cloud to be added to the sky component.
	///  @speed The horizontal movement speed.
	CloudComponent* createComponent(float relSpeed)
	{
		CloudComponent* cComponent = new CloudComponent();

		// Random positions for the clouds to originate
		cComponent->x = rand() % 800;
		cComponent->y = rand() % 50;

		// rand is between [0, 8] + 7 = [7, 15]
		int ranVal = (rand() % 9) + 7;

		// Divide equal ranVal by 10.0 (reducing it to a float [0.7, 1.5] or [70%, 150%]
		cComponent->speed = relSpeed * (ranVal / 10.0);

		return cComponent;
	}

	/// Sets the horizontal movement speed of the sun.
	///  @speed The horizontal movement speed.
	void setSunSpeed(float speed)
	{
		sun_speed = speed;
	}

	/// Gets the current horizontal movement speed of the sun.
	///  @returns Horizontal movement speed of the sun.
	float getSunSpeed(void)
	{
		return sun_speed;
	}

	/// Gets the current number of clouds present in the sky.
	///  @returns Number of clouds present in SkyComponent.
	int getCloudCount(void)
	{
		return ccount;
	}

	/// Sets the number of clouds present in the sky.
	void setCloudCount(int count)
	{
		int diff = 0;
		if (count > ccount)	{
			diff = count - ccount;
			while(diff > 0)
			{
				CloudComponent* cmp = clouds.back();
				free(cmp);
				clouds.pop_back();
				diff--;
			}
		} else {
			diff = ccount - count;
			while(diff > 0)
			{
				CloudComponent* cmp = createComponent(sun_speed);
				clouds.push_front(cmp);
				diff--;
			}
		}

		ccount = count;
	}

private:
	/// Sun components
	float sun_speed;
	float sun_x;
	float sun_y;

	/// Constants
	static constexpr float END_BOUND = 200.0f;
	static constexpr float WORLD_END = -200.0f;

	/// Cloud Components (Speed/X/Y)
	std::list<CloudComponent*> clouds;
	int ccount;

	Pixmap img_mask;
	XImage* img_sky;
	Spritesheet* sheet;
};

#endif

