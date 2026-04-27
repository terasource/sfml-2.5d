#include "animations.hpp"
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>


AnimationHandler::AnimationHandler() :
    mCharacterTexture(), mCharacterSprite(mCharacterTexture),
    mCharacterHairTexture(), mCharacterHairSprite(mCharacterHairTexture),
    mCharacterArmourTexture(), mCharacterArmourSprite(mCharacterArmourTexture),
    mCurrentFrame(0), mCurrentTime(0.f), mAnimationTime(0.5)
{
    if (!mCharacterTexture.loadFromFile("assets\\characters\\char_a_p1_0bas_humn_v00.png")) {
        std::cout << "Character texture cannot be loaded!" << std::endl;
    }
    if (!mCharacterHairTexture.loadFromFile("assets\\characters\\char_a_p1_4har_bob1_v03.png")) {
        std::cout << "Hair texture cannot be loaded!" << std::endl;
    }
    if (!mCharacterArmourTexture.loadFromFile("assets\\characters\\char_a_p1_1out_pfpn_v05.png")) {
        std::cout << "Armour texture cannot be loaded! " << std::endl;
    }
};

void AnimationHandler::AddAnimationSet(AnimationType aType, int startRow, int column, int width, int height, int startIndex) {
    //up
    AddAnimationFrame(aType, DirectionType::Up, (startRow + static_cast<int>(DirectionType::Up)), column, width, height, startIndex);
    //left 
    AddAnimationFrame(aType, DirectionType::Left, (startRow + static_cast<int>(DirectionType::Left)), column, width, height, startIndex);
    //down
    AddAnimationFrame(aType, DirectionType::Down, (startRow + static_cast<int>(DirectionType::Down)), column, width, height, startIndex);
    //right
    AddAnimationFrame(aType, DirectionType::Right, (startRow + static_cast<int>(DirectionType::Right)), column, width, height, startIndex);
};

void AnimationHandler::AddAnimationFrame(AnimationType aType, DirectionType dType, int row, int column, int width, int height, int startIndex) {
    for (int i = startIndex; i < column; i++) {
        animations[{aType, dType}].emplace_back(sf::Vector2i(i * width, row * height), sf::Vector2i(width, height));
    }
};

void AnimationHandler::UpdateAnimation(AnimationType type, DirectionType mMovementDirection) {
    if (animations[{type, mMovementDirection}].empty())
        std::cout << "animation is not valid";
    mCurrentFrame = 0;
    mCurrentTime = 0;
    const auto& currentRectangle = animations[{type, mMovementDirection}][mCurrentFrame];
    mCharacterSprite.setTextureRect(currentRectangle);
    mCharacterHairSprite.setTextureRect(currentRectangle);
    mCharacterArmourSprite.setTextureRect(currentRectangle);
};