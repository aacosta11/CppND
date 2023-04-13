#include "Button.h"

// CONSTRUCTORS / DESTRUCTORS

Button::Button()
{
    _rect = {0, 0, 0, 0};
    _texture = Texture();
}

Button::Button(const Button &button)
{
    _rect = button._rect;
    _texture = button._texture;
}

Button::Button(Button &&button)
{
    _rect = button._rect;
    _texture = std::move(button._texture);

    button._rect = {0, 0, 0, 0};
}

Button &Button::operator=(const Button &button)
{
    _rect = button._rect;
    _texture = button._texture;

    return *this;
}

Button &Button::operator=(Button &&button)
{
    _rect = button._rect;
    _texture = std::move(button._texture);

    button._rect = {0, 0, 0, 0};

    return *this;
}

Button::~Button() { }

// RENDERING

bool Button::loadFromFile(SDL_Renderer *renderer, std::string path)
{
    if (!_texture.loadFromFile(renderer, path)) return false;
    _rect.w = _texture.getWidth();
    _rect.h = _texture.getHeight();
    return true;
}

void Button::render(SDL_Renderer *renderer)
{
    _texture.render(renderer, _rect.x, _rect.y, NULL);
}

// EVENTS

void Button::handleSDLEvent(SDL_Event &e, bool &flag, bool shouldBeListening)
{
    if (!shouldBeListening) return;
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (x >= _rect.x && x <= _rect.x + _rect.w && y >= _rect.y && y <= _rect.y + _rect.h)
        {
            flag = true;
        }
    }
}

// GETTERS

SDL_Rect Button::getRect() { return _rect; }

int Button::getWidth() { return _rect.w; }

int Button::getHeight() { return _rect.h; }

// SETTERS

void Button::setPosition(int x, int y)
{
    _rect.x = x;
    _rect.y = y;
}

void Button::setDimensions(int w, int h)
{
    _rect.w = w;
    _rect.h = h;
}