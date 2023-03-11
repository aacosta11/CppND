#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <memory>
#include <string>

class Texture
{
public:
	// CONSTRUCTORS / DESTRUCTORS
	Texture();
	~Texture();

	// DEALLOCATION
	void free();
	
	// LOADING
	bool loadFromFile(SDL_Renderer *renderer, std::string path);

	// RENDERING
	void render(SDL_Renderer *renderer, int x, int y, SDL_Rect *clip);
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	void setBlendMode(SDL_BlendMode blending);
	void setAlpha(Uint8 alpha);
	
	// GETTERS / SETTERS
	int getWidth();
	int getHeight();
	void setWidth(int width);
	void setHeight(int height);

private:
	SDL_Texture *_texture;

	int _width;
	int _height;
};

#endif