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
    // position
    _vel = {0, 0};
    _acc = {0, 0};
    // collision
    _collider = {posX, posY, 0, 0};
    _relativeColliderPos = {0, 0, 0, 0};
    _allSpriteClips = std::map<EntityState, SDL_Rect *>();
    _spriteRect = {posX, posY, 0, 0};
    _spriteClips = NULL;
    // animation
    _currentFrame = 0;
    _currentState = EntityState::IDLE;
    _animationTimer = Timer();
    // states
    _isAirborne = false;
    _isAnimating = false;
    _attackHasLanded = false;
    _leftPressed = false;
    _rightPressed = false;
    _upPressed = false;
    _downPressed = false;
}

Entity::~Entity()
{
    if (_spriteClips != NULL)
        delete[] _spriteClips;
}

// RENDERING

void Entity::render(SDL_Renderer *renderer)
{
    if (_allSpriteClips.empty())
    {
        _texture.render(renderer, _spriteRect.x, _spriteRect.y, NULL);
        return;
    }

    SDL_Rect clip = _allSpriteClips[_currentState][_currentFrame];

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

// MOVEMENT

void Entity::move(float timeStep, std::vector<SDL_Rect> colliders, std::vector<Entity *> enemies)
{
    applyGravity(timeStep);
    applyFriction(timeStep);
    applyVelocity(timeStep);
    applyAcceleration(timeStep);

    updateCollider();

    // Check for collisions
    for (SDL_Rect &collider : colliders)
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
        for (Entity *enemy : enemies)
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

    if ( _animationTimer.getTicks() > 100 )
    {
        _isAnimating = false;
        _currentState = EntityState::IDLE;
        _attackHasLanded = false;
        _animationTimer.stop();
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

void Entity::setCollider(SDL_Rect rect) { _collider = rect; }

void Entity::setRelativeColliderPos(SDL_Rect rect) { _relativeColliderPos = rect; }

void Entity::updateCollider()
{
    _collider.x = _spriteRect.x + _relativeColliderPos.x;
    _collider.y = _spriteRect.y + _relativeColliderPos.y;
    _collider.w = _spriteRect.w + _relativeColliderPos.w;
    _collider.h = _spriteRect.h + _relativeColliderPos.h;
}

void Entity::addToSpriteClips(Entity::EntityState state, SDL_Rect *clips)
{
    _allSpriteClips[state] = clips;
}

void Entity::setSpriteClips(SDL_Rect *clips)
{
    if (_spriteClips != NULL)
        delete[] _spriteClips;
    _spriteClips = clips;
}

void Entity::setCurrentFrame(int frame) { _currentFrame = frame; }

void Entity::setCurrentState(Entity::EntityState state) { _currentState = state; }

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
            _upPressed = true;
            if (_isAirborne)
                break;
            _vel.y = -800;
            _isAirborne = true;
            break;
        case SDLK_DOWN:
            _downPressed = true;
            break;
        case SDLK_LEFT:
            _leftPressed = true;
            _vel.x = -PLAYER_VELOCITY / 1.5;
            break;
        case SDLK_RIGHT:
            _rightPressed = true;
            _vel.x = PLAYER_VELOCITY;
            break;
        case SDLK_SPACE:
            _spacePressed = true;
            if (!_isAnimating)
            {
                _currentState = EntityState::ATTACKING;
                _animationTimer.start();
                _isAnimating = true;
            }
            break;
        }
    }
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_UP:
            _upPressed = false;
            break;
        case SDLK_DOWN:
            _downPressed = false;
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
        case SDLK_SPACE:
            _spacePressed = false;
            break;
        }
    }
}