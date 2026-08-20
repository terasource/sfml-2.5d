#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "player.hpp"
#include "TileMap.hpp"
#include "probs.hpp"
#include "mapLoader.hpp"
#include "collision.hpp"
#include "texturemanager.hpp"

class Game {
public:
  Game();
  void run();

private:
  void processEvent();
  void update(sf::Time dt);
  void render();

  TextureManager mTextureManager;

  TileMap map;
  TileAnimation animations;
  AnimationHandler mAnimationHandler;
  Player mPlayer;

  static constexpr sf::Vector2u mWindowSize = { 960, 640 };

  sf::Clock mClock;
  sf::Time globalTime;
  
  // this sets the physical engine's tickrate to 60. this determines to how many calculates should be performed within a second.
  static constexpr unsigned int updatesPerSecond = 60; 
  static constexpr sf::Time timePerTick = sf::seconds(1.0f / updatesPerSecond);
  sf::Time lastFrameDrawed = sf::Time::Zero;

  sf::Time fpsTimer;

  sf::RenderWindow mWindow;
  sf::View mView;
  sf::Sprite mBackGround;
  sf::Texture mBackGroundTexture;
  sf::Font mFont;
  sf::Text mFpsText;

  probs mProbs;

  std::string jsonpath = "assets\\Sample map.json";

};
