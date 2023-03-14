/* -------------------------------------------------------------------------- */
/* --------------------------------- ENTITY --------------------------------- */
/* -------------------------------------------------------------------------- */
#include "Entity.h"

// CONSTRUCTORS / DESTRUCTORS

Entity::Entity() : Entity(0, 0) { }

Entity::Entity(int posX = 0, int posY = 0)
{
    _collider = {posX, posY, 0, 0};
    _colliderOffset = {0, 0, 0, 0};
    _textureRect = {posX, posY, 0, 0};
    _vel = {0, 0};
    _acc = {0, 0};
}

Entity::~Entity() { }

// RENDERING

void Entity::render(SDL_Renderer *renderer)
{
    _texture.render(renderer, _textureRect.x, _textureRect.y, NULL);
}

bool Entity::loadTexture(SDL_Renderer *renderer, std::string path)
{
    _texture.loadFromFile(renderer, path);
    _textureRect.w = _texture.getWidth();
    _textureRect.h = _texture.getHeight();
    return true;
}

// MOVEMENT

void Entity::move(float timeStep, std::vector<SDL_Rect> colliders)
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
            case CollisionDirection::TOP:
                _textureRect.y = collider.y + collider.h - _colliderOffset.y;
                _vel.y = 0;
                break;
            case CollisionDirection::BOTTOM:
                _textureRect.y = collider.y - _textureRect.h - _colliderOffset.y;
                _isAirborne = false;
                _vel.y = 0;
                break;
            case CollisionDirection::LEFT:
                _textureRect.x = collider.x + collider.w - _colliderOffset.x;
                _vel.x = 0;
                break;
            case CollisionDirection::RIGHT:
                _textureRect.x = collider.x - _textureRect.w + _colliderOffset.x;
                _vel.x = 0;
                break;
            default:
                break;
        }
    }

    updateCollider();
}

// COLLISION
// enum CollisionDirection { NONE, TOP, BOTTOM, LEFT, RIGHT };
Entity::CollisionDirection Entity::getCollisionDirection(SDL_Rect &rect)
{
    if (_collider.y + _collider.h < rect.y || _collider.y > rect.y + rect.h || _collider.x + _collider.w < rect.x || _collider.x > rect.x + rect.w)
        return CollisionDirection::NONE;

    int w = 0.5 * (_collider.w + rect.w);
    int h = 0.5 * (_collider.h + rect.h);
    int dx = _collider.x - rect.x + (0.5 * _collider.w - 0.5 * rect.w);
    int dy = _collider.y - rect.y + (0.5 * _collider.h - 0.5 * rect.h);

    if (abs(dx) <= w && abs(dy) <= h)
    {
        int wy = w * dy;
        int hx = h * dx;

        if (wy > hx)
        {
            if (wy > -hx)
                return CollisionDirection::TOP;
            else
                return CollisionDirection::RIGHT;
        }
        else
        {
            if (wy > -hx)
                return CollisionDirection::LEFT;
            else
                return CollisionDirection::BOTTOM;
        }
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
    _textureRect.x += _vel.x * timeStep;
    _textureRect.y += _vel.y * timeStep;
}

void Entity::applyAcceleration(float timeStep)
{
    _vel.x += _acc.x * timeStep;
    _vel.y += _acc.y * timeStep;
}

// GETTERS

SDL_Rect Entity::getCollider() { return _collider; }

float Entity::getColliderWidth() { return _collider.w; }

float Entity::getColliderHeight() { return _collider.h; }

float Entity::getColliderX() { return _collider.x; }

float Entity::getColliderY() { return _collider.y; }

SDL_Rect Entity::getTextureRect() { return _textureRect; }

float Entity::getTextureRectWidth() { return _textureRect.w; }

float Entity::getTextureRectHeight() { return _textureRect.h; }

float Entity::getTextureRectX() { return _textureRect.x; }

float Entity::getTextureRectY() { return _textureRect.y; }

float Entity::getTextureWidth() { return _texture.getWidth(); }

float Entity::getTextureHeight() { return _texture.getHeight(); }

// SETTERS

void Entity::setTextureRectX(int x) { _textureRect.x = x; }

void Entity::setTextureRectY(int y) { _textureRect.y = y; }

void Entity::setCollider(SDL_Rect rect) { _collider = rect; }

void Entity::updateCollider()
{
    _collider.x = _textureRect.x + _colliderOffset.x;
    _collider.y = _textureRect.y + _colliderOffset.y;
    _collider.w = _textureRect.w + _colliderOffset.w;
    _collider.h = _textureRect.h + _colliderOffset.h;
}

void Entity::setColliderOffset(SDL_Rect rect) { _colliderOffset = rect; }

void Entity::setGravity(float gravity) { GRAVITY = gravity; }

void Entity::setFriction(float friction) { FRICTION = friction; }

// GLOBALS

float Entity::GRAVITY = 9.8f * 250.0f;

float Entity::FRICTION = 0.1f;

/* -------------------------------------------------------------------------- */
/* ----------------------------- PLAYABLE ENTITY ---------------------------- */
/* -------------------------------------------------------------------------- */

PlayableEntity::PlayableEntity(int posX, int posY) : Entity(posX, posY) { }

void PlayableEntity::handleEvent(SDL_Event &e)
{
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_SPACE:
            _textureRect.x = 100;
            _textureRect.y = 100;
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
        }
    }
}