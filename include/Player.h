#ifndef PLAYER_H
#define PLAYER_H

#include "TexturedElement.h"
#include "Window.h"


class Player : public TexturedElementWithPhysics
{
public:
    // CONSTRUCTORS / DESTRUCTORS
    Player(int posX, int posY);

    // HANDLE EVENTS
    void handleEvent(SDL_Event &e);
    
    // MOVEMENT
    void move(float timeStep, std::vector<SDL_Rect> colliders);

    // CONSTANTS
    static const int PLAYER_VELOCITY = 200;

private:
    bool _isJumping = false;

    bool _leftPressed = false;
    bool _rightPressed = false;
};

#endif
