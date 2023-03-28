/* -------------------------------------------------------------------------- */
/* --------------------------------- ENTITY --------------------------------- */
/* -------------------------------------------------------------------------- */
#include "Entity.h"
#include <iterator>

// CONSTRUCTORS / DESTRUCTORS

Entity::Entity() : Entity(0, 0, 0, 0) {}

Entity::Entity(int posX, int posY, int width, int height) : _collider({ posX, posY, width, height}), _spriteRect({posX, posY, 0, 0})
{
    // health
    _health = 100;
    // world objects
    _worldObjects = std::vector<SDL_Rect>();
    // enemies
    _enemies = std::vector<Entity*>();
    // position
    _vel = {0, 0};
    _acc = {0, 0};
    // texture
    _texture = Texture();
    // animation
    _animations = std::map<EntityState, Animation>();
    _currentState = EntityState::IDLE;
    _animationTimer = Timer();
    _currentFrame = 0;
    _currentCycle = 0;
    // states
    _isAirborne = false;
    _attackHasLanded = false;
    _leftKeyPressed = false;
    _rightKeyPressed = false;
}

Entity::Entity(const Entity &entity)
{
    _health = 100;
    _worldObjects = entity._worldObjects;
    _enemies = entity._enemies;
    _vel = { 0, 0 };
    _acc = { 0, 0 };
    _collider = entity._collider;
    _texture = entity._texture;
    _spriteRect = entity._spriteRect;
    _animations = entity._animations;
    _currentState = EntityState::IDLE;
    _animationTimer = entity._animationTimer;
    _currentFrame = 0;
    _currentCycle = 0;
    _isAirborne = false;
    _attackHasLanded = false;
    _leftKeyPressed = false;
    _rightKeyPressed = false;
}

Entity::Entity(Entity &&entity)
{
    _health = entity._health;
    _worldObjects = entity._worldObjects;
    _enemies = entity._enemies;
    _vel = entity._vel;
    _acc = entity._acc;
    _collider = entity._collider;
    _texture = std::move(entity._texture);
    _spriteRect = entity._spriteRect;
    _animations = entity._animations;
    _currentState = entity._currentState;
    _animationTimer = entity._animationTimer;
    _currentFrame = entity._currentFrame;
    _currentCycle = entity._currentCycle;
    _isAirborne = entity._isAirborne;
    _attackHasLanded = entity._attackHasLanded;
    _leftKeyPressed = entity._leftKeyPressed;
    _rightKeyPressed = entity._rightKeyPressed;

    entity._health = 0;
    entity._worldObjects = std::vector<SDL_Rect>();
    entity._enemies = std::vector<Entity*>();
    entity._vel = { 0, 0 };
    entity._acc = { 0, 0 };
    entity._collider = { 0, 0, 0, 0 };
    entity._spriteRect = { 0, 0, 0, 0 };
    entity._currentState = EntityState::IDLE;
    entity._animationTimer = Timer();
    entity._currentFrame = 0;
    entity._currentCycle = 0;
    entity._isAirborne = false;
    entity._attackHasLanded = false;
    entity._leftKeyPressed = false;
    entity._rightKeyPressed = false;
}

Entity& Entity::operator=(const Entity &entity)
{
    _health = 100;
    _worldObjects = entity._worldObjects;
    _enemies = entity._enemies;
    _vel = { 0, 0 };
    _acc = { 0, 0 };
    _collider = entity._collider;
    _texture = entity._texture;
    _spriteRect = entity._spriteRect;
    _animations = entity._animations;
    _currentState = EntityState::IDLE;
    _animationTimer = entity._animationTimer;
    _currentFrame = 0;
    _currentCycle = 0;
    _isAirborne = false;
    _attackHasLanded = false;
    _leftKeyPressed = false;
    _rightKeyPressed = false;
    return *this;
}

Entity& Entity::operator=(Entity &&entity)
{
    _health = entity._health;
    _worldObjects = entity._worldObjects;
    _enemies = entity._enemies;
    _vel = entity._vel;
    _acc = entity._acc;
    _collider = entity._collider;
    _texture = std::move(entity._texture);
    _spriteRect = entity._spriteRect;
    _animations = entity._animations;
    _currentState = entity._currentState;
    _animationTimer = entity._animationTimer;
    _currentFrame = entity._currentFrame;
    _currentCycle = entity._currentCycle;
    _isAirborne = entity._isAirborne;
    _attackHasLanded = entity._attackHasLanded;
    _leftKeyPressed = entity._leftKeyPressed;
    _rightKeyPressed = entity._rightKeyPressed;

    entity._health = 0;
    entity._worldObjects = std::vector<SDL_Rect>();
    entity._enemies = std::vector<Entity*>();
    entity._vel = { 0, 0 };
    entity._acc = { 0, 0 };
    entity._collider = { 0, 0, 0, 0 };
    entity._spriteRect = { 0, 0, 0, 0 };
    entity._currentState = EntityState::IDLE;
    entity._animationTimer = Timer();
    entity._currentFrame = 0;
    entity._currentCycle = 0;
    entity._isAirborne = false;
    entity._attackHasLanded = false;
    entity._leftKeyPressed = false;
    entity._rightKeyPressed = false;
    return *this;
}

Entity::~Entity() { }

// RENDERING

void Entity::render(SDL_Renderer *renderer)
{
    if (_animations.empty() || _animations.find(_currentState) == _animations.end())
    {
        _texture.render(renderer, _spriteRect.x, _spriteRect.y, NULL);
        return;
    }

    SDL_Rect clip = _animations[_currentState].clips[_currentFrame];
    _spriteRect.x = (_collider.x + _collider.w / 2) - (clip.w / 2);
    _spriteRect.y = (_collider.y + _collider.h) - clip.h;
    _spriteRect.w = clip.w;
    _spriteRect.h = clip.h;
    _texture.render(renderer, _spriteRect.x, _spriteRect.y, &clip);
}

bool Entity::loadTexture(SDL_Renderer *renderer, std::string path)
{
    _texture.loadFromFile(renderer, path);
    _spriteRect.w = _texture.getWidth();
    _spriteRect.h = _texture.getHeight();
    return true;
}

void Entity::update(float timeStep)
{
    // apply physics
    applyGravity(timeStep);
    applyFriction(timeStep);
    applyVelocity(timeStep);
    applyAcceleration(timeStep);

    // Check for collisions with world objects
    for (SDL_Rect &collider : _worldObjects)
    {
        CollisionDirection dir = getCollisionDirection(collider);
        switch (dir)
        {
        case CollisionDirection::NONE:
            break;
        case CollisionDirection::TOP:
            _collider.y = collider.y + collider.h;
            _vel.y = 0;
            break;
        case CollisionDirection::BOTTOM:
            _collider.y = collider.y - _collider.h;
            _isAirborne = false;
            _vel.y = 0;
            break;
        case CollisionDirection::LEFT:
            _collider.x = collider.x + collider.w;
            _vel.x = 0;
            break;
        case CollisionDirection::RIGHT:
            _collider.x = collider.x - _collider.w;
            _vel.x = 0;
            break;
        default:
            break;
        }
    }

    // Check for enemy collisions
    if ( _currentState == EntityState::ATTACKING && !_attackHasLanded )
    {
        for (Entity *enemy : _enemies)
        {
            SDL_Rect enemyCollider = enemy->getCollider();
            CollisionDirection dir = getCollisionDirection(enemyCollider);
            switch (dir)
            {
            case CollisionDirection::RIGHT:
                enemy->takeDamage(10);
                _attackHasLanded = true;
                break;
            default:
                break;
            }
        }
    }

    // update current frame (if time has passed)
    if (_animationTimer.getTicks() > _animations[_currentState].timeBetweenFrames && _animations[_currentState].timeBetweenFrames > 0)
    {
        _currentFrame ++;
        // if current frame is out of bounds, reset to 0
        if (_currentFrame >= _animations[_currentState].clips.size())
        {
            _currentFrame = 0;
            _currentCycle ++;
            // if number of cycles is reached, reset to 0
            if (_currentCycle >= _animations[_currentState].cycles && _animations[_currentState].cycles > 0)
            {
                _currentCycle = 0;

                // if no key is pressed, set state to idle
                if ( _leftKeyPressed && !_rightKeyPressed )
                    _currentState = EntityState::MOVE_LEFT;
                else if ( _rightKeyPressed && !_leftKeyPressed )
                    _currentState = EntityState::MOVE_RIGHT;
                else
                    _currentState = EntityState::IDLE;
            }
        }
        // reset timer
        _animationTimer.start();
    }
}

// COLLISION
// returns enum CollisionDirection { NONE, TOP, BOTTOM, LEFT, RIGHT };
Entity::CollisionDirection Entity::getCollisionDirection(SDL_Rect &rect)
{
    SDL_Rect intersection;
    SDL_IntersectRect(&_collider, &rect, &intersection);
    if (SDL_IntersectRect(&_collider, &rect, &intersection) == SDL_FALSE)
        return CollisionDirection::NONE;

    int LEFT = _collider.x;
    int RIGHT = _collider.x + _collider.w;
    int TOP = _collider.y;
    int BOTTOM = _collider.y + _collider.h;

    int iLEFT = intersection.x;
    int iRIGHT = intersection.x + intersection.w;
    int iTOP = intersection.y;
    int iBOTTOM = intersection.y + intersection.h;

    int distanceToTop = abs(TOP - iTOP);
    int distanceToBottom = abs(BOTTOM - iBOTTOM);
    int distanceToLeft = abs(LEFT - iLEFT);
    int distanceToRight = abs(RIGHT - iRIGHT);

    int minDistance = std::min({distanceToTop, distanceToBottom, distanceToLeft, distanceToRight});

    if (intersection.w > intersection.h)
    {
        if (minDistance == distanceToTop)
            return CollisionDirection::TOP;
        else if (minDistance == distanceToBottom)
            return CollisionDirection::BOTTOM;
    }
    else
    {
        if (minDistance == distanceToLeft)
            return CollisionDirection::LEFT;
        else if (minDistance == distanceToRight)
            return CollisionDirection::RIGHT;
    }

    return CollisionDirection::NONE;
}

// STATES

void Entity::updateCurrentState(Entity::EntityState state)
{
    _currentState = state;
    _currentFrame = 0;
    _currentCycle = 0;
    _attackHasLanded = false;
    _animationTimer.start();
}


// PHYSICS

void Entity::applyGravity(float timeStep)
{
    _acc.y = GRAVITY;
}

void Entity::applyFriction(float timeStep)
{
    _acc.x = -_vel.x * FRICTION * timeStep;
}

void Entity::applyVelocity(float timeStep)
{
    _collider.x += _vel.x * timeStep;
    _collider.y += _vel.y * timeStep;
}

void Entity::applyAcceleration(float timeStep)
{
    _vel.x += _acc.x * timeStep;
    _vel.y += _acc.y * timeStep;
}

// GETTERS

int Entity::getHealth() { return _health; }

Entity::EntityState Entity::getCurrentState() { return _currentState; }

std::vector<SDL_Rect> Entity::getCurrentStateClips() { return _animations[_currentState].clips; }

int Entity::getPosX() { return _collider.x; }

int Entity::getPosY() { return _collider.y; }

SDL_Rect Entity::getCollider() { return _collider; }

// SETTERS

void Entity::setPosition(int posX, int posY)
{
    _collider.x = posX;
    _collider.y = posY;
}

void Entity::takeDamage(int damage)
{
    if (_currentState == EntityState::UNRESPONSIVE)
        return;
    updateCurrentState(EntityState::TAKING_DAMAGE);
    _health -= damage;
    if (_health < 0)
        _health = 0;
}

void Entity::setWorldObjects(std::vector<SDL_Rect> worldObjects) { _worldObjects = worldObjects; }

void Entity::setEnemies(std::vector<Entity *> enemies) { _enemies = enemies; }

void Entity::setCurrentState(Entity::EntityState state) { _currentState = state; }

void Entity::addAnimation(Entity::EntityState state, std::vector<SDL_Rect> clips, int cycles, int timeBetweenFrames)
{
    _animations[state] = { clips, cycles, timeBetweenFrames };
}

// GLOBALS

float Entity::GRAVITY = 9.8f * 250.0f;

float Entity::FRICTION = 0.1f;






/* -------------------------------------------------------------------------- */
/* ----------------------------- PLAYABLE ENTITY ---------------------------- */
/* -------------------------------------------------------------------------- */

PlayableEntity::PlayableEntity(int posX, int posY, int width, int height) : Entity(posX, posY, width, height) {}

void PlayableEntity::handleEvent(SDL_Event &e)
{
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_r:
            _collider.x = 100;
            _collider.y = 100;
            _vel.x = 0;
            _vel.y = 0;
            break;
        case SDLK_UP:
            if (_isAirborne)
                break;
            _vel.y = -800;
            _isAirborne = true;
            break;
        case SDLK_DOWN:
            break;
        case SDLK_LEFT:
            _leftKeyPressed = true;
            _vel.x = -PLAYER_VELOCITY / 3;
            updateCurrentState(EntityState::MOVE_LEFT);
            break;
        case SDLK_RIGHT:
            _rightKeyPressed = true;
            _vel.x = PLAYER_VELOCITY;
            updateCurrentState(EntityState::MOVE_RIGHT);
            break;
        case SDLK_SPACE:
            updateCurrentState(EntityState::ATTACKING);
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
            _leftKeyPressed = false;
            if (!_rightKeyPressed)
            {
                _vel.x = 0;
                updateCurrentState(EntityState::IDLE);
            }
            break;
        case SDLK_RIGHT:
            _rightKeyPressed = false;
            if (!_leftKeyPressed)
            {
                _vel.x = 0;
                updateCurrentState(EntityState::IDLE);
            }
            break;
        }
    }
}