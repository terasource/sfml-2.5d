#include "player.hpp"
#include "animations.hpp"
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include <map>

Player::Player() :
    mDefaultPosition(300.f, 300.f), mDefaultScale({ 2.5f, 2.5f }),
    speed(300.f),
    mAnimationType(AnimationType::Idle),
    mMovementDirection(DirectionType::Right) {


    //idle animation
    mAnimationHandler.AddAnimationSet(AnimationType::Idle, 0, 1, 64, 64); // 0 is startRow for idle animation, direction is down
    //walking animation
    mAnimationHandler.AddAnimationSet(AnimationType::Walk, 4, 6, 64, 64); // 4 is startRow for walkinganimation, direction is down
    //running animation
    int startIndex = 5;
    mAnimationHandler.AddAnimationSet(AnimationType::Run, 4, 8, 64, 64, startIndex); // 4 is startRow for runninganimation, direction is down, startColumn is 5


    //positions and scales
    //character
    mAnimationHandler.mCharacterSprite.setScale(mDefaultScale);
    mAnimationHandler.mCharacterSprite.setPosition(mDefaultPosition);
    //hair
    mAnimationHandler.mCharacterHairSprite.setScale(mDefaultScale);
    //armour
    mAnimationHandler.mCharacterArmourSprite.setScale(mDefaultScale);

};


sf::FloatRect Player::GetCharacterHitbox() {
    auto pos = mAnimationHandler.mCharacterSprite.getPosition();

    return sf::FloatRect({ pos.x + 20, pos.y + 25 }, { 15, 15 });
};
void Player::update(sf::Time& dt, bool hasFocus) {

    HandleInput(hasFocus);

    IdleAnimation(dt);

    auto movementFactor = movement * dt.asSeconds();
    auto characterPosition = mAnimationHandler.mCharacterSprite.getPosition();

    mAnimationHandler.mCharacterSprite.move(movementFactor);
    mAnimationHandler.mCharacterHairSprite.setPosition(characterPosition);
    mAnimationHandler.mCharacterArmourSprite.setPosition(characterPosition);
    MovementAnimation(dt);

};

void Player::draw(sf::RenderWindow& mWindow) {
    mWindow.draw(mAnimationHandler.mCharacterSprite);
    mWindow.draw(mAnimationHandler.mCharacterHairSprite);
    mWindow.draw(mAnimationHandler.mCharacterArmourSprite);
};

void Player::HandleInput(bool hasFocus) {

    mIsMoving = false;
    movement = { 0,0 };

    if (!hasFocus) {
        movement = { 0,0 };
        mIsMoving = false;
        AnimationType::Idle;

        return;
    }


    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
        mAnimationType = AnimationType::Walk;
        speed = 250.f;
        mAnimationHandler.mAnimationSpeed = 0.1f;
    }
    else {
        speed = 400.f;
        mAnimationHandler.mAnimationSpeed = 0.15f;
        if (!(mAnimationType == AnimationType::Run)) {
            mAnimationType = AnimationType::Run;
            mAnimationHandler.UpdateAnimation(mAnimationType, mMovementDirection);
        }
    }


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        movement.y -= speed;
        mMovementDirection = DirectionType::Up;
        mIsMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        movement.x -= speed;
        mMovementDirection = DirectionType::Left;
        mIsMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        movement.y += speed;
        mMovementDirection = DirectionType::Down;
        mIsMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        movement.x += speed;
        mMovementDirection = DirectionType::Right;
        mIsMoving = true;
    }


    float magnitude = std::sqrt(movement.x * movement.x + movement.y * movement.y);
    if (magnitude > 0) {
        movement /= magnitude;
        movement *= speed;
    }
}
void Player::MovementAnimation(sf::Time& dt) {

    if (mIsMoving) {
        mStopTimer = 0;
        // mAnimationHandler.mAnimationSpeed = 40.f / speed;
        if (!mWasMoving) {
            mAnimationHandler.mAnimationTime = 0;
            if (mAnimationHandler.mCurrentFrame < mAnimationHandler.animations[{mAnimationType, mMovementDirection}].size()) {
                mAnimationHandler.mCurrentFrame++;
            }
        }
        else {
            mAnimationHandler.mAnimationTime += dt.asSeconds();
            //std::cout << "animation timer = " << mAnimationHandler.mAnimationTime << std::endl;
            //std::cout << "animation speed = " << mAnimationHandler.mAnimationSpeed << std::endl;
            if (mAnimationHandler.mAnimationTime >= mAnimationHandler.mAnimationSpeed) {
                mAnimationHandler.mAnimationTime = 0;
                mAnimationHandler.mCurrentFrame++;
            }

            if (mAnimationHandler.mCurrentFrame >= mAnimationHandler.animations[{mAnimationType, mMovementDirection}].size())
                mAnimationHandler.mCurrentFrame = 0;

            if (mAnimationHandler.mCurrentFrame < mAnimationHandler.animations[{mAnimationType, mMovementDirection}].size()) {
                mAnimationHandler.mCharacterSprite.setTextureRect(mAnimationHandler.animations[{mAnimationType, mMovementDirection}][mAnimationHandler.mCurrentFrame]);
                //hair animation cycle
                mAnimationHandler.mCharacterHairSprite.setTextureRect(mAnimationHandler.animations[{mAnimationType, mMovementDirection}][mAnimationHandler.mCurrentFrame]);
                //armour animation cycle
                mAnimationHandler.mCharacterArmourSprite.setTextureRect(mAnimationHandler.animations[{mAnimationType, mMovementDirection}][mAnimationHandler.mCurrentFrame]);

            }
        }

    }
    else {

        mStopTimer += dt.asSeconds();
        if (mStopTimer >= 0.15)
            mAnimationHandler.mCurrentFrame = 0;

        if (mAnimationHandler.mCurrentFrame < mAnimationHandler.animations[{mAnimationType, mMovementDirection}].size()) {
            mAnimationHandler.mCharacterSprite.setTextureRect(mAnimationHandler.animations[{mAnimationType, mMovementDirection}][mAnimationHandler.mCurrentFrame]);
            mAnimationHandler.mCharacterHairSprite.setTextureRect(mAnimationHandler.animations[{mAnimationType, mMovementDirection}][mAnimationHandler.mCurrentFrame]);
            mAnimationHandler.mCharacterArmourSprite.setTextureRect(mAnimationHandler.animations[{mAnimationType, mMovementDirection}][mAnimationHandler.mCurrentFrame]);
        }

    }

    mWasMoving = mIsMoving;
};

void Player::IdleAnimation(sf::Time& dt) {
    if (!mIsMoving && mStopTimer > 0.5)
    {
        mAnimationType = AnimationType::Idle;
        mAnimationHandler.UpdateAnimation(mAnimationType, mMovementDirection);
    }
}

sf::Vector2f Player::getPosition() {
    auto pos = mAnimationHandler.mCharacterSprite.getPosition();

    return { pos.x, pos.y };
}


