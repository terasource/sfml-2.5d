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

    if (mBackGroundTexture.loadFromFile("assets/env/grass.png")) {
        mBackGroundTexture.setRepeated(true);

        mBackGround.setTexture(mBackGroundTexture);
        mBackGroundTexture.setSmooth(false);

        mBackGround.setTextureRect(sf::IntRect({ 0, 0 }, { 5000, 5000 }));
        mBackGround.setScale({ 4.f, 4.f });
    }
    for (int i = 0; i < 5; i++) {
        loadMap(jsonpath, map, i);
        std::cout << "layer = " << i << std::endl;
    }

    mFpsText.setCharacterSize(20);
    mFpsText.setFillColor(sf::Color::Blue);
    mFpsText.setPosition({ 5.f, 5.f });
    mFpsText.setString("FPS: 0");


    mView.setSize(mWindowSize);
    mBox.setSize({ 50.f,50.f });
    mBox.setFillColor(sf::Color::Red);
    mBox.setPosition({ 300.f, 300.f });

    map.setScale({ 1.5f, 1.5f });
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
    mWindow.setView(mView);
    //mWindow.draw(mBackGround);
    mWindow.draw(map);
    //mWindow.draw(mBox);
    mPlayer.draw(mWindow);

    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(mFpsText);

    mWindow.display();
}