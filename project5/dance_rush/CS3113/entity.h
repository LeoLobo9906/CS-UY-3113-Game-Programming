#ifndef ENTITY_H
#define ENTITY_H

#include "map.h"

enum Direction    { LEFT, UP, RIGHT, DOWN                    };
enum EntityStatus { ACTIVE, INACTIVE                         };
enum EntityType   { PLAYER, PLATFORM, NPC, COIN_TYPE, NONE   };
enum AIType       { WANDERER, FOLLOWER, FLYER                };
enum AIState      { WALKING, IDLE, FOLLOWING                 };

class Entity
{
private:
    Vector2 mPosition;
    Vector2 mMovement;
    Vector2 mVelocity;
    Vector2 mAcceleration;

    Vector2 mScale;
    Vector2 mColliderDimensions;

    std::string mTextureFilepath;
    Texture2D   mTexture;
    TextureType mTextureType;
    Vector2     mSpriteSheetDimensions;

    std::map<Direction, std::vector<int>> mAnimationAtlas;
    std::vector<int> mAnimationIndices;
    Direction mDirection;
    int       mFrameSpeed;

    int   mCurrentFrameIndex = 0;
    float mAnimationTime     = 0.0f;

    bool  mIsJumping    = false;
    float mJumpingPower = 0.0f;

    int   mSpeed;
    float mAngle;

    bool mIsCollidingTop    = false;
    bool mIsCollidingBottom = false;
    bool mIsCollidingRight  = false;
    bool mIsCollidingLeft   = false;

    EntityStatus mEntityStatus = ACTIVE;
    bool         mIgnoresMap    = false;
    EntityType   mEntityType;

    AIType  mAIType;
    AIState mAIState;
    Map    *mWanderMap = nullptr;

    bool isColliding(Entity *other) const;

    void checkCollisionY(Entity *collidableEntities, int count);
    void checkCollisionY(Map *map);
    void checkCollisionX(Entity *collidableEntities, int count);
    void checkCollisionX(Map *map);

    void resetColliderFlags()
    {
        mIsCollidingTop    = false;
        mIsCollidingBottom = false;
        mIsCollidingRight  = false;
        mIsCollidingLeft   = false;
    }

    void animate(float deltaTime);
    void AIActivate(Entity *target);
    void AIWander();
    void AIFollow(Entity *target);
    void AIFly();

public:
    static constexpr int   DEFAULT_SIZE          = 48;
    static constexpr int   DEFAULT_SPEED         = 80;
    static constexpr int   DEFAULT_FRAME_SPEED   = 8;
    static constexpr float Y_COLLISION_THRESHOLD = 0.5f;

    Entity();
    Entity(Vector2 position, Vector2 scale, const char *textureFilepath,
           EntityType entityType);
    Entity(Vector2 position, Vector2 scale, const char *textureFilepath,
           TextureType textureType, Vector2 spriteSheetDimensions,
           std::map<Direction, std::vector<int>> animationAtlas,
           EntityType entityType);
    Entity(const Entity &other);
    Entity &operator=(const Entity &other);
    ~Entity();

    void update(float deltaTime, Entity *player, Map *map,
                Entity *collidableEntities, int collisionCheckCount);
    void render();
    void normaliseMovement() { Normalise(&mMovement); }

    void jump()       { mIsJumping    = true;    }
    void activate()   { mEntityStatus = ACTIVE;  }
    void deactivate() { mEntityStatus = INACTIVE;}

    bool isActive()          const { return mEntityStatus == ACTIVE; }

    bool isCollidingWith(Entity *other) const { return isColliding(other); }

    void moveLeft()  { mMovement.x = -1; mDirection = LEFT;  }
    void moveRight() { mMovement.x =  1; mDirection = RIGHT; }
    void resetMovement() { mMovement = {0.0f, 0.0f}; }

    Vector2    getPosition()           const { return mPosition;           }
    Vector2    getMovement()           const { return mMovement;           }
    Vector2    getVelocity()           const { return mVelocity;           }
    Vector2    getScale()              const { return mScale;              }
    Vector2    getColliderDimensions() const { return mColliderDimensions; }
    EntityType getEntityType()         const { return mEntityType;         }
    bool       isCollidingBottom()     const { return mIsCollidingBottom;  }
    bool       isCollidingTop()        const { return mIsCollidingTop;     }

    void setPosition(Vector2 p)            { mPosition           = p;                   }
    void setVelocity(Vector2 v)            { mVelocity           = v;                   }
    void setAcceleration(Vector2 a)        { mAcceleration       = a;                   }
    void setMovement(Vector2 m)            { mMovement           = m;                   }
    void setScale(Vector2 s)               { mScale              = s;                   }
    void setColliderDimensions(Vector2 d)  { mColliderDimensions = d;                   }
    void setSpeed(int sp)                  { mSpeed              = sp;                  }
    void setFrameSpeed(int fs)             { mFrameSpeed         = fs;                  }
    void setJumpingPower(float jp)         { mJumpingPower       = jp;                  }
    void setAngle(float a)                 { mAngle              = a;                   }
    void setEntityType(EntityType t)       { mEntityType         = t;                   }
    void setTexture(const char *fp)        { if (mTexture.id != 0) UnloadTexture(mTexture); mTextureFilepath = fp; mTexture = LoadTexture(fp); }
    void setAIState(AIState s)             { mAIState            = s;                   }
    void setAIType(AIType t)               { mAIType             = t;                   }
    void setIgnoresMap(bool v)             { mIgnoresMap         = v;                   }
    void setWanderMap(Map *m)              { mWanderMap          = m;                   }
    void setDirection(Direction d)
    {
        mDirection = d;
        if (mTextureType == ATLAS)
            mAnimationIndices = mAnimationAtlas.at(mDirection);
    }
};

#endif