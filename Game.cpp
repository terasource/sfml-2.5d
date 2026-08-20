#include <iostream>
#include "Game.hpp"

Game::Game() :
    mTextureManager(),
    mAnimationHandler(mTextureManager),
    mPlayer(mTextureManager, mAnimationHandler),
    mBackGroundTexture(),
    mBackGround(mBackGroundTexture),
    mWindow(sf::VideoMode(mWindowSize), "myGame", sf::Style::Default),
    mFont("assets/fonts/arial.ttf"),
    mFpsText(mFont),
    mView(sf::FloatRect({ 0.f, 0.f }, sf::Vector2f(mWindowSize)))
{   
    mAnimationHandler.load_sprites_textures();
    mPlayer.initialize_sprites_textures();
    mWindow.setFramerateLimit(60);

    loadMapData(jsonpath, map);

    mProbs.matchTextures();

    mFpsText.setCharacterSize(20);
    mFpsText.setFillColor(sf::Color::Blue);
    mFpsText.setPosition({ 5.f, 5.f });
    mFpsText.setString("FPS: 0");
}

void Game::run() {
    while (mWindow.isOpen()) {
        processEvent();

        sf::Time dt = mClock.restart();
        globalTime += dt;
        lastFrameDrawed += dt;

        
        while (lastFrameDrawed >= timePerTick) {
            lastFrameDrawed -= timePerTick;
            processEvent();
            update(timePerTick);
        }


        fpsTimer += dt;

        if (fpsTimer.asSeconds() > 1.5f) {
            int fps = 1.0f / dt.asSeconds();
            mFpsText.setString("FPS: " + std::to_string(fps));
            std::cout << "{" << mPlayer.getPosition().x << " ," << mPlayer.getPosition().y << " }" << std::endl;
            fpsTimer = sf::Time::Zero;
        }

        render();
    }
}


void Game::processEvent() {
    while (const std::optional event = mWindow.pollEvent()) {
        if (event->is < sf::Event::Closed>()) {
            mWindow.close();
        }

        if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            float nsx = (float)resized->size.x;
            float nsy = (float)resized->size.y;
            mView.setSize({ nsx, nsy });
            mView.setCenter({ nsx / 2, nsy / 2 });
        }
    }
}

void Game::update(sf::Time dt) {
    mPlayer.update(dt, mWindow.hasFocus());

    map.update(dt);                     
}

void Game::render() {
    mWindow.clear();
    mWindow.setView(mView);
    mView.setCenter({std::round(mPlayer.getPosition().x), std::round(mPlayer.getPosition().y)});

    mWindow.draw(map);

    mProbs.DrawProbs(mWindow);
    mPlayer.draw(mWindow);

    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(mFpsText);

    mWindow.display();
}