#ifndef _INCL_MOUSESTATE
#define _INCL_MOUSESTATE

/// MouseState
///	 Represents the state of a mouse input device, including mouse cursor position and buttons pressed.
class MouseState
{
public:
	/// Initializes a new instance of the MouseState class.
	MouseState(void)
	{
		_x = 0;
		_y = 0;
	}

	/// Returns the horizontal position of the mouse cursor.
	///  @returns The horizontal position.
	int getX(void)
	{
		return _x;
	}

	/// Returns the vertical position of the mouse cursor.
	///  @returns The vertical position.
	int getY(void)
	{
		return _y;
	}

	/// Specifies the horizontal position of the mouse cursor.
	///  @x The horizontal position of the mouse cursor.
	void setX(int x)
	{
		_x = x;
	}

	/// Specifies the vertical position of the mouse cursor.
	///  @y The vertical position of the mouse cursor.
	void setY(int y)
	{
		_y = y;
	}

private:
	int _x;
	int _y;
};

#endif
