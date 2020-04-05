#pragma once

/// Standard libraries
#include <iostream>
#include <math.h>

/// Vector2
///	 Defines a vector with two components.
class Vector2
{
public:
	/// Initializes a new instance of Vector2.
	Vector2(void) 
	{
		_x = 0;
		_y = 0;
	}

	/// Initializes a new instance of Vector2.
	///  @x Initial value for the x-component of the vector.
	///  @y Initial value for the y-component of the vector.
	Vector2(float x, float y) 
	{
		_x = x;
		_y = y;
	}

	/// Gets the x-component of the vector.
	///  @returns The horizontal component of the vector.
	float getX(void)
	{
		return _x;
	}

	/// Gets the y-component of the vector.
	///  @returns The vertical component of the vector.
	float getY(void)
	{
		return _y;
	}

	/// Sets the x-component of the vector.
	///  @x The value to set.
	void setX(float x)
	{
		_x = x;
	}

	/// Sets the y-component of the vector.
	///  @y The value to set.
	void setY(float y)
	{
		_y = y;
	}

	/// Sets the vector to a specific point.
	///  @y The value to set.
	///  @y The value to set.
	void set(double x, double y)
	{
		_x = x;
		_y = y;
	}

	/// Shifts the current vector.
	///  @xvalue The x-shift value.
	///  @yvalue The y-shift value.
	void move(float xvalue, float yvalue)
	{
		_x += xvalue;
		_y += yvalue;
	}

	/// Calculates the distance between two vectors.
	///  @value1 Source vector. 
	///  @value2 Source vector.
	///  @returns Distance between the two vectors.
	static float distance(Vector2* value1, Vector2* value2) 
	{
		float xd = value1->_x - value2->_x;
		float yd = value1->_y - value2->_y;
		return sqrt(xd * xd + yd * yd);
	}

	/// Adds two vectors.
	///  @value1 Source vector. 
	///  @value2 Source vector.
	///  @returns Sum of the source vectors.
	static Vector2* add(Vector2* value1, Vector2* value2) 
	{
		return new Vector2(value1->_x + value2->_x, value1->_y + value2->_y);
	}

	/// Subtracts a vector from a vector.
	///  @value1 Source vector. 
	///  @value2 Source vector.
	///  @returns Result of the subtraction.
	static Vector2* sub(Vector2* value1, Vector2* value2) 
	{
		return new Vector2(value1->_x - value2->_x, value1->_y - value2->_y);
	}

private:
	float _x;
	float _y;
};
