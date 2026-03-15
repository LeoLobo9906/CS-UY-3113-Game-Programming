//project3 - Lunar Lander
#ifndef ENTITY_H
#define ENTITY_H

#include "cs3113.h"

enum Direction    { LEFT, UP, RIGHT, DOWN         };
enum EntityStatus { ACTIVE, INACTIVE              };
enum EntityType   { PLAYER, PLATFORM, MOVING_PLATFORM, TREE, NONE_TYPE };

class Entity
{
private:
    Vector2 mPosition;
    Vector2 mMovement;
    Vector2 mVelocity;
    Vector2 mAcceleration;

    Vector2 mScale;
    Vector2 mColliderDimensions;

    Texture2D   mTexture;
    TextureType mTextureType;
    Vector2     mSpriteSheetDimensions;

    std::map<Direction, std::vector<int>> mAnimationAtlas;
    std::vector<int> mAnimationIndices;
    Direction mDirection;
    int       mFrameSpeed;

    int   mCurrentFrameIndex = 0;
    float mAnimationTime     = 0.0f;

    // Ping-pong animation support
    std::vector<int> mPingPongFrames;
    int              mPingPongIndex = 0;
    bool             mUsePingPong   = false;

    bool  mIsJumping    = false;
    float mJumpingPower = 0.0f;

    int   mSpeed;
    float mAngle;

    bool mIsCollidingTop    = false;
    bool mIsCollidingBottom = false;
    bool mIsCollidingRight  = false;
    bool mIsCollidingLeft   = false;

    EntityStatus mEntityStatus = ACTIVE;
    EntityType   mEntityType;

    bool isColliding(Entity *other) const;
    void checkCollisionY(Entity *collidableEntities, int collisionCheckCount);
    void checkCollisionX(Entity *collidableEntities, int collisionCheckCount);
    void resetColliderFlags()
    {
        mIsCollidingTop    = false;
        mIsCollidingBottom = false;
        mIsCollidingRight  = false;
        mIsCollidingLeft   = false;
    }

    void animate(float deltaTime);

public:
    static constexpr int   DEFAULT_SIZE          = 250;
    static constexpr int   DEFAULT_SPEED         = 200;
    static constexpr int   DEFAULT_FRAME_SPEED   = 14;
    static constexpr float Y_COLLISION_THRESHOLD = 0.5f;

    Entity();
    Entity(Vector2 position, Vector2 scale, const char *textureFilepath,
           EntityType entityType);
    Entity(Vector2 position, Vector2 scale, const char *textureFilepath,
           TextureType textureType, Vector2 spriteSheetDimensions,
           std::map<Direction, std::vector<int>> animationAtlas,
           EntityType entityType);
    ~Entity();

    void update(float deltaTime, Entity *platforms, int platformCount);
    void render();
    void normaliseMovement() { Normalise(&mMovement); }

    void jump()       { mIsJumping   = true;     }
    void activate()   { mEntityStatus = ACTIVE;   }
    void deactivate() { mEntityStatus = INACTIVE; }
    void displayCollider();

    bool isActive() const { return mEntityStatus == ACTIVE; }

    void moveLeft()  { mMovement.x = -1; mDirection = LEFT;  }
    void moveRight() { mMovement.x =  1; mDirection = RIGHT; }
    void resetMovement() { mMovement = { 0.0f, 0.0f }; }

    // Ping-pong animation (for bird flapping)
    void setPingPongFrames(std::vector<int> frames) { mPingPongFrames = frames; mUsePingPong = true; }

    Vector2     getPosition()           const { return mPosition;           }
    Vector2     getMovement()           const { return mMovement;           }
    Vector2     getVelocity()           const { return mVelocity;           }
    Vector2     getAcceleration()       const { return mAcceleration;       }
    Vector2     getScale()              const { return mScale;              }
    Vector2     getColliderDimensions() const { return mColliderDimensions; }
    EntityType  getEntityType()         const { return mEntityType;         }
    Direction   getDirection()          const { return mDirection;          }

    bool isCollidingTop()    const { return mIsCollidingTop;    }
    bool isCollidingBottom() const { return mIsCollidingBottom; }
    bool isCollidingLeft()   const { return mIsCollidingLeft;   }
    bool isCollidingRight()  const { return mIsCollidingRight;  }

    void setPosition(Vector2 p)           { mPosition           = p;                      }
    void setVelocity(Vector2 v)           { mVelocity           = v;                      }
    void setAcceleration(Vector2 a)       { mAcceleration       = a;                      }
    void setMovement(Vector2 m)           { mMovement           = m;                      }
    void setScale(Vector2 s)              { mScale              = s;                      }
    void setColliderDimensions(Vector2 d) { mColliderDimensions = d;                      }
    void setSpeed(int sp)                 { mSpeed              = sp;                     }
    void setFrameSpeed(int fs)            { mFrameSpeed         = fs;                     }
    void setJumpingPower(float jp)        { mJumpingPower       = jp;                     }
    void setAngle(float a)                { mAngle              = a;                      }
    void setEntityType(EntityType t)      { mEntityType         = t;                      }
    void setTexture(const char *fp)       { mTexture            = LoadTexture(fp);        }
};

#endif // ENTITY_H