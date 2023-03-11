#include "Texture.h"

// CONSTRUCTORS / DESTRUCTORS

Texture::Texture()
{
    _texture = NULL;
    _width = 0;
    _height = 0;
}

Texture::~Texture()
{
    free();
}

// DEALLOCATION

void Texture::free()
{
    if (_texture != NULL)
    {
        SDL_DestroyTexture(_texture);
        _texture = NULL;
        _width = 0;
        _height = 0;
    }
}

// LOADING

bool Texture::loadFromFile(SDL_Renderer *renderer, std::string path)
{
    free();
    SDL_Texture *newTexture = NULL;
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());

    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s", path.c_str(), IMG_GetError());
        return false;
    }

    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

    if (newTexture == NULL)
    {
        printf("Unable to create texture from %s! SDL Error: %s", path.c_str(), SDL_GetError());
        return false;
    }

    _width = loadedSurface->w;
    _height = loadedSurface->h;

    SDL_FreeSurface(loadedSurface);

    _texture = newTexture;
    return _texture != NULL;
}


// RENDERING

void Texture::render(SDL_Renderer *renderer, int x = 0, int y = 0, SDL_Rect *clip = NULL)
{
    SDL_Rect renderQuad = {x, y, _width, _height};

    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopy(renderer, _texture, clip, &renderQuad);
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    SDL_SetTextureColorMod(_texture, red, green, blue);
}

void Texture::setBlendMode(SDL_BlendMode blending)
{
    SDL_SetTextureBlendMode(_texture, blending);
}

void Texture::setAlpha(Uint8 alpha)
{
    SDL_SetTextureAlphaMod(_texture, alpha);
}

// GETTERS / SETTERS

int Texture::getWidth() { return _width; }

int Texture::getHeight() { return _height; }

void Texture::setWidth(int width) { _width = width; }

void Texture::setHeight(int height) { _height = height; }