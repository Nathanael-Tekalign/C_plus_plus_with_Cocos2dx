#include "MainScene.h"
#include "base/CCDirector.h"
#include "2d/CCLabel.h"

#include <algorithm>
#include <cstdlib>

USING_NS_CC;

static float clampFloat(float v, float lo, float hi) {
    return std::max(lo, std::min(v, hi));
}

Scene* MainScene::createScene() {
    return MainScene::create();
}

bool MainScene::init() {
    if (!Scene::init()) return false;

    vs = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    // Background
    auto bg = LayerColor::create(Color4B(0, 0, 0, 255), vs.width, vs.height);
    bg->setPosition(origin);
    addChild(bg, -10);

    // UI
    scoreLabel = Label::createWithSystemFont("Score: 0", "Arial", 22);
    scoreLabel->setAnchorPoint(Vec2(0, 1));
    scoreLabel->setPosition(origin.x + 10, origin.y + vs.height - 10);
    addChild(scoreLabel, 100);

    timeLabel = Label::createWithSystemFont("Time: 0.0", "Arial", 22);
    timeLabel->setAnchorPoint(Vec2(1, 1));
    timeLabel->setPosition(origin.x + vs.width - 10, origin.y + vs.height - 10);
    addChild(timeLabel, 100);

    stateLabel = Label::createWithSystemFont("", "Arial", 28);
    stateLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
    stateLabel->setPosition(origin + vs / 2);
    addChild(stateLabel, 200);

    // Player
    player = makeSquare(Color4B(70, 200, 255, 255), 52.0f);
    player->setPosition(origin.x + vs.width * 0.5f, origin.y + 90.0f);
    addChild(player, 10);

    // Keyboard
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event*){
        if (keyCode == EventKeyboard::KeyCode::KEY_A ||
            keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
            moveLeft = true;
        }
        if (keyCode == EventKeyboard::KeyCode::KEY_D ||
            keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
            moveRight = true;
        }
        if (keyCode == EventKeyboard::KeyCode::KEY_R) {
            resetGame();
        }
    };
    listener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event*){
        if (keyCode == EventKeyboard::KeyCode::KEY_A ||
            keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
            moveLeft = false;
        }
        if (keyCode == EventKeyboard::KeyCode::KEY_D ||
            keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
            moveRight = false;
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    resetGame();
    scheduleUpdate();
    return true;
}

void MainScene::resetGame() {
    // Clear falling things
    for (auto& t : things) {
        if (t.node) t.node->removeFromParent();
    }
    things.clear();

    // Reset state
    moveLeft = moveRight = false;
    gameOver = false;
    timeAlive = 0.0f;
    score = 0;

    obstacleSpawnTimer = 0.0f;
    coinSpawnTimer = 0.0f;

    obstacleSpawnInterval = 0.80f;
    coinSpawnInterval = 2.20f;

    fallSpeed = 220.0f;
    difficultyTimer = 0.0f;

    stateLabel->setString("Dodge blocks, collect coins!\nR = Restart");
    stateLabel->setOpacity(200);

    // Put player back
    if (player) player->setPosition(origin.x + vs.width * 0.5f, origin.y + 90.0f);

    refreshHUD();
}

Node* MainScene::makeSquare(const Color4B& color, float size) {
    return LayerColor::create(color, size, size);
}

float MainScene::randRange(float a, float b) {
    float r = (float)std::rand() / (float)RAND_MAX;
    return a + (b - a) * r;
}

void MainScene::spawnObstacle() {
    const float size = 46.0f;
    auto n = makeSquare(Color4B(140, 140, 140, 255), size);

    float x = randRange(origin.x + size * 0.5f, origin.x + vs.width - size * 0.5f);
    float y = origin.y + vs.height + size;
    n->setPosition(x, y);

    addChild(n, 5);

    FallingThing t;
    t.node = n;
    t.vy = -(fallSpeed + randRange(0.0f, 120.0f));
    t.isCoin = false;
    things.push_back(t);
}

void MainScene::spawnCoin() {
    const float size = 34.0f;
    auto n = makeSquare(Color4B(255, 200, 40, 255), size);

    float x = randRange(origin.x + size * 0.5f, origin.x + vs.width - size * 0.5f);
    float y = origin.y + vs.height + size;
    n->setPosition(x, y);

    addChild(n, 6);

    FallingThing t;
    t.node = n;
    t.vy = -(fallSpeed * 0.9f + randRange(0.0f, 80.0f));
    t.isCoin = true;
    things.push_back(t);
}

void MainScene::setGameOver(const char* reason) {
    if (gameOver) return;
    gameOver = true;

    std::string msg = "Game Over!\n";
    msg += reason;
    msg += "\n\nScore: " + std::to_string(score);
    msg += "\nTime: " + std::to_string((int)timeAlive) + "s";
    msg += "\n\nPress R to restart";

    stateLabel->setString(msg);
    stateLabel->setOpacity(255);
}

void MainScene::refreshHUD() {
    if (scoreLabel) scoreLabel->setString("Score: " + std::to_string(score));
    if (timeLabel) {
        char buf[64];
        std::snprintf(buf, sizeof(buf), "Time: %.1f", timeAlive);
        timeLabel->setString(buf);
    }
}

void MainScene::update(float dt) {
    if (dt > 0.05f) dt = 0.05f; // prevent huge jumps on hitch

    if (!gameOver) {
        timeAlive += dt;

        // ramp difficulty slowly
        difficultyTimer += dt;
        if (difficultyTimer >= 1.0f) {
            difficultyTimer -= 1.0f;
            fallSpeed += 6.0f;                         // slightly faster over time
            obstacleSpawnInterval = std::max(0.35f, obstacleSpawnInterval - 0.01f);
        }

        // player movement
        float dir = 0.0f;
        if (moveLeft) dir -= 1.0f;
        if (moveRight) dir += 1.0f;

        if (player) {
            Vec2 p = player->getPosition();
            p.x += dir * playerSpeed * dt;

            float half = player->getBoundingBox().size.width * 0.5f;
            p.x = clampFloat(p.x, origin.x + half, origin.x + vs.width - half);
            player->setPosition(p);
        }

        // spawning
        obstacleSpawnTimer += dt;
        if (obstacleSpawnTimer >= obstacleSpawnInterval) {
            obstacleSpawnTimer -= obstacleSpawnInterval;
            spawnObstacle();
        }

        coinSpawnTimer += dt;
        if (coinSpawnTimer >= coinSpawnInterval) {
            coinSpawnTimer -= coinSpawnInterval;
            spawnCoin();
        }
    }

    // move things + collision
    Rect playerBox;
    if (player) playerBox = player->getBoundingBox();

    for (auto& t : things) {
        if (!t.node) continue;

        if (!gameOver) {
            Vec2 p = t.node->getPosition();
            p.y += t.vy * dt;
            t.node->setPosition(p);
        }

        if (!gameOver && player) {
            if (t.node->getBoundingBox().intersectsRect(playerBox)) {
                if (t.isCoin) {
                    score += 1;
                    t.node->removeFromParent();
                    t.node = nullptr;
                } else {
                    setGameOver("You got hit!");
                }
            }
        }
    }

    // cleanup
    const float killY = origin.y - 80.0f;
    things.erase(std::remove_if(things.begin(), things.end(),
        [&](const FallingThing& t){
            if (!t.node) return true;
            if (t.node->getPositionY() < killY) {
                t.node->removeFromParent();
                return true;
            }
            return false;
        }), things.end());

    refreshHUD();
}