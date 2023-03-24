#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdexcept>
#include <memory>
#include <vector>
#include <string>

#include "Window.h"
#include "Renderer.h"
#include "Texture.h"
#include "Entity.h"
#include "Timer.h"


class Game
{
public:
    // CONSTRUCTORS / DESTRUCTORS
    Game();
    ~Game();

    // SDL
    bool initSDL();
    void closeSDL();

    // LOADING
    bool loadPlayer();
    bool loadTree();
    bool loadAssets();

    // MAIN LOOP
    void run();
    
    // CONSTANTS
    static const int SCREEN_WIDTH = 640;
    static const int SCREEN_HEIGHT = 480;
    static const int SCREEN_FPS = 60;
    static const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

private:
    std::unique_ptr<Window> _gWindow;
    std::unique_ptr<Renderer> _gRenderer;
    std::unique_ptr<Texture> _backdrop;
    std::unique_ptr<PlayableEntity> _player;
    std::unique_ptr<Entity> _tree;
};

#endif