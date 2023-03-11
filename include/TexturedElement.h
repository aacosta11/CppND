/* -------------------------------------------------------------------------- */
/* -------------------------- CLASS DECLARATION FOR ------------------------- */
/* ---------------------------- TEXTURED ELEMENTS --------------------------- */
/* -------------------------------------------------------------------------- */

#ifndef TEXTUREDELEMENT_H
#define TEXTUREDELEMENT_H

#include <SDL2/SDL.h>
#include <vector>
#include "Texture.h"

class TexturedElement
{
public:
    // CONSTRUCTORS / DESTRUCTORS
    TexturedElement();
    TexturedElement(int posX, int posY);
    ~TexturedElement();

    // RENDERING
    void render(SDL_Renderer *renderer);
    bool loadTexture(SDL_Renderer *renderer, std::string path);

    // GETTERS
    int getPosX();
    int getPosY();
    int getWidth();
    int getHeight();

    // SETTERS
    void setPosX(int x);
    void setPosY(int y);
    void setDimensions(int w, int h);

protected:
    struct Coordinates { float x, y; } _pos;
    struct Dimensions { int w, h; } _dim;

    std::unique_ptr<Texture> _texture;
};

#endif

/* -------------------------------------------------------------------------- */
/* -------------------------- CLASS DECLARATION FOR ------------------------- */
/* --------------------- TEXTURED ELEMENTS WITH PHYSICS --------------------- */
/* -------------------------------------------------------------------------- */

#ifndef TEXTUREDELEMENTWITHPHYSICS_H
#define TEXTUREDELEMENTWITHPHYSICS_H

class TexturedElementWithPhysics : public TexturedElement
{
public:
    // CONSTRUCTORS / DESTRUCTORS
    TexturedElementWithPhysics();
    TexturedElementWithPhysics(int posX, int posY);
    ~TexturedElementWithPhysics();

    // LOADING
    bool loadTexture(SDL_Renderer *renderer, std::string path);

    // COLLISION
    enum CollisionDirection { NONE, TOP, BOTTOM, LEFT, RIGHT };
    bool isCollidingWith(SDL_Rect &rect);
    CollisionDirection checkCollision(SDL_Rect &rect);

    // MOVEMENT
    void move(float timeStep, std::vector<SDL_Rect> colliders);

    // PHYSICS
    void applyGravity(float timeStep);
    void applyFriction(float timeStep);
    void applyVelocity(float timeStep);
    void applyAcceleration(float timeStep);

    // GETTERS
    float getVelX();
    float getVelY();
    float getAccX();
    float getAccY();
    SDL_Rect getCollider();

    // SETTERS
    void setVelX(float x);
    void setVelY(float y);
    void setAccX(float x);
    void setAccY(float y);
    void setCollider(SDL_Rect collider);
    void setCollider(int x, int y, int w, int h);
    void setColliderUpdateModifiers(int x, int y, int w, int h);
    void updateCollider();

    // CONSTANTS
    static constexpr float GRAVITY = 9.8f * 250.0f;
    static constexpr float FRICTION = 0.1f;

protected:
    struct XY { float x, y; } _vel, _acc;
    struct Rect { int x, y, w, h; } _colliderUpdateModifiers;
    SDL_Rect _collider;
};

#endif