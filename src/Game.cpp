#include "Game.h"


// CONSTRUCTORS / DESTRUCTORS

Game::Game()
{
    _gWindow = NULL;
    _gRenderer = NULL;
    _player = NULL;
    _backdrop = NULL;
    _tree = NULL;
    _backdropPosition = { 0, 0 };
}

Game::~Game()
{
    if (_gWindow != NULL || _gRenderer != NULL)
        closeSDL();
}

// SDL

bool Game::initSDL()
{
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // set texture filtering to linear
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        printf("Warning: Linear texture filtering not enabled!");

    // create window
    _gWindow = std::make_unique<Window>();
    if (!_gWindow->createWindow("CppND-Capstone", SCREEN_WIDTH, SCREEN_HEIGHT))
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // create renderer
    _gRenderer = std::make_unique<Renderer>();
    if (!_gRenderer->createRenderer(_gWindow->getWindowHandle()))
    {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // initialize renderer color
    SDL_SetRenderDrawColor(_gRenderer->getRendererHandle(), 0xFF, 0xFF, 0xFF, 0xFF);

    // initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
    }

    return true;
}

void Game::closeSDL()
{
    _gWindow.reset();
    _gRenderer.reset();
    _backdrop.reset();
    _player.reset();
    _tree.reset();

    IMG_Quit();
    SDL_Quit();
}

// LOADING

bool Game::loadPlayer()
{

    _player = std::make_unique<PlayableEntity>(100, 150, PLAYER_WIDTH, PLAYER_HEIGHT);
    if (!_player->loadTexture(_gRenderer->getRendererHandle(), "../assets/player-spritesheet.png"))
        return false;

    // set player sprite clips
    std::vector<SDL_Rect> idleClips = {
        { 16, 0, 55, 143 },
        { 80, 0, 55, 143 },
        { 144, 0, 55, 143 },
        { 208, 0, 55, 143 },
    };
    std::vector<SDL_Rect> moveRightClips = {
        { 16, 149, 55, 143 },
        { 80, 149, 52, 143 },
        { 141, 149, 48, 143 },
        { 198, 149, 53, 143 },
        { 265, 149, 57, 143 },
        { 322, 149, 51, 143 },
        { 383, 149, 52, 143 },
        { 446, 149, 66, 143 },
        { 525, 149, 79, 143 },
        { 617, 149, 99, 143 },
        { 729, 149, 78, 143 },
        { 817, 149, 78, 143 },
    }; 
    std::vector<SDL_Rect> moveLeftClips = {
        { 16, 300, 55, 143 },
        { 104, 300, 53, 143 },
        { 199, 300, 50, 143 },
        { 289, 300, 53, 143 },
        { 378, 300, 53, 143 },
        { 473, 300, 53, 143 },
        { 567, 300, 53, 143 },
        { 655, 300, 55, 143 },
        { 742, 300, 51, 143 },
        { 836, 300, 53, 143 },
        { 927, 300, 57, 143 },
        { 1021, 300, 57, 143 },
        { 1112, 300, 52, 143 },
        { 1204, 300, 51, 143 },
    };
    std::vector<SDL_Rect> attackingClips = {
        { 16, 457, 55, 143 },
        { 88, 457, 68, 143 },
        { 166, 457, 93, 143 },
        { 275, 457, 101, 143 },
        { 389, 457, 96, 143 },
    };

    _player->addAnimation(Entity::EntityState::IDLE, idleClips, 0, 500);
    _player->addAnimation(Entity::EntityState::MOVE_RIGHT, moveRightClips, 0, 150);
    _player->addAnimation(Entity::EntityState::MOVE_LEFT, moveLeftClips, 0, 150); 
    _player->addAnimation(Entity::EntityState::ATTACKING, attackingClips, 1, 50);

    return true;
}

bool Game::loadTree()
{
    _tree = std::make_unique<Entity>(350, -200, TREE_WIDTH, TREE_HEIGHT);
    if (!_tree->loadTexture(_gRenderer->getRendererHandle(), "../assets/tree-spritesheet.png"))
        return false;

    // set tree sprite clips
    std::vector<SDL_Rect> treeIdleClips {
        { 16, 0, 501, 500 },
    };

    std::vector<SDL_Rect> treeDamagedIdleClips {
        { 580, 0, 502, 500 },
    };

    std::vector<SDL_Rect> treeUnresponsiveClips {
        { 1184, 280, 299, 264 },           
    };

    std::vector<SDL_Rect> treeTakingDamageClips {
        { 16, 0, 501, 500 },
        { 20, 0, 501, 500 },
        { 12, 0, 501, 500 },
    };

    _tree->addAnimation(Entity::EntityState::IDLE, treeIdleClips, 0, 0); 
    _tree->addAnimation(Entity::EntityState::DAMAGED_IDLE, treeDamagedIdleClips, 0, 0);
    _tree->addAnimation(Entity::EntityState::UNRESPONSIVE, treeUnresponsiveClips, 0, 0);
    _tree->addAnimation(Entity::EntityState::TAKING_DAMAGE, treeTakingDamageClips, 3, 20);

    return true; 
}

bool Game::loadAssets()
{    
    // load backdrop texture
    _backdrop = std::make_unique<Texture>(); 
    if (!_backdrop->loadFromFile(_gRenderer->getRendererHandle(), "../assets/backdrop.png"))
        return false;
    float ratio = static_cast<float>((float)_backdrop->getHeight() / (float)_backdrop->getWidth());
    _backdrop->setHeight(SCREEN_HEIGHT);
    _backdrop->setWidth(SCREEN_HEIGHT / ratio);
    _gWindow->setWindowSize(_backdrop->getWidth(), _backdrop->getHeight());
    
    // load player texture
    if (!loadPlayer())
        return false;
    
    // load tree texture
    if (!loadTree())
        return false;

    return true;
}

// TREE

void Game::updateTreeFrames() 
{
    if (_tree->getCurrentState() == Entity::EntityState::TAKING_DAMAGE)
    {
        return;
    }
    if (_tree->getHealth() > 50)
    {
        _tree->updateCurrentState(Entity::EntityState::IDLE);
        return;
    }
    if (_tree->getHealth() > 0)
    {
        std::vector<SDL_Rect> clips = _tree->getCurrentStateClips();
        clips.push_back( {
            clips[0].x + 10,
            clips[0].y,
            clips[0].w,
            clips[0].h
            } 
        );
        clips.push_back( {
            clips[0].x - 10,
            clips[0].y,
            clips[0].w,
            clips[0].h
            } 
        );
        _tree->addAnimation(Entity::EntityState::TAKING_DAMAGE, clips, 3, 20);
        _tree->updateCurrentState(Entity::EntityState::DAMAGED_IDLE);
        return;
    }



    _tree->updateCurrentState(Entity::EntityState::UNRESPONSIVE);
}

void Game::handleWindowResize()
{
    int relativePlayerX =  _player->getPosX() - _backdropPosition.x;
    int relativePlayerY =  _player->getPosY() - _backdropPosition.y;
    int relativeTreeX = _tree->getPosX() - _backdropPosition.x;
    int relativeTreeY = _tree->getPosY() - _backdropPosition.y;

    // update backdrop position
    _backdropPosition.x = ( _gWindow->getWidth() / 2 ) - ( _backdrop->getWidth() / 2 );
    _backdropPosition.y = ( _gWindow->getHeight() / 2 ) - ( _backdrop->getHeight() / 2 );

    // update player position
    int playerX = _backdropPosition.x + relativePlayerX;
    int playerY = _backdropPosition.y + relativePlayerY;
    _player->setPosition(playerX, playerY);
    
    // update tree position
    int treeX = _backdropPosition.x + relativeTreeX;
    int treeY = _backdropPosition.y + relativeTreeY;
    _tree->setPosition(treeX, treeY);

    // update world boundaries
    _floor = { _backdropPosition.x, _backdropPosition.y + _backdrop->getHeight() - 20, _backdrop->getWidth(), 40 };
    _rightWall = { _backdropPosition.x + _backdrop->getWidth() - 20, _backdropPosition.y, 20, _backdrop->getHeight() };
    _leftWall = { _backdropPosition.x, _backdropPosition.y, 20, _backdrop->getHeight() };
    _player->setWorldObjects({ _floor, _rightWall, _leftWall });
    _tree->setWorldObjects({ _floor, _rightWall, _leftWall });
}

// MAIN LOOP

void Game::run()
{
    // start up SDL and create window
    if (!initSDL())
    {
        printf("Failed to initialize!\n");
        throw std::runtime_error("Failed to initialize!");
        return;
    }

    // load player texture
    if (!loadAssets())
    {
        printf("Failed to load game assets!\n");
        throw std::runtime_error("Failed to load game assets!");
        return;
    }

    bool quit = false;
    SDL_Event e;

    Timer stepTimer;
    Timer capTimer;
    Timer fpsTimer;

    int countedFrames = 0;
    fpsTimer.start();

    // world boundaries
    _floor = { 0, _gWindow->getHeight() - 20, _gWindow->getWidth(), 40 };
    _rightWall = { _gWindow->getWidth() - 20, 0, 20, _gWindow->getHeight() - 20 };
    _leftWall = { 0, 0, 20, _gWindow->getHeight() - 20 };

    _player->updateCurrentState(Entity::EntityState::IDLE);
    _tree->updateCurrentState(Entity::EntityState::IDLE);

    // set world objects    
    _player->setWorldObjects( { _floor, _rightWall, _leftWall, } );
    _tree->setWorldObjects( { _floor, _rightWall, _leftWall, } );

    // set enemies for player
    _player->setEnemies( { _tree.get() } );

    // main loop
    while (!quit)
    {
        capTimer.start(); // fps cap timer

        bool hasWindowResized = false;
        // handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            // user requests quit
            if (e.type == SDL_QUIT)
                quit = true;
            
            // handle window events
            _gWindow->handleEvent(e, _gRenderer->getRendererHandle(), hasWindowResized);
            _player->handleEvent(e);
        }

        // handle window resize
        if (hasWindowResized)
            handleWindowResize();

        // calculate fps
        float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
        if (avgFPS > 2000000)
            avgFPS = 0;

        // calculate time step
        float timeStep = stepTimer.getTicks() / 1000.f;

        // update positions
        _tree->update(timeStep);
        _player->update(timeStep);

        // update tree frames
        updateTreeFrames();

        // restart step timer
        stepTimer.start();

        // clear screen
        SDL_SetRenderDrawColor(_gRenderer->getRendererHandle(), 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(_gRenderer->getRendererHandle());

        // render textures
        _backdrop->render(_gRenderer->getRendererHandle(), _backdropPosition.x, _backdropPosition.y, NULL);
        _tree->render(_gRenderer->getRendererHandle());
        _player->render(_gRenderer->getRendererHandle());

        // render collision boxes (for debugging)
        // SDL_Rect player = _player->getCollider();
        // SDL_RenderDrawRect(_gRenderer->getRendererHandle(), &player);

        // SDL_Rect tree = _tree->getCollider();
        // SDL_RenderDrawRect(_gRenderer->getRendererHandle(), &tree);

        // SDL_RenderDrawRect(_gRenderer->getRendererHandle(), &_floor);
        // SDL_RenderDrawRect(_gRenderer->getRendererHandle(), &_rightWall);
        // SDL_RenderDrawRect(_gRenderer->getRendererHandle(), &_leftWall);

        // update screen
        SDL_RenderPresent(_gRenderer->getRendererHandle());
        ++countedFrames;

        // if frame finished early, wait remaining time
        int frameTicks = capTimer.getTicks();
        if (frameTicks < SCREEN_TICKS_PER_FRAME)
            SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
    }
}