#include "Game.h"

// CONSTRUCTORS / DESTRUCTORS

Game::Game()
{
    _gWindow = NULL;
    _gRenderer = NULL;
    _player = NULL;
    _backdrop = NULL;
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
    _player = std::make_unique<Player>(100, 150);
    if (!_player->loadTexture(_gRenderer->getRendererHandle(), "../assets/player.png"))
        return false;

    // load tree texture
    _tree = std::make_unique<TexturedElementWithPhysics>();
    if (!_tree->loadTexture(_gRenderer->getRendererHandle(), "../assets/innocent_tree.png"))
        return false;
    _tree->setPosX(_gWindow->getWidth() - _tree->getWidth());
    _tree->setPosY(_gWindow->getHeight() - _tree->getHeight() - 150);
    _tree->setColliderUpdateModifiers(150, 0, -250, 0);

    return true;
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
    SDL_Rect floor = { 0, _gWindow->getHeight() - 20, _gWindow->getWidth(), 40 };
    SDL_Rect rightWall = { _gWindow->getWidth() - 20, 0, 20, _gWindow->getHeight() };
    SDL_Rect leftWall = { 0, 0, 20, _gWindow->getHeight() };

    // main loop

    while (!quit)
    {
        capTimer.start(); // fps cap timer

        // handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            // user requests quit
            if (e.type == SDL_QUIT)
                quit = true;
            
            // handle window events
            _gWindow->handleEvent(e, _gRenderer->getRendererHandle());
            _player->handleEvent(e);
        }

        // calculate fps
        float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
        if (avgFPS > 2000000)
            avgFPS = 0;

        // calculate time step
        float timeStep = stepTimer.getTicks() / 1000.f;

        // update positions
        _tree->move(timeStep, { floor, rightWall, leftWall });
        _player->move(timeStep, { floor, rightWall, leftWall, _tree->getCollider() });

        // restart step timer
        stepTimer.start();

        // clear screen
        SDL_SetRenderDrawColor(_gRenderer->getRendererHandle(), 0xFF, 0x00, 0x00, 0xFF);
        SDL_RenderClear(_gRenderer->getRendererHandle());

        // render textures
        _backdrop->render(_gRenderer->getRendererHandle(), 0, 0, NULL);
        _tree->render(_gRenderer->getRendererHandle());
        _player->render(_gRenderer->getRendererHandle());

        // update screen
        SDL_RenderPresent(_gRenderer->getRendererHandle());
        ++countedFrames;

        // if frame finished early, wait remaining time
        int frameTicks = capTimer.getTicks();
        if (frameTicks < SCREEN_TICKS_PER_FRAME)
            SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
    }
}