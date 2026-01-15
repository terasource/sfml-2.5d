#include <iostream>
#include "Game.hpp"

Game::Game() :
    mCharacterTexture(),
    mCharacter(mCharacterTexture),
    mBackGroundTexture(),
    mBackGround(mBackGroundTexture),
    mWindow(sf::VideoMode({ 1280,720 }), "myGame", sf::Style::Close),
    mAnimationSpeed(0.1f),
    currentFrame(0),
    mWindowSize(1280, 720) {
    mWindow.setFramerateLimit(60);

    if (mBackGroundTexture.loadFromFile("assets/env/grass.png")) {
        mBackGroundTexture.setRepeated(true);

        mBackGround.setTexture(mBackGroundTexture);
        mBackGroundTexture.setSmooth(false);

        mBackGround.setTextureRect(sf::IntRect({ 0, 0 }, { 5000, 5000 }));
        mBackGround.setScale({ 4.f, 4.f });
    }
    mTextures.resize(6);
    for (int i = 0; i < mTextures.size(); i++) {
        std::string file_name = "assets/characters/renklianimasyon" + std::to_string(i + 1) + ".png";

        if (!mTextures[i].loadFromFile(file_name)) {
            std::cout << "image " << i << " cannot uploaded!" << std::endl;
        }
    }

    sf::Vector2u textureSize = mTextures[0].getSize();

    mCharacter.setOrigin({ textureSize.x / 2.0f, textureSize.y / 2.f });
    mCharacter.setTexture(mTextures[0], true);
    mCharacter.setScale({ 4.f, 4.f });
    mCharacter.setPosition({ 100.f, 100.f });

    mView.setSize(mWindowSize);

    mBox.setSize({ 50.f,50.f });
    mBox.setFillColor(sf::Color::Red);
    mBox.setPosition({ 300.f, 300.f });
}

void Game::run() {
    while (mWindow.isOpen()) {
        processEvent();

        sf::Time dt = mClock.restart();
        update(dt);
        render();
    }
}

void Game::processEvent() {
    while (const std::optional event = mWindow.pollEvent()) {
        if (event->is < sf::Event::Closed>()) {
            mWindow.close();
        }
    }
}

void Game::update(sf::Time dt) {
    bool isMoving = false;
    sf::Vector2f movement(0.f, 0.f);
    float speed = 300.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        movement.y -= speed;
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        mCharacter.setScale({ -4.f, 4.f });
        movement.x -= speed;
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        movement.y += speed;
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        mCharacter.setScale({ 4.f, 4.f });
        movement.x += speed;
        isMoving = true;
    }

    mCharacter.move(movement * dt.asSeconds());

    if (isMoving) {
        mStopTimer = 0;
        mAnimationSpeed = 30.f / speed;
        if (!mWasMoving) {
            mAnimationTimer = 0;
            if (currentFrame >= 6)
                currentFrame = 1;
            if (currentFrame < 6)
                mCharacter.setTexture(mTextures[currentFrame++], true);
        }
        else {
            mAnimationTimer += dt.asSeconds();
            if (mAnimationTimer >= mAnimationSpeed) {
                mAnimationTimer = 0;
                currentFrame++;

                if (currentFrame >= 6)
                    currentFrame = 1;
            }

            if (currentFrame < 6)
                mCharacter.setTexture(mTextures[currentFrame], true);
        }

    }
    else {
        mStopTimer += dt.asSeconds();

        if (mStopTimer >= 0.15)
            currentFrame = 0;

        if (currentFrame >= 6)
            currentFrame = 1;

        if (currentFrame < 6)
            mCharacter.setTexture(mTextures[currentFrame], true);

    }

    mView.setCenter(mCharacter.getPosition());

    mWasMoving = isMoving;
}

void Game::render() {
    mWindow.clear();
    mWindow.setView(mView);
    mWindow.draw(mBackGround);
    mWindow.draw(mBox);
    mWindow.draw(mCharacter);
    mWindow.display();
}