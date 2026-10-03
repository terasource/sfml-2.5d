#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "player.hpp"
#include "TileMap.hpp"
#include "probs.hpp"
#include "mapLoader.hpp"
#include "collisionmanager.hpp"
#include "texturemanager.hpp"
#include "server.hpp"
#include "client.hpp"
#include "inputHandler.hpp"

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

  std::vector<playerState> playerStateList;
  playerState p_state;
  inputHandler inputhandl;
  Server server;
  int listening_socket = 0;
  Client client;
  int client_socket = 0;
  int bytes = 0;
  CollisionManager mCollisionManager;

  static constexpr sf::Vector2u mWindowSize = { 960, 640 };

  sf::Clock mClock;
  sf::Time globalTime;
  
  // this sets the physical engine's tickrate to 60. this determines to how many calculates should be performed within a second.
  static constexpr unsigned int updatesPerSecond = 60; 
  static constexpr sf::Time timePerTick = sf::seconds(1.0f / updatesPerSecond);
  sf::Time simulationTime = sf::Time::Zero;

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
