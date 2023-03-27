#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <memory>
#include <stdexcept>
#include <stdio.h>

#include "Texture.h"


class Window
{
	public:
		Window();
		~Window();

		void free();

		bool createWindow( std::string title, int width, int height );

		void handleEvent( SDL_Event& e, SDL_Renderer *renderer, bool &hasWindowResized );

		SDL_Window *getWindowHandle();
		int getWidth();
		int getHeight();
		bool hasMouseFocus();
		bool hasKeyboardFocus();
		bool isMinimized();

		void setWindowSize( int width, int height );

	private:
		SDL_Window *_window;

		int _width;
		int _height;

		bool _mouseFocus;
		bool _keyboardFocus;
		bool _fullScreen;
		bool _minimized;
};

#endif