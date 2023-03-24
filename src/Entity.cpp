/* -------------------------------------------------------------------------- */
/* --------------------------------- ENTITY --------------------------------- */
/* -------------------------------------------------------------------------- */
#include "Entity.h"
#include <iterator>

// CONSTRUCTORS / DESTRUCTORS

Entity::Entity() : Entity(0, 0) {}

Entity::Entity(int posX, int posY)
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
    // collision
    _collider = {posX, posY, 0, 0};
    _relativeColliderPos = {0, 0, 0, 0};
    _spriteRect = {posX, posY, 0, 0};
    // animation
    _currentFrame = 0;
    _currentState = EntityState::IDLE;
    _animationTimer = Timer();
    // states
    _isAirborne = false;
    _attackHasLanded = false;
}

Entity::~Entity() { }

// RENDERING

void Entity::render(SDL_Renderer *renderer)
{
    if (_animations.empty())
    {
        _texture.render(renderer, _spriteRect.x, _spriteRect.y, NULL);
        return;
    }

    SDL_Rect clip = _animations[_currentState].clips[_currentFrame];
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

    updateCollider();

    // Check for collisions with world objects
    for (SDL_Rect &collider : _worldObjects)
    {
        CollisionDirection dir = getCollisionDirection(collider);
        switch (dir)
        {
        case CollisionDirection::NONE:
            break;
        case CollisionDirection::TOP:
            _spriteRect.y = collider.y + collider.h;
            _collider.y = _spriteRect.y + _relativeColliderPos.y;
            _vel.y = 0;
            break;
        case CollisionDirection::BOTTOM:
            _spriteRect.y = collider.y - _spriteRect.h;
            _collider.y = _spriteRect.y + _relativeColliderPos.y;
            _isAirborne = false;
            _vel.y = 0;
            break;
        case CollisionDirection::LEFT:
            _spriteRect.x = collider.x + collider.w;
            _collider.x = _spriteRect.x + _relativeColliderPos.x;
            _vel.x = 0;
            break;
        case CollisionDirection::RIGHT:
            _spriteRect.x = collider.x - _spriteRect.w;
            _collider.x = _spriteRect.x + _relativeColliderPos.x;
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

    // update animation
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
    _spriteRect.x += _vel.x * timeStep;
    _spriteRect.y += _vel.y * timeStep;
}

void Entity::applyAcceleration(float timeStep)
{
    _vel.x += _acc.x * timeStep;
    _vel.y += _acc.y * timeStep;
}

// GETTERS

int Entity::getHealth() { return _health; }

SDL_Rect Entity::getCollider() { return _collider; }

// SETTERS

void Entity::takeDamage(int damage)
{
    _health -= damage;
    if (_health < 0)
        _health = 0;
}

void Entity::setWorldObjects(std::vector<SDL_Rect> worldObjects) { _worldObjects = worldObjects; }

void Entity::setEnemies(std::vector<Entity *> enemies) { _enemies = enemies; }

void Entity::updateCollider()
{
    _collider.x = _spriteRect.x + _relativeColliderPos.x;
    _collider.y = _spriteRect.y + _relativeColliderPos.y;
    _collider.w = _spriteRect.w + _relativeColliderPos.w;
    _collider.h = _spriteRect.h + _relativeColliderPos.h;
}

void Entity::setRelativeColliderPos(SDL_Rect rect) { _relativeColliderPos = rect; }

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

PlayableEntity::PlayableEntity(int posX, int posY) : Entity(posX, posY) {}

void PlayableEntity::handleEvent(SDL_Event &e)
{
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_r:
            _spriteRect.x = 100;
            _spriteRect.y = 100;
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