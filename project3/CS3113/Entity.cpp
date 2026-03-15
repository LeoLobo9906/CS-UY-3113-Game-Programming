//project3 - Lunar Lander
#include "Entity.h"

// ── Default constructor ──────────────────────────────────────────────────────
Entity::Entity()
    : mPosition{0.0f, 0.0f}, mMovement{0.0f, 0.0f},
      mVelocity{0.0f, 0.0f}, mAcceleration{0.0f, 0.0f},
      mScale{DEFAULT_SIZE, DEFAULT_SIZE},
      mColliderDimensions{DEFAULT_SIZE, DEFAULT_SIZE},
      mTexture{}, mTextureType{SINGLE}, mAngle{0.0f},
      mSpriteSheetDimensions{}, mDirection{RIGHT},
      mAnimationAtlas{{}}, mAnimationIndices{}, mFrameSpeed{0},
      mSpeed{DEFAULT_SPEED}, mEntityType{NONE_TYPE}
{}

// ── Single-texture constructor ───────────────────────────────────────────────
Entity::Entity(Vector2 position, Vector2 scale, const char *textureFilepath,
               EntityType entityType)
    : mPosition{position}, mVelocity{0.0f, 0.0f},
      mAcceleration{0.0f, 0.0f}, mScale{scale},
      mMovement{0.0f, 0.0f}, mColliderDimensions{scale},
      mTexture{LoadTexture(textureFilepath)},
      mTextureType{SINGLE}, mDirection{RIGHT},
      mAnimationAtlas{{}}, mAnimationIndices{},
      mFrameSpeed{0}, mSpeed{DEFAULT_SPEED},
      mAngle{0.0f}, mEntityType{entityType}
{}

// ── Atlas constructor ────────────────────────────────────────────────────────
Entity::Entity(Vector2 position, Vector2 scale, const char *textureFilepath,
               TextureType textureType, Vector2 spriteSheetDimensions,
               std::map<Direction, std::vector<int>> animationAtlas,
               EntityType entityType)
    : mPosition{position}, mVelocity{0.0f, 0.0f},
      mAcceleration{0.0f, 0.0f}, mMovement{0.0f, 0.0f},
      mScale{scale}, mColliderDimensions{scale},
      mTexture{LoadTexture(textureFilepath)},
      mTextureType{ATLAS},
      mSpriteSheetDimensions{spriteSheetDimensions},
      mAnimationAtlas{animationAtlas}, mDirection{RIGHT},
      mAnimationIndices{animationAtlas.at(RIGHT)},
      mFrameSpeed{DEFAULT_FRAME_SPEED},
      mAngle{0.0f}, mSpeed{DEFAULT_SPEED},
      mEntityType{entityType}
{}

Entity::~Entity() { UnloadTexture(mTexture); }

// ── isColliding ──────────────────────────────────────────────────────────────
bool Entity::isColliding(Entity *other) const
{
    if (!other->isActive()) return false;

    float xDistance = fabs(mPosition.x - other->getPosition().x) -
        ((mColliderDimensions.x + other->getColliderDimensions().x) / 2.0f);
    float yDistance = fabs(mPosition.y - other->getPosition().y) -
        ((mColliderDimensions.y + other->getColliderDimensions().y) / 2.0f);

    return (xDistance < 0.0f && yDistance < 0.0f);
}

// ── checkCollisionY ──────────────────────────────────────────────────────────
void Entity::checkCollisionY(Entity *collidableEntities, int collisionCheckCount)
{
    for (int i = 0; i < collisionCheckCount; i++)
    {
        Entity *collidableEntity = &collidableEntities[i];

        if (isColliding(collidableEntity))
        {
            float yDistance = fabs(mPosition.y - collidableEntity->mPosition.y);
            float yOverlap  = fabs(yDistance -
                              (mColliderDimensions.y / 2.0f) -
                              (collidableEntity->mColliderDimensions.y / 2.0f));

            if (mVelocity.y > 0)
            {
                mPosition.y       -= yOverlap;
                mVelocity.y        = 0;
                mIsCollidingBottom = true;
            }
            else if (mVelocity.y < 0)
            {
                mPosition.y    += yOverlap;
                mVelocity.y     = 0;
                mIsCollidingTop = true;
            }
        }
    }
}

// ── checkCollisionX ──────────────────────────────────────────────────────────
void Entity::checkCollisionX(Entity *collidableEntities, int collisionCheckCount)
{
    for (int i = 0; i < collisionCheckCount; i++)
    {
        Entity *collidableEntity = &collidableEntities[i];

        if (isColliding(collidableEntity))
        {
            float yDistance = fabs(mPosition.y - collidableEntity->mPosition.y);
            float yOverlap  = fabs(yDistance -
                              (mColliderDimensions.y / 2.0f) -
                              (collidableEntity->mColliderDimensions.y / 2.0f));

            if (yOverlap < Y_COLLISION_THRESHOLD) continue;

            float xDistance = fabs(mPosition.x - collidableEntity->mPosition.x);
            float xOverlap  = fabs(xDistance -
                              (mColliderDimensions.x / 2.0f) -
                              (collidableEntity->mColliderDimensions.x / 2.0f));

            if (mVelocity.x > 0)
            {
                mPosition.x      -= xOverlap;
                mVelocity.x       = 0;
                mIsCollidingRight = true;
            }
            else if (mVelocity.x < 0)
            {
                mPosition.x     += xOverlap;
                mVelocity.x      = 0;
                mIsCollidingLeft = true;
            }
        }
    }
}

// ── animate ──────────────────────────────────────────────────────────────────
void Entity::animate(float deltaTime)
{
    if (mUsePingPong && !mPingPongFrames.empty())
    {
        mAnimationTime += deltaTime;
        float secondsPerFrame = 1.0f / mFrameSpeed;

        if (mAnimationTime >= secondsPerFrame)
        {
            mAnimationTime = 0.0f;
            mPingPongIndex++;
            if (mPingPongIndex >= (int)mPingPongFrames.size())
                mPingPongIndex = 0;
            mCurrentFrameIndex = mPingPongFrames[mPingPongIndex];
        }
        return;
    }

    // Standard atlas animation
    mAnimationIndices = mAnimationAtlas.at(mDirection);
    mAnimationTime += deltaTime;
    float secondsPerFrame = 1.0f / mFrameSpeed;

    if (mAnimationTime >= secondsPerFrame)
    {
        mAnimationTime = 0.0f;
        mCurrentFrameIndex++;
        mCurrentFrameIndex %= mAnimationIndices.size();
    }
}

// ── displayCollider ──────────────────────────────────────────────────────────
void Entity::displayCollider()
{
    Rectangle colliderBox = {
        mPosition.x - mColliderDimensions.x / 2.0f,
        mPosition.y - mColliderDimensions.y / 2.0f,
        mColliderDimensions.x,
        mColliderDimensions.y
    };
    DrawRectangleLines((int)colliderBox.x, (int)colliderBox.y,
                       (int)colliderBox.width, (int)colliderBox.height, GREEN);
}

// ── update ───────────────────────────────────────────────────────────────────
void Entity::update(float deltaTime, Entity *platforms, int platformCount)
{
    if (mEntityStatus == INACTIVE) return;

    resetColliderFlags();

    // Horizontal velocity from movement + acceleration
    mVelocity.x += mAcceleration.x * deltaTime;
    mVelocity.y += mAcceleration.y * deltaTime;

    // Jumping
    if (mIsJumping)
    {
        mIsJumping   = false;
        mVelocity.y -= mJumpingPower;
    }

    // Apply position then check collisions
    mPosition.y += mVelocity.y * deltaTime;
    if (platforms) checkCollisionY(platforms, platformCount);

    mPosition.x += mVelocity.x * deltaTime;
    if (platforms) checkCollisionX(platforms, platformCount);

    // Always animate (bird always flaps)
    if (mTextureType == ATLAS || mUsePingPong)
        animate(deltaTime);
}

// ── render ───────────────────────────────────────────────────────────────────
void Entity::render()
{
    if (mEntityStatus == INACTIVE) return;

    Rectangle textureArea;

    switch (mTextureType)
    {
        case SINGLE:
            textureArea = {
                0.0f, 0.0f,
                (float)mTexture.width,
                (float)mTexture.height
            };
            break;

        case ATLAS:
            textureArea = getUVRectangle(
                &mTexture,
                mCurrentFrameIndex,
                (int)mSpriteSheetDimensions.x,
                (int)mSpriteSheetDimensions.y
            );
            break;

        default: break;
    }

    Rectangle destinationArea = {
        mPosition.x,
        mPosition.y,
        mScale.x,
        mScale.y
    };

    Vector2 originOffset = {
        mScale.x / 2.0f,
        mScale.y / 2.0f
    };

    DrawTexturePro(mTexture, textureArea, destinationArea,
                   originOffset, mAngle, WHITE);

    // displayCollider();  // uncomment to debug
}