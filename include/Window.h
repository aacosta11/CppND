#ifndef WINDOW_H
#define WINDOW_H

#include "Texture.h"

/*
This class is from Lazy Foo' Productions
(http://lazyfoo.net/)
*/
class Window
{
	public:
		// CONSTRUCTORS / DESTRUCTORS
		Window();
		Window(const Window &window);
		Window(Window &&window);
		Window &operator=(const Window &window);
		Window &operator=(Window &&window);
		~Window();
		
		// DEALLOCATION
		void free();

		// SDL WINDOW CREATION
		bool createWindow( std::string title, int width, int height );

		// EVENTS
		void handleEvent( SDL_Event& e, SDL_Renderer *renderer, bool &hasWindowResized );

		// GETTERS
		SDL_Window *getWindowHandle();
		std::string getTitle();
		int getWidth();
		int getHeight();
		bool isMinimized();
		
		// SETTERS
		void setTitle( std::string title );
		void setWindowSize( int width, int height );

	private:
		SDL_Window *_window;

		std::string _title;

		int _width;
		int _height;

		bool _fullScreen;
		bool _minimized;
};

#endif