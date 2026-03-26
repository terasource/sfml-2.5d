#pragma once
#include <SFML/Graphics.hpp>
#include "animations.hpp"
class Player {
public:
    Player();
    void update(sf::Time& dt, bool hasFocus);
    void draw(sf::RenderWindow& mWindow);
    void HandleInput(bool hasFocus);
    sf::Vector2f getPosition();

private:
    void MovementAnimation(sf::Time& dt);
    void IdleAnimation(sf::Time& dt);

    AnimationHandler mAnimationHandler;
    AnimationType mAnimationType;
    float speed;
    sf::Vector2f movement;
    sf::Vector2f mDefaultPosition;
    sf::Vector2f mDefaultScale;
    float mStopTimer;
    bool mWasMoving;
    bool mIsMoving;
    DirectionType mMovementDirection;
    sf::FloatRect GetCharacterHitbox();

};