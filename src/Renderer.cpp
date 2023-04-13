#include "Renderer.h"

// CONSTRUCTORS / DESTRUCTORS

Renderer::Renderer() : _renderer(NULL) { }

Renderer::~Renderer()
{
    free();
}

// DEALLOCATION

void Renderer::free()
{
    if (_renderer != NULL)
    {
        SDL_DestroyRenderer(_renderer);
        _renderer = NULL;
    }
}

// SDL RENDERER CREATION

bool Renderer::createRenderer(SDL_Window *window)
{
    _renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    return _renderer != NULL;
}

// GET RENDERER HANDLE

SDL_Renderer *Renderer::getRendererHandle()
{
    return _renderer;
}