#include "Window.h"

// CONSTRUCTORS / DESTRUCTORS

Window::Window() : _window(NULL), _title("new window"), _width(0), _height(0) {}

Window::Window(const Window &window)
{
    _title = window._title;
    _width = window._width;
    _height = window._height;
    createWindow(_title, _width, _height);
}

Window::Window(Window &&window)
{
    _title = window._title;
    _width = window._width;
    _height = window._height;
    window._window = NULL;
}

Window &Window::operator=(const Window &window)
{
    _title = window._title;
    _width = window._width;
    _height = window._height;
    createWindow(_title, _width, _height);
    return *this;
}

Window &Window::operator=(Window &&window)
{
    _title = window._title;
    _width = window._width;
    _height = window._height;
    window._window = NULL;
    return *this;
}

Window::~Window()
{
    free();
}

// DEALLOCATION

void Window::free()
{
    if (_window != NULL)
    {
        SDL_DestroyWindow(_window);
        _window = NULL;
    }
}

// SDL WINDOW CREATION

bool Window::createWindow(std::string title, int width, int height)
{
    _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    
    _title = title;
    _width = width;
    _height = height;

    return _window != NULL;
}

// EVENT HANDLING

void Window::handleEvent(SDL_Event &e, SDL_Renderer *renderer, bool &hasWindowResized)
{
    if (e.type == SDL_WINDOWEVENT)
    {
        switch (e.window.event)
        {
        // get new dimensions and repaint on window size change
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            hasWindowResized = true;
            _width = e.window.data1;
            _height = e.window.data2;
            SDL_RenderPresent(renderer);
            break;

        // repaint on exposure
        case SDL_WINDOWEVENT_EXPOSED:
            SDL_RenderPresent(renderer);
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

std::string Window::getTitle() { return _title; }

int Window::getWidth() { return _width; }

int Window::getHeight() { return _height; }

bool Window::isMinimized() { return _minimized; }

// SETTERS

void Window::setTitle(std::string title)
{
    _title = title;
    SDL_SetWindowTitle(_window, title.c_str());
}

void Window::setWindowSize(int width, int height)
{
    _width = width;
    _height = height;
    SDL_SetWindowSize(_window, width, height);
}