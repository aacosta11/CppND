#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>

class Renderer
{
public:
    // CONSTRUCTORS / DESTRUCTORS
    Renderer();
    Renderer(const Renderer &renderer) = delete;
    Renderer(const Renderer &&renderer) = delete;
    Renderer &operator=(const Renderer &renderer) = delete;
    Renderer &operator=(const Renderer &&renderer) = delete;
    ~Renderer();

    // DEALLOCATION
    void free();

    // SDL RENDERER CREATION
    bool createRenderer(SDL_Window *window);

    // GET RENDERER HANDLE
    SDL_Renderer *getRendererHandle();

private:
    SDL_Renderer *_renderer;
};

#endif