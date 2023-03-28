#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Texture
{
public:
	// CONSTRUCTORS / DESTRUCTORS
	Texture();
	Texture(const Texture &texture);
	Texture(Texture &&texture);
	Texture &operator=(const Texture &texture);
	Texture &operator=(Texture &&texture);
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

	int _INSTANCE_COUNT = 0;
};

#endif