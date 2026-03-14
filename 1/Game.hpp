#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "player.hpp"
#include "TileMap.hpp"

class Game {
public:
  Game();
  void run();

private:
  void processEvent();
  void update(sf::Time dt);
  void render();

  Player mPlayer;
  TileMap map;

  sf::Vector2f mWindowSize;
  sf::Clock mClock;
  sf::Time dt;
  sf::RenderWindow mWindow;
  sf::View mView;
  sf::RectangleShape mBox;
  sf::Sprite mBackGround;
  sf::Texture mBackGroundTexture;
  sf::Font mFont;
  sf::Text mFpsText;


  std::string jsonpath = "assets\\Sample map.json";
};
