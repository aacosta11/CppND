#include "Game.h"


// CONSTRUCTORS / DESTRUCTORS

Game::Game()
{
    _gWindow = NULL;
    _gRenderer = NULL;
    _player = NULL;
    _backdrop = NULL;
    _tree = NULL;
    _startButton = NULL;
    _resetButton = NULL;
    _backdropPosition = { 0, 0 };
    _gameStarted = false;
    _gameEnded = false;
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
    _startButton.reset();
    _resetButton.reset();

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
        { 0, 0, 146, 150 },
        { 146, 0, 146, 150 },
        { 292, 0, 146, 150 },
        { 438, 0, 146, 150 },
    };
    std::vector<SDL_Rect> moveRightClips = {
        { 0, 160, 146, 150 },
        { 146, 160, 146, 150 },
        { 292, 160, 146, 150 },
        { 438, 160, 146, 150 },
        { 584, 160, 146, 150 },
        { 730, 160, 146, 150 },
        { 876, 160, 146, 150 },
        { 1022, 160, 146, 150 },
        { 1168, 160, 146, 150 },
        { 1314, 160, 146, 150 },
        { 1460, 160, 146, 150 },
        { 1606, 160, 146, 150 },
    }; 
    std::vector<SDL_Rect> moveLeftClips = {
        { 0, 320, 146, 150 },
        { 146, 320, 146, 150 },
        { 292, 320, 146, 150 },
        { 438, 320, 146, 150 },
        { 584, 320, 146, 150 },
        { 730, 320, 146, 150 },
        { 876, 320, 146, 150 },
        { 1022, 320, 146, 150 },
        { 1168, 320, 146, 150 },
        { 1314, 320, 146, 150 },
        { 1460, 320, 146, 150 },
        { 1606, 320, 146, 150 },
        { 1752, 320, 146, 150 },
        { 1898, 320, 146, 150 },
    };
    std::vector<SDL_Rect> attackingClips = {
        { 0, 480, 177, 159 },
        { 177, 480, 177, 159 },
        { 354, 480, 177, 159 },
        { 532, 480, 177, 159 },
        { 709, 480, 177, 159 },
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

    // load start button texture
    _startButton = std::make_unique<Button>();
    if (!_startButton->loadFromFile(_gRenderer->getRendererHandle(), "../assets/start-button.png"))
        return false;
    _startButton->setPosition(_backdrop->getWidth() / 2 - _startButton->getWidth() / 2, _backdrop->getHeight() / 2 - _startButton->getHeight() / 2);
    
    // TODO: load reset button texture
    _resetButton = std::make_unique<Button>();
    // if (!_resetButton->loadFromFile(_gRenderer->getRendererHandle(), "../assets/reset-button.png"))
    //     return false;
    _resetButton->setDimensions(100, 50);
    _resetButton->setPosition(_backdrop->getWidth() / 2 - _resetButton->getWidth() / 2, _backdrop->getHeight() / 2 - _resetButton->getHeight() / 2);

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
    if (_tree->getCurrentState() == Entity::EntityState::TAKING_DAMAGE) return;

    SDL_Rect currentClip = _tree->getCurrentClip();
    auto createTakingDamageAnim = [currentClip]()->std::vector<SDL_Rect>{
        std::vector<SDL_Rect> clips {currentClip};
        clips.push_back( {
            currentClip.x + 10,
            currentClip.y,
            currentClip.w,
            currentClip.h
            } 
        );
        clips.push_back( {
            currentClip.x - 10,
            currentClip.y,
            currentClip.w,
            currentClip.h
            } 
        );
        return clips;
    };

    if (_tree->getHealth() > 50)
    {
        if (_tree->getCurrentState() != Entity::EntityState::IDLE)
            _tree->updateCurrentState(Entity::EntityState::IDLE);
        _tree->addAnimation(Entity::EntityState::TAKING_DAMAGE, createTakingDamageAnim(), 3, 20);
        return;
    }
    if (_tree->getHealth() > 0)
    {
        if (_tree->getCurrentState() != Entity::EntityState::DAMAGED_IDLE)
            _tree->updateCurrentState(Entity::EntityState::DAMAGED_IDLE);
        _tree->addAnimation(Entity::EntityState::TAKING_DAMAGE, createTakingDamageAnim(), 3, 20);
        return;
    }

    if (_tree->getCurrentState() != Entity::EntityState::UNRESPONSIVE)
        _tree->updateCurrentState(Entity::EntityState::UNRESPONSIVE);
    _gameEnded = true;
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

    // update start button position
    _startButton->setPosition(_backdropPosition.x + _backdrop->getWidth() / 2 - _startButton->getWidth() / 2, _backdropPosition.y + _backdrop->getHeight() / 2 - _startButton->getHeight() / 2);

    // update reset button position
    _resetButton->setPosition(_backdropPosition.x + _backdrop->getWidth() / 2 - _resetButton->getWidth() / 2, _backdropPosition.y + _backdrop->getHeight() / 2 - _resetButton->getHeight() / 2);
}

void Game::resetGame()
{
    _player->resetToPosition(100, -_player->getHeight() - 150);
    _tree->resetToPosition(_backdrop->getWidth() - _tree->getWidth() - 200, -_tree->getHeight() - 150);
    _gameEnded = false;
}

// MAIN LOOP

void Game::run()
{
    if (!initSDL())
    {
        printf("Failed to initialize!\n");
        throw std::runtime_error("Failed to initialize!");
        return;
    }
    if (!loadAssets())
    {
        printf("Failed to load game assets!\n");
        throw std::runtime_error("Failed to load game assets!");
        return;
    }

    SDL_Event e;

    // start timers
    Timer stepTimer;
    Timer capTimer;
    Timer fpsTimer;

    int countedFrames = 0;
    fpsTimer.start();

    // world boundaries
    _floor = { 0, _gWindow->getHeight() - 20, _gWindow->getWidth(), 40 };
    _rightWall = { _gWindow->getWidth() - 20, 0, 20, _gWindow->getHeight() - 20 };
    _leftWall = { 0, 0, 20, _gWindow->getHeight() - 20 };
    _player->setWorldObjects( { _floor, _rightWall, _leftWall, } );
    _tree->setWorldObjects( { _floor, _rightWall, _leftWall, } );

    // set player and tree to idle
    _player->updateCurrentState(Entity::EntityState::IDLE);
    _tree->updateCurrentState(Entity::EntityState::IDLE);

    // set player enemies
    _player->setEnemies( { _tree.get() } );

    // main loop
    _gameStarted = false;
    _gameEnded = false;
    bool quit = false;
    while (!quit)
    {
        capTimer.start();

        bool hasWindowResized = false;
        bool woodYouLikeToReset = false;

        // handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            // user requests quit
            if (e.type == SDL_QUIT)
                quit = true;
            
            // handle window events
            _gWindow->handleEvent(e, _gRenderer->getRendererHandle(), hasWindowResized);
            _player->handleEvent(e);
            _startButton->handleSDLEvent(e, _gameStarted, !_gameStarted);
            _resetButton->handleSDLEvent(e, woodYouLikeToReset, _gameEnded);
        }

        // handle window resize
        if (hasWindowResized)
            handleWindowResize();
        
        // handle reset
        if (woodYouLikeToReset)
            resetGame();

        // calculate fps
        float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
        if (avgFPS > 2000000)
            avgFPS = 0;

        // clear screen
        SDL_SetRenderDrawColor(_gRenderer->getRendererHandle(), 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(_gRenderer->getRendererHandle());

        _backdrop->render(_gRenderer->getRendererHandle(), _backdropPosition.x, _backdropPosition.y, NULL);
        
        if (_gameStarted)
        {
            // calculate time step
            float timeStep = stepTimer.getTicks() / 1000.f;

            // update positions
            _tree->update(timeStep);
            _player->update(timeStep);

            // update tree frames
            updateTreeFrames();

            // restart step timer
            stepTimer.start();

            // render textures
            _tree->render(_gRenderer->getRendererHandle());
            _player->render(_gRenderer->getRendererHandle());

            if ( _gameEnded )
            {
                _resetButton->render(_gRenderer->getRendererHandle());
                SDL_Rect rect = _resetButton->getRect();                
                SDL_SetRenderDrawColor(_gRenderer->getRendererHandle(), 0x00, 0x00, 0x00, 0x00);
                SDL_RenderFillRect(_gRenderer->getRendererHandle(), &rect);
            }
        }
        else
        {
            // render start button
            _startButton->render(_gRenderer->getRendererHandle());
        }

        // update screen
        SDL_RenderPresent(_gRenderer->getRendererHandle());
        ++countedFrames;

        // if frame finished early, wait remaining time
        int frameTicks = capTimer.getTicks();
        if (frameTicks < SCREEN_TICKS_PER_FRAME)
            SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
    }
}