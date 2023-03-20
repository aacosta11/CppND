#ifndef Entity_H
#define Entity_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <map>
#include <algorithm>
#include "Texture.h"
#include "Timer.h"

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
    void move(float timeStep, std::vector<SDL_Rect> colliders, std::vector<Entity*> enemies);

    // COLLISION
    enum CollisionDirection { NONE, TOP, BOTTOM, LEFT, RIGHT };
    CollisionDirection getCollisionDirection(SDL_Rect &rect);

    // STATES
    enum EntityState { IDLE, DAMAGED_IDLE, WALKING, ATTACKING, TAKING_DAMAGE, DYING };

    // PHYSICS
    void applyGravity(float timeStep);
    void applyFriction(float timeStep);
    void applyVelocity(float timeStep);
    void applyAcceleration(float timeStep);

    // GETTERS
    int getHealth();
    SDL_Rect getCollider();

    // SETTERS
    void takeDamage(int damage);
    void setCollider(SDL_Rect rect);
    void setRelativeColliderPos(SDL_Rect rect);
    void updateCollider();
    void addToSpriteClips(EntityState state, SDL_Rect* clips);
    void setSpriteClips(SDL_Rect* clips);
    void setCurrentFrame(int frame);
    void setCurrentState(EntityState state);

    // GLOBALS
    static float GRAVITY;
    static float FRICTION;

protected:
    // health
    int _health;

    // position
    struct XY { float x, y; } _vel, _acc;
    
    // collision
    SDL_Rect _collider;
    SDL_Rect _relativeColliderPos;

    // texture
    Texture _texture;
    SDL_Rect _spriteRect;
    std::map<EntityState, SDL_Rect*> _allSpriteClips;
    SDL_Rect* _spriteClips;

    // animation
    int _currentFrame;
    EntityState _currentState;
    Timer _animationTimer;

    // states
    bool _isAirborne;
    bool _isAnimating;
    bool _attackHasLanded;
    bool _leftPressed;
    bool _rightPressed;
    bool _upPressed;
    bool _downPressed;
    bool _spacePressed;
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