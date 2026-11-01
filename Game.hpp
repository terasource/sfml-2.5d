#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "player/player.hpp"
#include "map/TileMap.hpp"
#include "map/probs.hpp"
#include "map/mapLoader.hpp"
#include "player/collisionmanager.hpp"
#include "texture/texturemanager.hpp"
#include "networkl/server/server.hpp"
#include "networkl/client/client.hpp"
#include "networkl/client/inputHandler.hpp"
#include <bits/stdc++.h>
#include <algorithm>

class Game {
public:
  Game();
  void run();

private:
  void processEvent();
  void update(sf::Time dt);
  void render();

  TextureManager mTextureManager;
  SpriteManager mSpriteManager;

  TileMap map;
  TileAnimation animations;
  AnimationHandler mAnimationHandler;

  std::unordered_map<int, AnimationHandler> mAnimationHandlerList;

  Player* mainPlayer;
  std::unordered_map<int, Player> player_list;
  uint8_t client_player_id;
  std::vector<playerState> player_state_list;

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

  sf::Font mFont;
  sf::Text mFpsText;

  probs mProbs;
  bool moveState = false;
  std::string jsonpath = "assets//Sample map.json";

};
