#ifndef _INCL_ANIMATION
#define _INCL_ANIMATION

/// Animation
///	 The Animation class is designed to store the start and end index of an animation.
class Animation
{
public:
	/// Initializes a new instance of Animation.
	///  @start The start index of the animation.
	///  @end The end index of the animation.
	Animation(int start, int end)
	{
		_start = start;
		_end = end;
	}

	/// Gets the starting index.
	///  @returns The starting index of the animation.
	int getStart(void)
	{
		return _start;
	}

	/// Gets the ending index.
	///  @returns The end index of the animation.
	int getEnd(void)
	{
		return _end;
	}

	/// The animation delay coefficient.
	static const int ANIMATION_DELAY = 1;

private:
	int _start;
	int _end;
};

#endif
