#pragma once
#include <SFML/Graphics.hpp>
#include <vector>


class Game {
public:
  Game();
  void run();

private:
  void processEvent();
  void update(sf::Time dt);
  void render();

  sf::Vector2f mWindowSize;
  sf::Clock mClock;
  sf::RenderWindow mWindow;
  sf::Texture mCharacterTexture;
  std::vector<sf::Texture> mTextures;
  sf::Sprite mCharacter;
  int currentFrame;
  float mAnimationTimer;
  float mAnimationSpeed;
  sf::View mView;
  sf::RectangleShape mBox;
  sf::Sprite mBackGround;
  sf::Texture mBackGroundTexture;
  float mStopTimer;
  float mWasMoving;
};
