#include "Renderer.h"

Renderer::Renderer()
{
    _renderer = NULL;
}

Renderer::~Renderer()
{
    free();
}

bool Renderer::createRenderer(SDL_Window *window)
{
    _renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    return _renderer != NULL;
}

SDL_Renderer *Renderer::getRendererHandle()
{
    return _renderer;
}

void Renderer::free()
{
    if (_renderer != NULL)
    {
        SDL_DestroyRenderer(_renderer);
        _renderer = NULL;
    }
}