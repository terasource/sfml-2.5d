#include "animations.hpp"
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>


AnimationHandler::AnimationHandler() :
    mCurrentFrameIndex(0), mCurrentTime(0.f), mAnimationTime(0.5),
    mAnimationType(AnimationType::Walk),
    mMovementDirection(DirectionType::Right)
{


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
    mCurrentFrameIndex = 0;
    mCurrentTime = 0;
    //const auto& currentRectangle = animations[{type, mMovementDirection}][mCurrentFrameIndex];

};

void AnimationHandler::UpdateAnimationStates(AnimationType animtype, DirectionType dirtype) {
    mAnimationType = animtype;
    mMovementDirection = dirtype;
}


//refactor this dumbass logic.
sf::IntRect& AnimationHandler::MovementAnimation(sf::Time& dt, bool moveState) {
    //this function should only be responsible for movementanimation but now it loads the sprite too.

    if (moveState) {}

    if (1) {

        mStopTimer = 0;
        mAnimationTime = 0;

        mAnimationTime += dt.asSeconds();
        if (mAnimationTime >= mAnimationSpeed) {
            mAnimationTime = 0;
            mCurrentFrameIndex++;
        }

        if (mCurrentFrameIndex >= animations[{mAnimationType, mMovementDirection}].size())
            mCurrentFrameIndex = 0;

        if (mCurrentFrameIndex < animations[{mAnimationType, mMovementDirection}].size()) {
            mCurrentFrame = animations[{mAnimationType, mMovementDirection}][mCurrentFrameIndex];
        }

    }
    else {

        //there is no animation cycle for iddle because i have no iddle animation now.
        //maybe i will have in the future so there will have to be modified
        mStopTimer += dt.asSeconds();

        if (mStopTimer >= 0.15)
            mCurrentFrameIndex = 0;

        if (mCurrentFrameIndex >= animations[{mAnimationType, mMovementDirection}].size())
            mCurrentFrameIndex = 0;

        if (mCurrentFrameIndex < animations[{mAnimationType, mMovementDirection}].size()) {
            mCurrentFrame = animations[{mAnimationType, mMovementDirection}][mCurrentFrameIndex];
        }

    }

    return animations[{mAnimationType, mMovementDirection}][mCurrentFrameIndex];
};
