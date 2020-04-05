#pragma once

/// Standard libraries
#include <cmath>
#include <stdlib.h>

/// Contains commonly used functions and precalculated values.
namespace MATH
{
	/// Restricts a value to be within a specified range.
	///  @value The value to clamp.
	///  @min The minimum value. If value is less than min, min will be returned.
	///  @max The maximum value. If value is greater than max, max will be returned.
	///  @returns The clamped value.
	static float clamp(float value, float min, float max)
	{
		if(value < min)
		{
			return min;
		}
		else if(value > max)
		{
			return max;
		}
		return value;
	}

	/// Restricts a value to be within a specified range.
	///  @value The value to clamp.
	///  @min The minimum value. If value is less than min, min will be returned.
	///  @max The maximum value. If value is greater than max, max will be returned.
	///  @returns The clamped value.
	static int iclamp(float value, int min, int max)
	{
		if(value < min)
		{
			return min;
		}
		else if(value > max)
		{
			return max;
		}
		return (int)value;
	}

	/// Returns the largest integer less than or equal to the specified floating-point number.
	///  @value A floating-point number.
	///  @returns The largest integer less than or equal to 'value'.
	static int ifloor(float value)
	{
		return (int)floor(value);
	}

	/// Returns the smallest integral value that is greater than or equal to the specified floating-point number.
	///  @value A floating-point number.
	///  @returns The smallest integral value that is greater than or equal to 'value'. 
	static int iceiling(float value)
	{
		return (int)ceil(value);
	}

	/// Determines the depth a specified Rectangle intersects with this Rectangle.
	///  @rectA The Rectangle to evaluate.
	///  @rectB The Rectangle to evaluate.
	///  @returns A vector component represents the vertical and horizontal intersection.
	static Vector2* getIntersectionDepth(Rectangle* rectA, Rectangle* rectB)
	{
		// Calculate half sizes.
		float halfWidthA = rectA->getWidth() / 2.0f;
		float halfHeightA = rectA->getHeight() / 2.0f;
		float halfWidthB = rectB->getWidth() / 2.0f;
		float halfHeightB = rectB->getHeight() / 2.0f;

		// Calculate centers.
		Vector2 centerA(rectA->getLeft() + halfWidthA, rectA->getTop() + halfHeightA);
		Vector2 centerB(rectB->getLeft() + halfWidthB, rectB->getTop() + halfHeightB);

		// Calculate current and minimum-non-intersecting distances between centers.
		float distanceX = centerA.getX() - centerB.getX();
		float distanceY = centerA.getY() - centerB.getY();
		float minDistanceX = halfWidthA + halfWidthB;
		float minDistanceY = halfHeightA + halfHeightB;

		// If we are not intersecting at all, return (0, 0).
		if (abs(distanceX) >= minDistanceX || abs(distanceY) >= minDistanceY)
		{
			return new Vector2(0, 0);
		}

		// Calculate and return intersection depths.
		float depthX = distanceX > 0 ? minDistanceX - distanceX : -minDistanceX - distanceX;
		float depthY = distanceY > 0 ? minDistanceY - distanceY : -minDistanceY - distanceY;

		return new Vector2(depthX, depthY);
	}
}
