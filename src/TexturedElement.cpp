/* -------------------------------------------------------------------------- */
/* ---------------- IMPLEMENTATION FOR CLASS TexturedElement ---------------- */
/* -------------------------------------------------------------------------- */

#include "TexturedElement.h"

// CONSTRUCTORS / DESTRUCTORS

TexturedElement::TexturedElement() : TexturedElement(0, 0) {}

TexturedElement::TexturedElement(int posX = 0, int posY = 0)
{
    _pos = { (float)posX, (float)posY };
    _texture = std::make_unique<Texture>();
}

TexturedElement::~TexturedElement() { }

// RENDERING

void TexturedElement::render(SDL_Renderer *renderer)
{
    _texture->render(renderer, _pos.x, _pos.y, NULL);
}

bool TexturedElement::loadTexture(SDL_Renderer *renderer, std::string path)
{
    if (!_texture->loadFromFile(renderer, path))
        return false;

    _dim.w = _texture->getWidth();
    _dim.h = _texture->getHeight();

    return true;
}

// GETTERS

int TexturedElement::getPosX() { return _pos.x; }

int TexturedElement::getPosY() { return _pos.y; }

int TexturedElement::getWidth() { return _dim.w; }

int TexturedElement::getHeight() { return _dim.h; }

// SETTERS

void TexturedElement::setPosX(int x) { _pos.x = x; }

void TexturedElement::setPosY(int y) { _pos.y = y; }

void TexturedElement::setDimensions(int w, int h) { _dim.w = w; _dim.h = h; }


/* -------------------------------------------------------------------------- */
/* ----------- IMPLEMENTATION FOR CLASS TexturedElementWithPhysics ---------- */
/* -------------------------------------------------------------------------- */

// CONSTRUCTORS / DESTRUCTORS

TexturedElementWithPhysics::TexturedElementWithPhysics() : TexturedElementWithPhysics(0, 0) {}

TexturedElementWithPhysics::TexturedElementWithPhysics(int posX, int posY) : TexturedElement(posX, posY)
{
    _vel = { 0.0f, 0.0f };
    _acc = { 0.0f, 0.0f };
    _collider = { (int)_pos.x, (int)_pos.y, 0, 0 };
    _colliderUpdateModifiers = { 0, 0, 0, 0 };
}

TexturedElementWithPhysics::~TexturedElementWithPhysics() { }

// LOADING

bool TexturedElementWithPhysics::loadTexture(SDL_Renderer *renderer, std::string path)
{
    if (!TexturedElement::loadTexture(renderer, path))
        return false;

    _collider.w = _dim.w;
    _collider.h = _dim.h;

    return true;
}

// COLLISION

bool TexturedElementWithPhysics::isCollidingWith(SDL_Rect &rect)
{
    return SDL_HasIntersection(&_collider, &rect);
}

TexturedElementWithPhysics::CollisionDirection 
TexturedElementWithPhysics::checkCollision(SDL_Rect &rect)
{
    if (_collider.y + _collider.h < rect.y || _collider.y > rect.y + rect.h || _collider.x + _collider.w < rect.x || _collider.x > rect.x + rect.w )
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

// MOVEMENT

void TexturedElementWithPhysics::move(float timeStep, std::vector<SDL_Rect> colliders)
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

// PHYSICS

void TexturedElementWithPhysics::applyGravity(float timeStep)
{
    _acc.y = GRAVITY;
}

void TexturedElementWithPhysics::applyFriction(float timeStep)
{
    _acc.x = -_vel.x * FRICTION * timeStep;
}

void TexturedElementWithPhysics::applyVelocity(float timeStep)
{
    _pos.x += _vel.x * timeStep;
    _pos.y += _vel.y * timeStep;
}

void TexturedElementWithPhysics::applyAcceleration(float timeStep)
{
    _vel.x += _acc.x * timeStep;
    _vel.y += _acc.y * timeStep;
}

// GETTERS

float TexturedElementWithPhysics::getVelX() { return _vel.x; }

float TexturedElementWithPhysics::getVelY() { return _vel.y; }

float TexturedElementWithPhysics::getAccX() { return _acc.x; }

float TexturedElementWithPhysics::getAccY() { return _acc.y; }

SDL_Rect TexturedElementWithPhysics::getCollider() { return _collider; }

// SETTERS

void TexturedElementWithPhysics::setVelX(float x) { _vel.x = x; }

void TexturedElementWithPhysics::setVelY(float y) { _vel.y = y; }

void TexturedElementWithPhysics::setAccX(float x) { _acc.x = x; }

void TexturedElementWithPhysics::setAccY(float y) { _acc.y = y; }

void TexturedElementWithPhysics::setCollider(SDL_Rect collider) { _collider = collider; }

void TexturedElementWithPhysics::setCollider(int x, int y, int w, int h)
{
    _collider.x = x;
    _collider.y = y;
    _collider.w = w;
    _collider.h = h;
}

void TexturedElementWithPhysics::setColliderUpdateModifiers(int x, int y, int w, int h)
{
    _colliderUpdateModifiers.x = x;
    _colliderUpdateModifiers.y = y;
    _colliderUpdateModifiers.w = w;
    _colliderUpdateModifiers.h = h;
}

void TexturedElementWithPhysics::updateCollider()
{
    _collider.x = _pos.x + _colliderUpdateModifiers.x;
    _collider.y = _pos.y + _colliderUpdateModifiers.y;
    _collider.w = _dim.w + _colliderUpdateModifiers.w;
    _collider.h = _dim.h + _colliderUpdateModifiers.h;
}
