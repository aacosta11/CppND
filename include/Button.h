#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "Texture.h"

class Button
{
public:
    // CONSTRUCTORS / DESTRUCTORS
    Button();
    Button(const Button &button);
    Button(Button &&button);
    Button &operator=(const Button &button);
    Button &operator=(Button &&button);
    ~Button();

    // RENDERING
    bool loadFromFile(SDL_Renderer *renderer, std::string path);
    void render(SDL_Renderer *renderer);

    // EVENTS
    void handleSDLEvent(SDL_Event &e, bool &flag);

    // GETTERS
    SDL_Rect getRect();
    int getWidth();
    int getHeight();

    // SETTERS
    void setPosition(int x, int y);
    void setDimensions(int w, int h);
    
private:
    SDL_Rect _rect;
    Texture _texture;
};

#endif // BUTTON_H
