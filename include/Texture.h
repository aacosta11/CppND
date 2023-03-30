#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

/*
This class originates from Lazy Foo' Productions
(http://lazyfoo.net/)
*/
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