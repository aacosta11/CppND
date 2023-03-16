#ifndef Entity_H
#define Entity_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include "Texture.h"

class Entity
{
public:
    // CONSTRUCTORS / DESTRUCTORS
    Entity();
    Entity(int posX, int posY);
    ~Entity();

    // RENDERING
    void render(SDL_Renderer *renderer);
    bool loadTexture(SDL_Renderer *renderer, std::string path);

    // MOVEMENT
    void move(float timeStep, std::vector<SDL_Rect> colliders);

    // COLLISION
    enum CollisionDirection { NONE, TOP, BOTTOM, LEFT, RIGHT };
    CollisionDirection getCollisionDirection(SDL_Rect &rect);

    // PHYSICS
    void applyGravity(float timeStep);
    void applyFriction(float timeStep);
    void applyVelocity(float timeStep);
    void applyAcceleration(float timeStep);

    // GETTERS
    SDL_Rect getCollider();
    float getTextureWidth();
    float getTextureHeight();

    // SETTERS
    void setCollider(SDL_Rect rect);
    void updateCollider();
    void setSpriteRectX(int x);
    void setSpriteRectY(int y);
    void setSpriteClips(SDL_Rect clips[]);

    // GLOBALS
    static float GRAVITY;
    static float FRICTION;

protected:
    // float _health = 100.0f;
    
    // position
    struct XY { float x, y; } _vel, _acc;
    
    // collision
    SDL_Rect _collider;
    
    // texture
    Texture _texture;
    SDL_Rect _spriteRect;
    SDL_Rect* _spriteClips;


    // states
    int _animationFrame = 1;
    bool _isAirborne = false;
    bool _leftPressed = false;
    bool _rightPressed = false;
    bool _upPressed = false;
    bool _downPressed = false;
};

/* -------------------------------------------------------------------------- */
/* ----------------------------- PLAYABLE ENTITY ---------------------------- */
/* -------------------------------------------------------------------------- */

class PlayableEntity : public Entity
{
public:
    // CONSTRUCTORS / DESTRUCTORS
    PlayableEntity(int posX, int posY);

    // HANDLE EVENTS
    void handleEvent(SDL_Event &e);

    // CONSTANTS
    static const int PLAYER_VELOCITY = 200;
};

#endif