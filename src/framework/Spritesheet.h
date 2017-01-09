#ifndef _INCL_SPRITESHEET
#define _INCL_SPRITESHEET

/// X11/XLib libraries
#include <X11/Xlib.h>
#include <X11/Xutil.h>

/// Spritesheet
///  A uniform sheet of sprites that can be drawn individually.
class Spritesheet
{
public:
	/// Create a new sprite sheet based on a image margins.
	///  @image The image to be represented by the sprite sheet.
	///  @xlength The horizontal number of sprite represented by the sheet.
	///  @ylength The vertical number of sprite represented by the sheet.
	///  @margins The uniform margin around each sprite.
	Spritesheet(XImage* image, int xlength, int ylength, int margins)
	{
		img = image;
		xcount = xlength;
		ycount = ylength;

		padding = margins;

		spritex = image->width / xlength;
		spritey = image->height / ylength;

		spriteWidth = spritex - 2 * padding;
		spriteHeight = spritey - 2 * padding;
	}

	/// Retrieves the image coordinate from the sheet grid coordinate.
	///  @x The sheet vertical position.
	///  @y The sheet horizontal position.
	///  @sourceX The image horizontal position in the sprite sheet.
	///  @sourceY The image vertical position in the sprite sheet.
	void getInfo(int x, int y, int* sourceX, int* sourceY)
	{		
		if(x < 0 || x >= xcount)
			return;

		if(y < 0 || y >= ycount)
			return;

		*sourceX = (x * spritex) + padding;
		*sourceY = (y * spritey) + padding;
	}

	/// Gets the image coordinate position from an index.
	///  @index The index of an image within the sprite sheet.
	///  @sourceX The image horizontal position in the sprite sheet.
	///  @sourceY The image vertical position in the sprite sheet.
	void getInfo(int index,	int* sourceX, int* sourceY)
	{
		if(index < 0 || index >= (xcount * ycount))
			return;

		int posx = index % xcount;
		int posy = index / xcount;

		*sourceX = (posx * spritex) + padding;
		*sourceY = (posy * spritey) + padding;
	}

	/// Returns the underlying image of the spritesheet.
	///  @returns The underlying sheet image.
	XImage* getImage(void)
	{
		return img;
	}

	/// Get the horizontal sprite number capacity.
	///  @returns The horizontal sprite number.
	int getXLength(void)
	{
		return xcount;
	}

	/// Get the vertical sprite number capacity.
	///  @returns The vertical sprite number.
	int getYLength(void)
	{
		return ycount;
	}

	/// Get the number of sprites on the sheet.
	///  @returns The total number of sprites possible in the spritesheet.
	int getCount(void)
	{
		return xcount * ycount;
	}

	/// Returns the width of an individual sprite.
	///  @returns The width of a sprite.
	int getSpriteWidth(void)
	{
		return spriteWidth;
	}

	/// Returns the height of an individual sprite.
	///  @returns The height of a sprite.
	int getSpriteHeight(void)
	{
		return spriteHeight;
	}

private:
	// Column & row count of images
	int xcount;
	int ycount;

	// Height & width of image with padding
	int spritex;
	int spritey;

	// Height & width of image with padding removed
	int spriteWidth;
	int spriteHeight;

	// Padding of his image
	int padding;
	XImage* img;
};

#endif

