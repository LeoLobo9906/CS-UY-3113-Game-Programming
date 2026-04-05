#include "levelb.h"

LevelB::LevelB() : Scene{ {0.0f}, nullptr } {}
LevelB::LevelB(Vector2 origin, const char *bgHexCode) : Scene{ origin, bgHexCode } {}
LevelB::~LevelB() { shutdown(); }

void LevelB::initialise()
{
    mGameState.nextSceneID = 0;
    mGameState.coinsLeft   = NUM_COINS;

    mGameState.map = new Map(
        LEVELB_WIDTH, LEVELB_HEIGHT,
        (unsigned int *)mLevelData,
        "assets/brick.png",
        TILE_SIZE,
        1, 1,
        mOrigin
    );

    std::map<Direction, std::vector<int>> marioAtlas = {
        { RIGHT, { 1, 2, 3 } },
        { LEFT,  { 1, 2, 3 } },
        { UP,    { 4 }       },
        { DOWN,  { 0 }       },
    };

    float mapLeft = mOrigin.x - (LEVELB_WIDTH * TILE_SIZE / 2.0f);

    mGameState.mario = new Entity(
        { mapLeft + TILE_SIZE * 2,
          mOrigin.y + TILE_SIZE * 3 - TILE_SIZE * 2 },
        { 48.0f, 48.0f },
        "assets/mario.png",
        ATLAS,
        { 1, 5 },
        marioAtlas,
        PLAYER
    );
    mGameState.mario->setJumpingPower(550.0f);
    mGameState.mario->setSpeed(160);
    mGameState.mario->setFrameSpeed(8);
    mGameState.mario->setColliderDimensions({ 36.0f, 44.0f });
    mGameState.mario->setAcceleration({ 0.0f, GRAVITY });

    mGameState.enemyCount = NUM_ENEMIES;
    mGameState.enemies    = new Entity[NUM_ENEMIES];

    std::map<Direction, std::vector<int>> goombaAtlas = {
        { LEFT,  { 0, 1 } }, { RIGHT, { 0, 1 } },
        { UP,    { 0 }    }, { DOWN,  { 0 }    },
    };
    std::map<Direction, std::vector<int>> cheepAtlas = {
        { LEFT,  { 0, 1 } }, { RIGHT, { 0, 1 } },
        { UP,    { 0, 1 } }, { DOWN,  { 0, 1 } },
    };

    mGameState.enemies[0] = Entity(
        { mapLeft + TILE_SIZE * 3,
          mOrigin.y + TILE_SIZE * 3 - TILE_SIZE },
        { 48.0f, 48.0f },
        "assets/goomba.png",
        ATLAS, { 1, 2 }, goombaAtlas, NPC
    );
    mGameState.enemies[0].setAIType(WANDERER);
    mGameState.enemies[0].setAIState(WALKING);
    mGameState.enemies[0].setSpeed(70);
    mGameState.enemies[0].setAcceleration({ 0.0f, GRAVITY });
    mGameState.enemies[0].setColliderDimensions({ 40.0f, 44.0f });
    mGameState.enemies[0].setWanderMap(mGameState.map);
    mGameState.enemies[0].deactivate();

    mGameState.enemies[1] = Entity(
        { mapLeft + TILE_SIZE * 17,
          mOrigin.y + TILE_SIZE * 3 - TILE_SIZE },
        { 48.0f, 48.0f },
        "assets/goomba.png",
        ATLAS, { 1, 2 }, goombaAtlas, NPC
    );
    mGameState.enemies[1].setAIType(WANDERER);
    mGameState.enemies[1].setAIState(WALKING);
    mGameState.enemies[1].setSpeed(70);
    mGameState.enemies[1].setAcceleration({ 0.0f, GRAVITY });
    mGameState.enemies[1].setColliderDimensions({ 40.0f, 44.0f });
    mGameState.enemies[1].setWanderMap(mGameState.map);

    mGameState.enemies[2] = Entity(
        { mapLeft - 64.0f, mOrigin.y - TILE_SIZE },
        { 48.0f, 48.0f },
        "assets/bullet.png",
        NONE
    );
    mGameState.enemies[2].setEntityType(NPC);
    mGameState.enemies[2].setAIType(FLYER);
    mGameState.enemies[2].setAIState(WALKING);
    mGameState.enemies[2].setSpeed(140);
    mGameState.enemies[2].setAcceleration({ 0.0f, 0.0f });
    mGameState.enemies[2].setColliderDimensions({ 44.0f, 44.0f });

    mGameState.enemies[3] = Entity(
        { mapLeft + TILE_SIZE * 10,
          mOrigin.y + TILE_SIZE * 5 },
        { 48.0f, 48.0f },
        "assets/cheep.png",
        ATLAS, { 1, 2 }, cheepAtlas, NPC
    );
    mGameState.enemies[3].setAIType(FOLLOWER);
    mGameState.enemies[3].setAIState(IDLE);
    mGameState.enemies[3].setSpeed(0);
    mGameState.enemies[3].setAcceleration({ 0.0f, GRAVITY });
    mGameState.enemies[3].setVelocity({ 0.0f, -600.0f });
    mGameState.enemies[3].setColliderDimensions({ 40.0f, 40.0f });
    mGameState.enemies[3].setIgnoresMap(true);

    mGameState.coinCount = NUM_COINS;
    mGameState.coins     = new Entity[NUM_COINS];

    float coinPositions[NUM_COINS][2] = {
        { mapLeft + TILE_SIZE * 2,  mOrigin.y + TILE_SIZE * 1 },
        { mapLeft + TILE_SIZE * 5,  mOrigin.y - TILE_SIZE * 1 },
        { mapLeft + TILE_SIZE * 10, mOrigin.y + TILE_SIZE * 1 },
        { mapLeft + TILE_SIZE * 17, mOrigin.y - TILE_SIZE * 1 },
        { mapLeft + TILE_SIZE * 23, mOrigin.y + TILE_SIZE * 1 },
        { mapLeft + TILE_SIZE * 27, mOrigin.y - TILE_SIZE * 1 }
    };

    for (int i = 0; i < NUM_COINS; i++)
    {
        mGameState.coins[i] = Entity(
            { coinPositions[i][0], coinPositions[i][1] },
            { 40.0f, 40.0f },
            "assets/coin.png",
            COIN_TYPE
        );
        mGameState.coins[i].setColliderDimensions({ 32.0f, 32.0f });
    }

    mGameState.bgm       = LoadMusicStream("assets/overworld.ogg");
    mGameState.jumpSound = LoadSound("assets/jump.wav");
    mGameState.coinSound = LoadSound("assets/coin.wav");
    mGameState.hurtSound = LoadSound("assets/hurt.wav");
    SetMusicVolume(mGameState.bgm, 0.5f);
    PlayMusicStream(mGameState.bgm);
}

void LevelB::update(float deltaTime)
{
    UpdateMusicStream(mGameState.bgm);

    Entity *mario   = mGameState.mario;
    Map    *map     = mGameState.map;
    Entity *enemies = mGameState.enemies;
    int     eCount  = mGameState.enemyCount;

    float mapLeft  = mOrigin.x - (LEVELB_WIDTH * TILE_SIZE / 2.0f);
    float mapRight = mOrigin.x + (LEVELB_WIDTH * TILE_SIZE / 2.0f);

    mario->update(deltaTime, nullptr, map, nullptr, 0);

    for (int i = 0; i < eCount; i++)
        enemies[i].update(deltaTime, mario, map, nullptr, 0);

    if (enemies[2].getPosition().x > mapRight + 64.0f)
    {
        enemies[2].setPosition({ mapLeft - 64.0f,
                                  enemies[2].getPosition().y });
        enemies[2].setVelocity({ 0.0f, 0.0f });
    }

    if (enemies[3].getPosition().y > mGameState.map->getBottomBoundary() + 48.0f)
    {
        enemies[3].setPosition({ mapLeft + TILE_SIZE * 10,
                                  mOrigin.y + TILE_SIZE * 5 });
        enemies[3].setVelocity({ 0.0f, -600.0f });
    }

    for (int i = 0; i < mGameState.coinCount; i++)
    {
        if (!mGameState.coins[i].isActive()) continue;
        if (mario->isCollidingWith(&mGameState.coins[i]))
        {
            mGameState.coins[i].deactivate();
            mGameState.coinsLeft--;
            PlaySound(mGameState.coinSound);
        }
    }

    for (int i = 0; i < eCount; i++)
    {
        if (!enemies[i].isActive()) continue;
        if (mario->isCollidingWith(&enemies[i]))
        {
            PlaySound(mGameState.hurtSound);
            mGameState.lives--;
            mGameState.nextSceneID = (mGameState.lives <= 0) ? 4 : 2;
            return;
        }
    }

    if (mario->getPosition().y > mGameState.map->getBottomBoundary() + 100.0f)
    {
        PlaySound(mGameState.hurtSound);
        mGameState.lives--;
        mGameState.nextSceneID = (mGameState.lives <= 0) ? 4 : 2;
        return;
    }

    if (mGameState.coinsLeft <= 0)
        mGameState.nextSceneID = 3;
}

void LevelB::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));
    mGameState.map->render();
    for (int i = 0; i < mGameState.coinCount;  i++) mGameState.coins[i].render();
    for (int i = 0; i < mGameState.enemyCount; i++) mGameState.enemies[i].render();
    mGameState.mario->render();
}

void LevelB::shutdown()
{
    delete   mGameState.mario;
    delete[] mGameState.enemies;
    delete[] mGameState.coins;
    delete   mGameState.map;
    UnloadMusicStream(mGameState.bgm);
    UnloadSound(mGameState.jumpSound);
    UnloadSound(mGameState.coinSound);
    UnloadSound(mGameState.hurtSound);
    mGameState.mario   = nullptr;
    mGameState.enemies = nullptr;
    mGameState.coins   = nullptr;
    mGameState.map     = nullptr;
}