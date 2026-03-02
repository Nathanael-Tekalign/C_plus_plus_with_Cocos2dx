#pragma once
#include "cocos2d.h"
#include <vector>

class MainScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(MainScene);

    void update(float dt) override;

private:
    struct FallingThing {
        cocos2d::Node* node = nullptr;
        float vy = 0.0f;        // pixels/sec (negative = down)
        bool isCoin = false;
    };

    // world
    cocos2d::Size vs;
    cocos2d::Vec2 origin;

    // player
    cocos2d::Node* player = nullptr;
    float playerSpeed = 420.0f; // px/sec
    bool moveLeft = false;
    bool moveRight = false;

    // spawners
    float obstacleSpawnTimer = 0.0f;
    float obstacleSpawnInterval = 0.8f;
    float coinSpawnTimer = 0.0f;
    float coinSpawnInterval = 2.2f;

    // difficulty
    float fallSpeed = 220.0f;        // base px/sec
    float difficultyTimer = 0.0f;    // ramps speed over time

    // state
    bool gameOver = false;
    float timeAlive = 0.0f;
    int score = 0;

    // UI
    cocos2d::Label* scoreLabel = nullptr;
    cocos2d::Label* timeLabel = nullptr;
    cocos2d::Label* stateLabel = nullptr;

    std::vector<FallingThing> things;

    // helpers
    cocos2d::Node* makeSquare(const cocos2d::Color4B& color, float size);
    float randRange(float a, float b);
    void spawnObstacle();
    void spawnCoin();
    void refreshHUD();
    void setGameOver(const char* reason);
    void resetGame();
};