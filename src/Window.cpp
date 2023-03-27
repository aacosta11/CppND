#include "Window.h"

// CONSTRUCTORS / DESTRUCTORS

Window::Window()
{
    _window = NULL;
    _mouseFocus = false;
    _keyboardFocus = false;
    _width = 0;
    _height = 0;
}

Window::~Window()
{
    free();
}

// WINDOW CREATION

bool Window::createWindow(std::string title = "new window", int width = 0, int height = 0)
{
    _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

    _mouseFocus = true;
    _keyboardFocus = true;
    _width = width;
    _height = height;

    return _window != NULL;
}

// DEALLOCATE RESOURCES

void Window::free()
{
    if (_window != NULL)
    {
        SDL_DestroyWindow(_window);
        _window = NULL;
    }
}

// EVENT HANDLING

void Window::handleEvent(SDL_Event &e, SDL_Renderer *renderer, bool &hasWindowResized)
{
    if (e.type == SDL_WINDOWEVENT)
    {
        bool updateCaption = false;

        switch (e.window.event)
        {
        // get new dimensions and repaint on window size change
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            _width = e.window.data1;
            _height = e.window.data2;
            hasWindowResized = true;
            SDL_RenderPresent(renderer);
            break;

        // repaint on exposure
        case SDL_WINDOWEVENT_EXPOSED:
            SDL_RenderPresent(renderer);
            break;

        // mouse entered window
        case SDL_WINDOWEVENT_ENTER:
            _mouseFocus = true;
            updateCaption = true;
            break;

        // mouse left window
        case SDL_WINDOWEVENT_LEAVE:
            _mouseFocus = false;
            updateCaption = true;
            break;

        // window has keyboard focus
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            _keyboardFocus = true;
            updateCaption = true;
            break;

        // window lost keyboard focus
        case SDL_WINDOWEVENT_FOCUS_LOST:
            _keyboardFocus = false;
            updateCaption = true;
            break;

        // window minimized
        case SDL_WINDOWEVENT_MINIMIZED:
            _minimized = true;
            break;

        // window maxized
        case SDL_WINDOWEVENT_MAXIMIZED:
            _minimized = false;
            break;

        // window restored
        case SDL_WINDOWEVENT_RESTORED:
            _minimized = false;
            break;
        }
    }
    // enter exit full screen on return key
    else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F11)
    {
        if (_fullScreen)
        {
            SDL_SetWindowFullscreen(_window, SDL_FALSE);
            _fullScreen = false;
        }
        else
        {
            SDL_SetWindowFullscreen(_window, SDL_TRUE);
            _fullScreen = true;
            _minimized = false;
        }
    }
}

// GETTERS

SDL_Window *Window::getWindowHandle() { return _window; }

int Window::getWidth() { return _width; }

int Window::getHeight() { return _height; }

bool Window::hasMouseFocus() { return _mouseFocus; }

bool Window::hasKeyboardFocus() { return _keyboardFocus; }

bool Window::isMinimized() { return _minimized; }

// SETTERS

void Window::setWindowSize(int width, int height)
{
    _width = width;
    _height = height;
    SDL_SetWindowSize(_window, width, height);
}