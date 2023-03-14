#ifndef Entity_H
#define Entity_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <map>
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

    // HANDLE EVENTS
    // virtual void handleEvent(SDL_Event &e) = 0;

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
    float getColliderWidth();
    float getColliderHeight();
    float getColliderX();
    float getColliderY();
    SDL_Rect getTextureRect();
    float getTextureWidth();
    float getTextureHeight();
    float getTextureRectWidth();
    float getTextureRectHeight();
    float getTextureRectX();
    float getTextureRectY();


    // SETTERS
    void setTextureRectX(int x);
    void setTextureRectY(int y);
    void setCollider(SDL_Rect rect);
    void updateCollider();

    void setColliderOffset(SDL_Rect rect);
    void setGravity(float gravity);
    void setFriction(float friction);

    // GLOBALS
    static float GRAVITY;
    static float FRICTION;

protected:
    float _health = 100.0f;
    struct XY { float x, y; } _vel, _acc;
    SDL_Rect _textureRect;
    SDL_Rect _collider;
    SDL_Rect _colliderOffset;
    Texture _texture;

    // states
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