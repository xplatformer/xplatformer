#pragma once

#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysymdef.h>

#include "Spritesheet.h"
#include "KeyboardState.h"
#include "MouseState.h"
#include "Rectangle.h"
#include "Logger.h"

namespace Constants
{
	/// The default border size of the application.
	static const int DEFAULT_BORDER = 5;

	/// The default buffer size. 
	static const int DEFAULT_BUFFERSIZE = 10;

	/// The default width of the window.
	static const int DEFAULT_WINDOW_WIDTH = 800;

	/// The default height of the window.
	static const int DEFAULT_WINDOW_HEIGHT = 600;

	/// The default input masks.
	static const unsigned int DEFAULT_INPUT_MASK = ButtonPressMask	| KeyPressMask | KeyRelease | PointerMotionMask | EnterWindowMask | LeaveWindowMask | StructureNotifyMask;

	/// The default title of the window.
	static const char* DEFAULT_TITLE = "XLib Window";
}

/// Represents a collection of constants defining XLib colors.
namespace ColorConstants
{
	/// Gets a system-defined color with the value (0xFF0000L).
	const unsigned long COLOR_RED = 0xFF0000L;

	/// Gets a system-defined color with the value (0xFFFF00L).
	const unsigned long COLOR_YELLOW = 0xFFFF00L;

	/// Gets a system-defined color with the value (0x00FF00L).
	const unsigned long COLOR_GREEN = 0x00FF00L;

	/// Gets a system-defined color with the value (0x0000FFL).
	const unsigned long COLOR_BLUE = 0x0000FFL;

	/// Gets a system-defined color with the value (0xFF00FFL).
	const unsigned long COLOR_VIOLET = 0xFF00FFL;

	/// Gets a system-defined color with the value (0x000000L).
	const unsigned long COLOR_BLACK = 0x000000L;

	/// Gets a system-defined color with the value (0x7F7F7FL).
	const unsigned long COLOR_GREY = 0x7F7F7FL;

	/// Gets a system-defined color with the value (0xAFAFAFL).
	const unsigned long COLOR_LIGHTGREY = 0xAFAFAFL;

	/// Gets a system-defined color with the value (0xFFFFFFL).
	const unsigned long COLOR_WHITE = 0xFFFFFFL;
}

/// XInfo
///	 Performs image rendering, creates resources, handles system-level interactions and contains resources.
class XInfo
{
public:
	/// XInfo constructor.
	XInfo(void)
	{
		border = Constants::DEFAULT_BORDER;
		input_mask = Constants::DEFAULT_INPUT_MASK;
		title = Constants::DEFAULT_TITLE;
	}

	/// XInfo destructor.
	~XInfo(void)
	{
	}

	/// Initializes the standard variables of the wrapper component.
	///  @argc The number of arguments.
	///  @argv The arguments list.
	///  @border The size of the window border.
	///  @input_mask The events that should the application should be notified of.
	///  @title The title of the window.
	///  @icon The filename of the window icon.
	void initialize(int argc, char* argv[])
	{
		// Display opening uses the DISPLAY	environment variable.
		// It can go wrong if DISPLAY isn't set, or you don't have permission.
		display = XOpenDisplay("");
		if (!display)
		{
			Logger::application_error("Can't open display.");
		}

		// Prevents commands (such as keypress/keyrelease) from being re-sent
		XAutoRepeatOff(display);

		unsigned long white, black;

		// Find out some things about the display you're using.	
		screen = DefaultScreen(display);
		depth = DefaultDepth(display, screen);
		
		white = XWhitePixel(display, screen);
		black = XBlackPixel(display, screen);

		hints.x = 100;
		hints.y = 100;
		hints.width = Constants::DEFAULT_WINDOW_WIDTH;
		hints.height = Constants::DEFAULT_WINDOW_HEIGHT;
		hints.flags = PPosition | PSize;

		// display where window appears
		// window's parent in window tree
		// upper left corner location
		// size of the window
		// width of window's border
		// window border colour
		// window background colour
		window = XCreateSimpleWindow(display, DefaultRootWindow(display), hints.x, hints.y, hints.width, hints.height, border, black, white);					       

		// reads a file and sets it as the image (its a CC symbol)
		Pixmap map;
		if(icon == NULL)
		{
			map = readPixmap(icon);
		}

		// display containing the window
		// window whose properties are set
		// window's title
		// icon's title
		// pixmap for the icon
		// applications command line args
		// size hints for the window
		XSetStandardProperties(display, window, title, "SD", map, argv, argc, &hints);			

		// Create Graphics Contexts
		gdraw = gcontext[0] = XCreateGC(display, window, 0, NULL);
		XSetForeground(display, gdraw, white);
		XSetBackground(display, gdraw, black);
		XSetFillStyle(display, gdraw, FillSolid);

		gtext = gcontext[1] = XCreateGC(display, window, 0, NULL);
		XSetBackground(display, gtext, white);

		font = XLoadQueryFont(display, "*x24");
		if (font == NULL) {
			Logger::application_error("Can't load font. Please install xfonts-75dpi & run fc-cache -f -v");
		}
		XSetFont(display, gtext, font->fid);

		int depth = DefaultDepth(display, DefaultScreen(display));
		pixmap = XCreatePixmap(display, window, hints.width, hints.height, depth);	
		pix_bounds = new Rectangle(0, 0, hints.width, hints.height);

		mouse = new MouseState();
		keyboard = new KeyboardState();

		XSelectInput(display, window, input_mask);		
	}

	/// Loads an image from a file path into the specified image pointer.
	///  @filename Filename, relative to the loader root directory, and including the extension.
	///  @img A pointer to the loaded image asset.
	///  @returns True if successful, false otherwise.
	/// Notes:
	///		This method was based the following stackoverflow response to a question regarding loading TGA files.
	///		METHOD URL: http://stackoverflow.com/a/7050007/2127492
	bool loadImage(const char* filename, XImage** img)
	{
		FILE *filePtr;
		char ucharBad;
		short int sintBad;
		long imageSize;
		int colorMode;

		unsigned char imageTypeCode;
		short int imageWidth;
		short int imageHeight;
		unsigned char bitCount;
		char* imageData;

		// Open the TGA file.
		filePtr = fopen(filename, "rb");
		if (filePtr == NULL)
		{
			return NULL;
		}

		// Read the two first bytes we don't need.
		fread(&ucharBad, sizeof(char), 1, filePtr);
		fread(&ucharBad, sizeof(char), 1, filePtr);

		// Which type of image gets stored in imageTypeCode.
		fread(&imageTypeCode, sizeof(char), 1, filePtr);

		// For our purposes, the type code should be 2 (uncompressed RGB image)
		// or 3 (uncompressed black-and-white images).
		if (imageTypeCode != 2 && imageTypeCode != 3)
		{
			fclose(filePtr);
			return false;
		}

		// Read 13 bytes of data we don't need.
		fread(&sintBad, sizeof(short int), 1, filePtr);
		fread(&sintBad, sizeof(short int), 1, filePtr);
		fread(&ucharBad, sizeof(char), 1, filePtr);
		fread(&sintBad, sizeof(short int), 1, filePtr);
		fread(&sintBad, sizeof(short int), 1, filePtr);

		// Read the image's width and height.
		fread(&imageWidth, sizeof(short int), 1, filePtr);
		fread(&imageHeight, sizeof(short int), 1, filePtr);

		// Read the bit depth.
		fread(&bitCount, sizeof(char), 1, filePtr);

		// Read one byte of data we don't need.
		fread(&ucharBad, sizeof(char), 1, filePtr);

		// Color mode -> 3 = BGR, 4 = BGRA.
		colorMode = bitCount / 8;
		imageSize = imageWidth * imageHeight * colorMode;

		// Allocate memory for the image data.
		imageData = (char*)malloc(sizeof(char)*imageSize);

		// Read the image data.
		fread(imageData, sizeof(char), imageSize, filePtr);

		char *image32 = (char*)malloc(imageWidth * imageHeight * 4);
		char *p = image32;

		// Change from BGR to RGB so OpenGL can read the image data.
		for (int imageIdx = 0; imageIdx < imageSize; imageIdx += colorMode)
		{
			*(p + 0) = imageData[imageIdx + 0];  // B
			*(p + 1) = imageData[imageIdx + 1];  // G
			*(p + 2) = imageData[imageIdx + 2];  // R
			p = p + 4;
		}
		fclose(filePtr);

		(*img) = XCreateImage(display, CopyFromParent, depth, ZPixmap, 0, image32, imageWidth, imageHeight, 32, 0);

		return true;
	}

	/// Loads an image and its associated clipping mask from two file locations.
	///  @filename Filename of the image, relative to the loader root directory, and including the extension.
	///  @img A pointer to the loaded image asset.
	///  @clipFile Filename of the clipmap, relative to the loader root directory, and including the extension.
	///  @pxm A pointer to the loaded clipmap asset.
	///  @returns True if successful, false otherwise.
	bool loadImage(const char* filename, XImage** img, const char* clipFile, Pixmap* pxm)
	{
		bool readImage = loadImage(filename, img);
		if(!readImage)
		{
			return readImage;
		}

		unsigned bw = 0, bh = 0;
		int hsx = 0, hsy = 0;
		XReadBitmapFile(display, pixmap, clipFile, &bw, &bh, pxm, &hsx, &hsy);

		return true;
	}

	/// Loads a pixmap from a file path into the specified pixmap pointer.
	///  @filename Filename, relative to the loader root directory, and including the extension.
	///  @returns The loaded pixmap asset.
	Pixmap readPixmap(const char* filename)
	{
		unsigned bw = 0, bh = 0;
		int hsx = 0, hsy = 0;
		Pixmap map;

		XReadBitmapFile(display, window, filename, &bw, &bh, &map, &hsx, &hsy);
		return map;
	}

	/// Draws an image with a clipping mask.
	///  @x The x-coordinate (in screen coordinates) to draw the image.
	///  @y The y-coordinate (in screen coordinates) to draw the image.
	///  @posx The x-coordinate (in image coordinates) to draw the image.
	///  @posy The y-coordinate (in image coordinates) to draw the image.
	///  @width The width of the image to draw.
	///  @height The height of the image to draw.
	///  @img A pointer to the image asset to be drawn.
	///  @mask A pointer to the clipmask of the image.
	void draw(int x, int y,	int posx, int posy,	int width, int height, XImage* img, Pixmap mask)
	{
		int srcx = x - posx;
		int srcy = y - posy;

		XSetClipMask(display, gdraw, mask);
		XSetClipOrigin(display, gdraw, srcx, srcy);		

		XPutImage(display, pixmap, gdraw, img,
			posx, posy,
			x, y,
			width, height);

		XSetClipMask(display, gdraw, None);
	}

	/// Draws an image from a spritesheet.
	///  @sheet The spritesheet to draw the image from.
	///  @x The x-coordinate (in screen coordinates) to draw the image.
	///  @y The y-coordinate (in screen coordinates) to draw the image.
	///  @index The index of the image to be drawn.
	void draw(Spritesheet* sheet, int x, int y, int index)
	{
		int srcx, srcy, posx, posy;

		sheet->getInfo(index, &posx, &posy);

		srcx = x - posx;
		srcy = y - posy;

		XSetClipOrigin(display, gdraw, srcx, srcy);

		XPutImage(display, pixmap, gdraw, sheet->getImage(),
			posx, posy,
			x, y,
			sheet->getSpriteWidth(),
			sheet->getSpriteHeight());
	}

	/// Adds a string to a batch of sprites for rendering using the specified font, text, position, and color.
	///  @str A text string.
	///  @x The x-coordinate (in screen coordinates) to draw the image.
	///  @y The y-coordinate (in screen coordinates) to draw the image.
	///  @colour The color to tint a string.
	void drawString(std::string str, int x, int y, unsigned long colour)
	{
		const char* text = str.c_str();
		int length = str.length();
		GC gc_text = getTextDevice();

		XSetForeground(display, gc_text, 0UL);
		for(int rx = -1; rx <= 1; rx++)
		{
			for(int ry = -1; ry <= 1; ry++)
			{
				XDrawString(display, pixmap, gc_text, x - rx, y - ry, text, length);
			}
		}

		XSetForeground(display, gc_text, colour);
		XDrawString(display, pixmap, gc_text, x, y,	text, length);
	}

	/// Draws a rectangle outline to the screen.
	///  @gc The graphic context to be used when drawing the rectangle.
	///  @x The x-coordinate of the rectangle.
	///  @y The y-coordinate of the rectangle.
	///  @width The width of the rectangle.
	///  @height The height of the rectangle. 
	void drawRectangle(GC gc, int x, int y, unsigned int width, unsigned int height)
	{
		XDrawRectangle(display, pixmap, gc, x, y, width, height);
	}

	/// Draws a rectangle to the screen.
	///  @gc The graphic context to be used when drawing.
	///  @x The x-coordinate of the rectangle.
	///  @y The y-coordinate of the rectangle.
	///  @width The width of the rectangle.
	///  @height The height of the rectangle. 
	void fillRectangle(GC gc, int x, int y, unsigned int width, unsigned int height)
	{
		XFillRectangle(display, pixmap, gc, x, y, width, height);
	}

	/// Sets the draw color of the graphic context.
	///  @gc The graphic context to be used when drawing.
	///  @value The color value to specify.
	void setColor(GC gc, const unsigned long value)
	{
		XSetForeground(display, gc, value); 
	}

	/// Sets the clip mask of the sprite graphics context.
	///  @img_mask Specifies the pixmap of the graphics device.
	void setMask(Pixmap img_mask)
	{
		XSetClipMask(display, gdraw, img_mask);
	}

	/// Clears the clip mask of the sprite graphics context.
	void clearMask(void)
	{
		XSetClipMask(display, gdraw, None);
	}

	/// Clears image resource buffers.
	void clear(void)
	{
		XFillRectangle(display, pixmap, gdraw, 0, 0, getImageWidth(), getImageHeight());
	}

	/// Presents the display with the contents of the buffer in the sequence of back buffers owned by the XInfo.
	void flush(void)
	{
		XCopyArea(display, pixmap, window, gdraw,	0, 0, getImageWidth(), getImageHeight(), pix_bounds->getLeft(), pix_bounds->getTop());

		XFlush(display);
	}

	/// Opens the window.
	void openw(void)
	{
		XMapRaised(display, window);		
		XFlush(display);

		// let server get set up before sending drawing commands
		wait(2);	
	}

	/// Closes the current window and display.
	void close(void)
	{
		XCloseDisplay(display);
	}

	/// Sleeps the game for a period of milliseconds.
	///  @time The number of milliseconds to sleep the game.
	void wait(long time)
	{
		usleep(time);
	}

	/// Returns the window size hints.
	///  @returns The window size hints.
	XSizeHints getWindowHints(void)
	{
		return hints;
	}

	/// Returns the current Keyboard state.
	///  @returns The current keyboard state. 
	KeyboardState* getKeyboardState(void)
	{
		return keyboard;
	}

	/// Returns the current Mouse state.
	///  @returns The current mouse state. 
	MouseState* getMouseState(void)
	{
		return mouse;
	}

	/// Gets the current display device.
	///  @returns The current display device.
	Display* getDisplay(void)
	{
		return display;
	}

	/// Gets the underlying operating system window.
	///  @returns The application window.
	Window getWindow(void)
	{
		return window;
	}

	/// Gets the screen number for the application.
	///  @returns The screen number.
	int getScreen(void)
	{
		return screen;
	}

	/// Creates a graphic context from the current display.
	///  @returns A newly created graphic context.
	GC createGraphicContext(void)
	{
		return XCreateGC(display, window, 0, 0);
	}

	/// Gets the collection of graphic contexts for the application.
	///  @returns A collection of graphic contexts.
	GC* getGraphicDevices(void)
	{
		return gcontext;
	}

	/// Returns the graphic context associated with image-based rendering.
	///  @returns The graphic context responsible for image-based rendering.
	GC getGraphicContext(void)
	{
		return gcontext[0];
	}

	/// Returns the graphics context associated with text-based rendering.
	///  @returns The graphic context responsible for text-based rendering.
	GC getTextDevice(void)
	{
		return gcontext[1];
	}

	/// Returns the default font used when rendering text.
	///  @returns A graphics font.
	XFontStruct* getDefaultFont(void)
	{
		return font;
	}

	/// Returns the render-target surface for the application.
	///  @returns The render-target surface of the application.
	Pixmap getImageBuffer(void)
	{
		return pixmap;
	}

	/// Returns the window dimensions of a render-target surface.
	///  @returns The dimensions of the image buffer.
	Rectangle* getGraphicBounds(void)
	{
		return pix_bounds;
	}

	/// Returns the window width of a render-target surface.
	///  @returns The width of the image buffer.
	float getImageWidth(void)
	{
		return pix_bounds->getWidth();
	}

	/// Returns the window height of a render-target surface.
	///  @returns The height of the image buffer.
	float getImageHeight(void)
	{
		return pix_bounds->getHeight();
	}

	/// Sets the icon of the window for initialization.
	void setIcon(const char* wicon)
	{
		icon = wicon;
	}

	/// Sets the title of the window for initialization.
	void setTitle(const char* wtitle)
	{
		title = wtitle;
	}

private:
	/// XLib variables
	Display *display;
	Window window;
	XSizeHints hints;
	int windowWidth, windowHeight;

	/// Graphic contexts of the xinfo
	GC gcontext[2];
	GC gdraw;
	GC gtext;

	XFontStruct* font;
	int screen;
	unsigned int depth;

	Pixmap pixmap;
	Rectangle* pix_bounds;

	/// Input state managements
	KeyboardState* keyboard;
	MouseState* mouse;

	int border;
	unsigned int input_mask;

	// Information
	const char* title = NULL;
	const char* icon = NULL;
};
