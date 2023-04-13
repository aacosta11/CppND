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
#include "Button.h"


class Game
{
public:
    // CONSTRUCTORS / DESTRUCTORS
    Game();
    Game(const Game &game) = delete;
    Game(const Game &&game) = delete;
    Game &operator=(const Game &game) = delete;
    Game &operator=(const Game &&game) = delete;
    ~Game();

    // SDL
    bool initSDL();
    void closeSDL();

    // LOADING
    bool loadPlayer();
    bool loadTree();
    bool loadAssets();

    // TREE
    void updateTreeFrames();

    // EVENTS
    void handleWindowResize();
    void resetGame();

    // MAIN LOOP
    void run();
    
    // CONSTANTS
    static constexpr int PLAYER_HEIGHT = 150;
    static constexpr int PLAYER_WIDTH = 60;
    static constexpr int TREE_HEIGHT = 500;
    static constexpr int TREE_WIDTH = 200;
    static const int SCREEN_WIDTH = 640;
    static const int SCREEN_HEIGHT = 480;
    static const int SCREEN_FPS = 60;
    static constexpr int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

private:
    std::unique_ptr<Window> _gWindow;
    std::unique_ptr<Renderer> _gRenderer;
    std::unique_ptr<Texture> _backdrop;
    std::unique_ptr<PlayableEntity> _player;
    std::unique_ptr<Entity> _tree;
    std::unique_ptr<Button> _startButton;
    std::unique_ptr<Button> _resetButton;
    bool _gameStarted, _gameEnded;
    SDL_Rect _floor, _leftWall, _rightWall;

    struct xy { int x; int y; };
    xy  _backdropPosition;
};

#endif