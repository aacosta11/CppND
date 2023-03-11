#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <memory>

class Renderer
{
public:
    Renderer();
    ~Renderer();
    bool createRenderer(SDL_Window *window);
    SDL_Renderer *getRendererHandle();

private:
    void free();
    SDL_Renderer *_renderer;
};

#endif