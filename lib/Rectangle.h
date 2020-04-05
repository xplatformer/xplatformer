#pragma once

/// Standard libraries
#include <math.h>

/// Project components
#include "Vector2.h"

/// Rectangle
///  Defines a rectangle.
class Rectangle
{
public:
	/// Initializes a new instance of Rectangle.
	///  @x The x-coordinate of the rectangle.
	///  @y The y-coordinate of the rectangle.
	///  @width Width of the rectangle.
	///  @height Height of the rectangle.
	Rectangle(float x, float y, float width, float height)
	{
		_x = x;
		_y = y;
		_width = width;
		_height = height;
	}

	/// Specifies the x-coordinate of the rectangle.
	///  @x The value to set.
	void setX(float x)
	{
		_x = x;
	}

	/// Specifies the y-coordinate of the rectangle.
	///  @y The value to set.
	void setY(float y)
	{
		_y = y;
	}

	/// Specifies the coordinates of the rectangle.
	///  @x The value to set.
	///  @y The value to set.
	void setPoint(float x, float y)
	{
		_x = x;
		_y = y;
	}

	/// Specifies the width of the rectangle.
	///  @width The horizontal length of the rectangle.
	void setWidth(float width)
	{
		_width = width;
	}

	/// Gets the height of the rectangle.
	///  @height The vertical length of the rectangle.
	void setHeight(float height)
	{
		_height = height;
	}

	/// Gets the width of the rectangle.
	///  @returns Rectangle width.
	float getWidth(void)
	{
		return _width;
	}

	/// Gets the height of the rectangle.
	///  @returns Rectangle height.
	float getHeight(void)
	{
		return _height;
	}

	/// Returns the y-coordinate of the bottom of the rectangle.
	/// @returns The rectangle bottom y-coordinate.
	float getBottom(void)
	{
		return _y + _width;
	}

	/// Returns the y-coordinate of the top of the rectangle.
	/// @returns The rectangle top y-coordinate.
	float getTop(void)
	{
		return _y;
	}

	/// Returns the x-coordinate of the center of the rectangle.
	/// @returns The rectangle center x-coordinate.
	float getCenterX(void)
	{
		return _x + (_width / 2.0f);
	}

	/// Returns the y-coordinate of the center of the rectangle.
	/// @returns The rectangle center y-coordinate.
	float getCenterY(void)
	{
		return _y + (_height / 2.0f);
	}

	/// Gets the Point that specifies the center of the rectangle.
	/// @returns The center coordinate.
	Vector2 getCenter(void)
	{
		float halfWidth = _width / 2.0f;
		float halfHeight = _height / 2.0f;
		Vector2 vector(_x + halfWidth, _y + halfHeight);
		return vector;
	}

	/// Returns the x-coordinate of the left side of the rectangle.
	///  @returns The rectangle left x-coordinate.
	float getLeft(void)
	{
		return _x;
	}

	/// Returns the x-coordinate of the right side of the rectangle.
	///  @returns The rectangle right x-coordinate.
	float getRight(void)
	{
		return _x + _width;
	}

	/// Gets the upper-left value of the Rectangle.
	///  @returns The rectangle left x-coordinate.
	float getLocation(void)
	{
		return _x;
	}

	/// Determines the depth of horizontal intersection between rectangles.
	///  @rectA Source rectangle.
	///  @rectB Source rectangle.
	///  @returns Horizontal intersection depth.
	static float getHorizontalIntersectionDepth(Rectangle rectA, Rectangle rectB)
	{
		// Calculate half sizes.
		float halfWidthA = rectA.getWidth() / 2.0f;
		float halfWidthB = rectB.getWidth() / 2.0f;

		// Calculate centers.
		float centerA = rectA.getLeft() + halfWidthA;
		float centerB = rectB.getLeft() + halfWidthB;

		// Calculate current and minimum-non-intersecting distances between centers.
		float distanceX = centerA - centerB;
		float minDistanceX = halfWidthA + halfWidthB;

		// If we are not intersecting at all, return (0, 0).
		if (abs(distanceX) >= minDistanceX)
			return 0.0;

		// Calculate and return intersection depths.
		return distanceX > 0 ? minDistanceX - distanceX : -minDistanceX - distanceX;
	}

	/// Determines the depth of horizontal intersection between rectangles.
	///  @rectA Source rectangle.
	///  @rectB Source rectangle.
	///  @returns Vertical intersection depth.
	static float getVerticalIntersectionDepth(Rectangle rectA, Rectangle rectB)
	{
		// Calculate half sizes.
		float halfHeightA = rectA.getHeight() / 2.0f;
		float halfHeightB = rectB.getHeight() / 2.0f;

		// Calculate centers.
		float centerA = rectA.getTop() + halfHeightA;
		float centerB = rectB.getTop() + halfHeightB;

		// Calculate current and minimum-non-intersecting distances between centers.
		float distanceY = centerA - centerB;
		float minDistanceY = halfHeightA + halfHeightB;

		// If we are not intersecting at all, return (0, 0).
		if (abs(distanceY) >= minDistanceY)
			return 0.0;

		// Calculate and return intersection depths.
		return distanceY > 0 ? minDistanceY - distanceY : -minDistanceY - distanceY;
	}

private:
	float _x;
	float _y;
	float _width;
	float _height;
};
