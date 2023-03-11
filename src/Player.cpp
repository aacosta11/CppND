#include "Player.h"

// CONSTRUCTORS / DESTRUCTORS

Player::Player(int posX = 0, int posY = 0) : TexturedElementWithPhysics(posX, posY) { }

// HANDLE EVENTS

void Player::handleEvent(SDL_Event &e)
{
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_UP:
            if (_isJumping) break;
            _vel.y = -800;
            _isJumping = true;
            break;
        case SDLK_DOWN:
            break;
        case SDLK_LEFT:
            _vel.x = -PLAYER_VELOCITY / 1.5;
            _leftPressed = true;
            break;
        case SDLK_RIGHT:
            _vel.x = PLAYER_VELOCITY;
            _rightPressed = true;
            break;
        }
    }
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_UP:
            break;
        case SDLK_DOWN:
            break;
        case SDLK_LEFT:
            _leftPressed = false;
            if (!_rightPressed)
                _vel.x = 0;
            break;
        case SDLK_RIGHT:
            _rightPressed = false;
            if (!_leftPressed)
                _vel.x = 0;
            break;
        }
    }
}

// MOVEMENT

void Player::move(float timeStep, std::vector<SDL_Rect> colliders)
{
    applyGravity(timeStep);
    applyFriction(timeStep);
    applyVelocity(timeStep);
    applyAcceleration(timeStep);
    updateCollider();

    for (SDL_Rect &collider : colliders)
    {
        CollisionDirection dir = checkCollision(collider);
        switch (dir)
        {
            case CollisionDirection::TOP:
                _pos.y = collider.y + collider.h;
                _vel.y = 0;
                break;
            case CollisionDirection::BOTTOM:
                _vel.y = 0;
                _pos.y = collider.y - _collider.h;
                _isJumping = false;
                break;
            case CollisionDirection::LEFT:
                _pos.x = collider.x + collider.w;
                _vel.x = 0;
                break;
            case CollisionDirection::RIGHT:
                _pos.x = collider.x - _collider.w;
                _vel.x = 0;
                break;
            default:
                break;
        }
    }

    updateCollider();
}
