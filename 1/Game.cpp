#include <iostream>
#include "Game.hpp"
#include "player.hpp"
#include "TileMap.hpp"
#include "mapLoader.hpp"

Game::Game() :
    mPlayer(),
    mBackGroundTexture(),
    mBackGround(mBackGroundTexture),
    mWindow(sf::VideoMode({ 1280,720 }), "myGame", sf::Style::Close),
    mWindowSize({ 1280, 720 }),
    mFont("assets/fonts/arial.ttf"),
    mFpsText(mFont)
{
    mWindow.setFramerateLimit(0);

    loadMap(jsonpath, map);

    mFpsText.setCharacterSize(20);
    mFpsText.setFillColor(sf::Color::Blue);
    mFpsText.setPosition({ 5.f, 5.f });
    mFpsText.setString("FPS: 0");

    //mView.setSize(mWindowSize);
    float sizeX = (float)(1280.0 / 960.0); // 30 * 32
    float sizeY = (float)(720.0 / 640.0); // 20 * 32
    map.setScale({ sizeX, sizeY });
    //map.setScale({ 1.35f, 1.2f });
}

void Game::run() {
    while (mWindow.isOpen()) {
        processEvent();

        dt = mClock.restart();

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

    mPlayer.update(dt, mWindow.hasFocus());

    if (dt.asSeconds() > 0) {
        int fps = 1 / dt.asSeconds();
        mFpsText.setString("FPS: " + std::to_string(fps));
    }
}

void Game::render() {
    mWindow.clear();
    //mWindow.setView(mView);
    //mWindow.draw(mBackGround);
    mWindow.draw(map);
    //mWindow.draw(mBox);
    mPlayer.draw(mWindow);

    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(mFpsText);

    mWindow.display();
}