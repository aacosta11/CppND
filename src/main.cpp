#include <iostream>
#include <stdio.h>
#include <memory>
#include <SDL2/SDL.h>
#include "Game.h"


int main(int argc, char *args[])
{
    Game game;
    try
    {    
        game.run();
    }
    catch (std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}