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
    Entity(int posX, int posY, int width, int height);
    Entity(const Entity &entity);
    Entity(Entity &&entity);
    Entity &operator=(const Entity &entity);
    Entity &operator=(Entity &&entity);
    ~Entity();

    // RENDERING
    void render(SDL_Renderer *renderer);
    bool loadTexture(SDL_Renderer *renderer, std::string path);
    void update(float timeStep);

    // COLLISION
    enum CollisionDirection { NONE, TOP, BOTTOM, LEFT,  RIGHT };
    CollisionDirection getCollisionDirection(SDL_Rect &rect);

    // STATES
    enum EntityState { IDLE, DAMAGED_IDLE, MOVE_RIGHT, MOVE_LEFT, ATTACKING, TAKING_DAMAGE, UNRESPONSIVE };
    void updateCurrentState(EntityState state);   
    
    // PHYSICS
    void applyGravity(float timeStep);
    void applyFriction(float timeStep);
    void applyVelocity(float timeStep);
    void applyAcceleration(float timeStep);

    // GETTERS
    int getHealth();
    EntityState getCurrentState();
    std::vector<SDL_Rect> getCurrentStateClips();
    SDL_Rect getCurrentClip();
    int getWidth();
    int getHeight();
    int getPosX();
    int getPosY();
    SDL_Rect getCollider();

    // SETTERS
    void setPosition(int posX, int posY);
    void setHealth(int health);
    void resetToPosition(int posX, int posY);
    void takeDamage(int damage);
    void setWorldObjects(std::vector<SDL_Rect> worldObjects);
    void setEnemies(std::vector<Entity*> enemies);
    void setCurrentState(EntityState state);
    void addAnimation(EntityState state, std::vector<SDL_Rect> clips, int cycles, int timeBetweenFrames);

    // GLOBALS
    static float GRAVITY;
    static float FRICTION;

protected:
    // health
    int _health;
    // world objects
    std::vector<SDL_Rect> _worldObjects;
    // enemies
    std::vector<Entity*> _enemies;
    // physics
    struct XY { float x, y; } _vel, _acc;
    // collision
    SDL_Rect _collider;
    // texture
    Texture _texture;
    SDL_Rect _spriteRect;
    // animation
    struct Animation { std::vector<SDL_Rect> clips; int cycles; int timeBetweenFrames; };
    std::map<EntityState, Animation> _animations;
    EntityState _currentState;
    Timer _animationTimer;
    int _currentFrame;
    int _currentCycle;
    // states
    bool _isAirborne;
    bool _attackHasLanded;
    bool _leftKeyPressed;
    bool _rightKeyPressed;
};

/* -------------------------------------------------------------------------- */
/* ----------------------------- PLAYABLE ENTITY ---------------------------- */
/* -------------------------------------------------------------------------- */

class PlayableEntity : public Entity
{
public:
    // CONSTRUCTORS / DESTRUCTORS
    PlayableEntity(int posX, int posY, int width, int height);

    // HANDLE EVENTS
    void handleEvent(SDL_Event &e);

    // CONSTANTS
    static const int PLAYER_VELOCITY = 100;
};

#endif