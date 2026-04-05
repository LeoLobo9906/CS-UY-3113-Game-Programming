#include "entity.h"

Entity::Entity()
    : mPosition{0.0f, 0.0f}, mMovement{0.0f, 0.0f},
      mVelocity{0.0f, 0.0f}, mAcceleration{0.0f, 0.0f},
      mScale{DEFAULT_SIZE, DEFAULT_SIZE},
      mColliderDimensions{DEFAULT_SIZE, DEFAULT_SIZE},
      mTexture{}, mTextureType{SINGLE}, mAngle{0.0f},
      mSpriteSheetDimensions{}, mDirection{RIGHT},
      mAnimationAtlas{{}}, mAnimationIndices{}, mFrameSpeed{0},
      mSpeed{DEFAULT_SPEED}, mEntityType{NONE}
{}

Entity::Entity(Vector2 position, Vector2 scale, const char *textureFilepath,
               EntityType entityType)
    : mPosition{position}, mVelocity{0.0f, 0.0f},
      mAcceleration{0.0f, 0.0f}, mScale{scale},
      mMovement{0.0f, 0.0f}, mColliderDimensions{scale},
      mTextureFilepath{textureFilepath},
      mTexture{LoadTexture(textureFilepath)},
      mTextureType{SINGLE}, mDirection{RIGHT},
      mAnimationAtlas{{}}, mAnimationIndices{},
      mFrameSpeed{0}, mSpeed{DEFAULT_SPEED},
      mAngle{0.0f}, mEntityType{entityType}
{}

Entity::Entity(Vector2 position, Vector2 scale, const char *textureFilepath,
               TextureType textureType, Vector2 spriteSheetDimensions,
               std::map<Direction, std::vector<int>> animationAtlas,
               EntityType entityType)
    : mPosition{position}, mVelocity{0.0f, 0.0f},
      mAcceleration{0.0f, 0.0f}, mMovement{0.0f, 0.0f},
      mScale{scale}, mColliderDimensions{scale},
      mTextureFilepath{textureFilepath},
      mTexture{LoadTexture(textureFilepath)},
      mTextureType{ATLAS},
      mSpriteSheetDimensions{spriteSheetDimensions},
      mAnimationAtlas{animationAtlas}, mDirection{RIGHT},
      mAnimationIndices{animationAtlas.at(RIGHT)},
      mFrameSpeed{DEFAULT_FRAME_SPEED},
      mAngle{0.0f}, mSpeed{DEFAULT_SPEED},
      mEntityType{entityType}
{}

Entity::~Entity() { if (mTexture.id != 0) UnloadTexture(mTexture); }

Entity::Entity(const Entity &other)
    : mPosition             {other.mPosition},
      mMovement             {other.mMovement},
      mVelocity             {other.mVelocity},
      mAcceleration         {other.mAcceleration},
      mScale                {other.mScale},
      mColliderDimensions   {other.mColliderDimensions},
      mTextureFilepath      {other.mTextureFilepath},
      mTexture              {other.mTextureFilepath.empty()
                                 ? Texture2D{}
                                 : LoadTexture(other.mTextureFilepath.c_str())},
      mTextureType          {other.mTextureType},
      mSpriteSheetDimensions{other.mSpriteSheetDimensions},
      mAnimationAtlas       {other.mAnimationAtlas},
      mAnimationIndices     {other.mAnimationIndices},
      mDirection            {other.mDirection},
      mFrameSpeed           {other.mFrameSpeed},
      mCurrentFrameIndex    {other.mCurrentFrameIndex},
      mAnimationTime        {other.mAnimationTime},
      mIsJumping            {other.mIsJumping},
      mJumpingPower         {other.mJumpingPower},
      mSpeed                {other.mSpeed},
      mAngle                {other.mAngle},
      mIsCollidingTop       {other.mIsCollidingTop},
      mIsCollidingBottom    {other.mIsCollidingBottom},
      mIsCollidingRight     {other.mIsCollidingRight},
      mIsCollidingLeft      {other.mIsCollidingLeft},
      mEntityStatus         {other.mEntityStatus},
      mEntityType           {other.mEntityType},
      mAIType               {other.mAIType},
      mAIState              {other.mAIState},
      mWanderMap            {other.mWanderMap},
      mIgnoresMap           {other.mIgnoresMap}
{}

Entity &Entity::operator=(const Entity &other)
{
    if (this == &other) return *this;
    if (mTexture.id != 0) UnloadTexture(mTexture);
    mPosition              = other.mPosition;
    mMovement              = other.mMovement;
    mVelocity              = other.mVelocity;
    mAcceleration          = other.mAcceleration;
    mScale                 = other.mScale;
    mColliderDimensions    = other.mColliderDimensions;
    mTextureFilepath       = other.mTextureFilepath;
    mTexture               = other.mTextureFilepath.empty()
                                 ? Texture2D{}
                                 : LoadTexture(other.mTextureFilepath.c_str());
    mTextureType           = other.mTextureType;
    mSpriteSheetDimensions = other.mSpriteSheetDimensions;
    mAnimationAtlas        = other.mAnimationAtlas;
    mAnimationIndices      = other.mAnimationIndices;
    mDirection             = other.mDirection;
    mFrameSpeed            = other.mFrameSpeed;
    mCurrentFrameIndex     = other.mCurrentFrameIndex;
    mAnimationTime         = other.mAnimationTime;
    mIsJumping             = other.mIsJumping;
    mJumpingPower          = other.mJumpingPower;
    mSpeed                 = other.mSpeed;
    mAngle                 = other.mAngle;
    mIsCollidingTop        = other.mIsCollidingTop;
    mIsCollidingBottom     = other.mIsCollidingBottom;
    mIsCollidingRight      = other.mIsCollidingRight;
    mIsCollidingLeft       = other.mIsCollidingLeft;
    mEntityStatus          = other.mEntityStatus;
    mEntityType            = other.mEntityType;
    mAIType                = other.mAIType;
    mAIState               = other.mAIState;
    mWanderMap             = other.mWanderMap;
    mIgnoresMap            = other.mIgnoresMap;
    return *this;
}

bool Entity::isColliding(Entity *other) const
{
    if (!other->isActive() || other == this) return false;

    float xDist = fabs(mPosition.x - other->getPosition().x) -
                  ((mColliderDimensions.x + other->getColliderDimensions().x) / 2.0f);
    float yDist = fabs(mPosition.y - other->getPosition().y) -
                  ((mColliderDimensions.y + other->getColliderDimensions().y) / 2.0f);

    return (xDist < 0.0f && yDist < 0.0f);
}

void Entity::checkCollisionY(Entity *collidableEntities, int count)
{
    for (int i = 0; i < count; i++)
    {
        Entity *other = &collidableEntities[i];
        if (!isColliding(other)) continue;

        float yDist    = fabs(mPosition.y - other->mPosition.y);
        float yOverlap = fabs(yDist - (mColliderDimensions.y / 2.0f)
                                    - (other->mColliderDimensions.y / 2.0f));
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

void Entity::checkCollisionX(Entity *collidableEntities, int count)
{
    for (int i = 0; i < count; i++)
    {
        Entity *other = &collidableEntities[i];
        if (!isColliding(other)) continue;

        float yDist    = fabs(mPosition.y - other->mPosition.y);
        float yOverlap = fabs(yDist - (mColliderDimensions.y / 2.0f)
                                    - (other->mColliderDimensions.y / 2.0f));
        if (yOverlap < Y_COLLISION_THRESHOLD) continue;

        float xDist    = fabs(mPosition.x - other->mPosition.x);
        float xOverlap = fabs(xDist - (mColliderDimensions.x / 2.0f)
                                    - (other->mColliderDimensions.x / 2.0f));
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

void Entity::checkCollisionY(Map *map)
{
    if (!map) return;

    Vector2 topC  = { mPosition.x,                              mPosition.y - mColliderDimensions.y / 2.0f };
    Vector2 topL  = { mPosition.x - mColliderDimensions.x / 2.0f, mPosition.y - mColliderDimensions.y / 2.0f };
    Vector2 topR  = { mPosition.x + mColliderDimensions.x / 2.0f, mPosition.y - mColliderDimensions.y / 2.0f };
    Vector2 botC  = { mPosition.x,                              mPosition.y + mColliderDimensions.y / 2.0f };
    Vector2 botL  = { mPosition.x - mColliderDimensions.x / 2.0f, mPosition.y + mColliderDimensions.y / 2.0f };
    Vector2 botR  = { mPosition.x + mColliderDimensions.x / 2.0f, mPosition.y + mColliderDimensions.y / 2.0f };

    float xOv = 0.0f, yOv = 0.0f;

    if ((map->isSolidTileAt(topC, &xOv, &yOv) ||
         map->isSolidTileAt(topL, &xOv, &yOv) ||
         map->isSolidTileAt(topR, &xOv, &yOv)) && mVelocity.y < 0.0f)
    {
        mPosition.y    += yOv;
        mVelocity.y     = 0.0f;
        mIsCollidingTop = true;
    }

    if ((map->isSolidTileAt(botC, &xOv, &yOv) ||
         map->isSolidTileAt(botL, &xOv, &yOv) ||
         map->isSolidTileAt(botR, &xOv, &yOv)) && mVelocity.y > 0.0f)
    {
        mPosition.y       -= yOv;
        mVelocity.y        = 0.0f;
        mIsCollidingBottom = true;
    }
}

void Entity::checkCollisionX(Map *map)
{
    if (!map) return;

    Vector2 left  = { mPosition.x - mColliderDimensions.x / 2.0f, mPosition.y };
    Vector2 right = { mPosition.x + mColliderDimensions.x / 2.0f, mPosition.y };

    float xOv = 0.0f, yOv = 0.0f;

    if (map->isSolidTileAt(right, &xOv, &yOv) && mVelocity.x > 0.0f && yOv >= 0.5f)
    {
        mPosition.x      -= xOv * 1.01f;
        mVelocity.x       = 0.0f;
        mIsCollidingRight = true;
    }
    if (map->isSolidTileAt(left, &xOv, &yOv) && mVelocity.x < 0.0f && yOv >= 0.5f)
    {
        mPosition.x     += xOv * 1.01f;
        mVelocity.x      = 0.0f;
        mIsCollidingLeft = true;
    }
}

void Entity::animate(float deltaTime)
{
    mAnimationIndices = mAnimationAtlas.at(mDirection);
    mAnimationTime   += deltaTime;
    float spf         = 1.0f / mFrameSpeed;

    if (mAnimationTime >= spf)
    {
        mAnimationTime = 0.0f;
        mCurrentFrameIndex++;
        mCurrentFrameIndex %= (int)mAnimationIndices.size();
    }
}

void Entity::AIWander()
{
    if (mMovement.x == 0) moveLeft();

    if (mIsCollidingLeft)
    {
        mMovement.x =  1;
        mDirection  = RIGHT;
    }
    if (mIsCollidingRight)
    {
        mMovement.x = -1;
        mDirection  = LEFT;
    }

    if (mWanderMap)
    {
        float probeX  = mPosition.x + mMovement.x * (mColliderDimensions.x / 2.0f + 4.0f);
        float probeY  = mPosition.y + mColliderDimensions.y / 2.0f + 4.0f;
        float dummy   = 0.0f;
        bool  ground  = mWanderMap->isSolidTileAt({probeX, probeY}, &dummy, &dummy);
        if (!ground)
        {
            mMovement.x = -mMovement.x;
            mDirection  = (mMovement.x > 0) ? RIGHT : LEFT;
        }
    }
}

void Entity::AIFollow(Entity *target)
{
    switch (mAIState)
    {
    case IDLE:
        if (Vector2Distance(mPosition, target->getPosition()) < 250.0f)
            mAIState = WALKING;
        break;
    case WALKING:
        if (mPosition.x > target->getPosition().x) moveLeft();
        else                                        moveRight();
        break;
    default: break;
    }
}

void Entity::AIFly()
{
    mMovement.x =  1;
    mDirection  = RIGHT;
}

void Entity::AIActivate(Entity *target)
{
    switch (mAIType)
    {
    case WANDERER: AIWander();        break;
    case FOLLOWER: AIFollow(target);  break;
    case FLYER:    AIFly();           break;
    default: break;
    }
}

void Entity::update(float deltaTime, Entity *player, Map *map,
                    Entity *collidableEntities, int collisionCheckCount)
{
    if (mEntityStatus == INACTIVE) return;

    if (mEntityType == NPC) AIActivate(player);

    resetColliderFlags();

    mVelocity.x  = mMovement.x * mSpeed;
    mVelocity.x += mAcceleration.x * deltaTime;
    mVelocity.y += mAcceleration.y * deltaTime;

    if (mIsJumping)
    {
        mIsJumping   = false;
        mVelocity.y -= mJumpingPower;
    }

    mPosition.y += mVelocity.y * deltaTime;
    checkCollisionY(collidableEntities, collisionCheckCount);
    if (!mIgnoresMap) checkCollisionY(map);

    mPosition.x += mVelocity.x * deltaTime;
    checkCollisionX(collidableEntities, collisionCheckCount);
    if (!mIgnoresMap) checkCollisionX(map);

    if (mTextureType == ATLAS && GetLength(mMovement) != 0)
        animate(deltaTime);
}

void Entity::render()
{
    if (mEntityStatus == INACTIVE) return;

    Rectangle src;
    switch (mTextureType)
    {
    case SINGLE:
        src = { 0.0f, 0.0f,
                (float)mTexture.width, (float)mTexture.height };
        break;
    case ATLAS:
        src = getUVRectangle(&mTexture,
                             mAnimationIndices[mCurrentFrameIndex],
                             (int)mSpriteSheetDimensions.x,
                             (int)mSpriteSheetDimensions.y);
        break;
    default: break;
    }

    Rectangle dst = { mPosition.x, mPosition.y, mScale.x, mScale.y };
    Vector2   ori = { mScale.x / 2.0f, mScale.y / 2.0f };

    DrawTexturePro(mTexture, src, dst, ori, mAngle, WHITE);
}