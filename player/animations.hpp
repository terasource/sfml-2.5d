#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include "player/playerState.hpp"   

// turn into a template class for every animations.

enum class AnimationType {
    Idle,
    Walk,
    Run,
};

class AnimationHandler {
public:
    AnimationHandler();

    int mTotalFrames;
    size_t mCurrentFrameIndex;
    sf::IntRect mCurrentFrame;
    float mCurrentTime;
    float mAnimationTime;
    float mAnimationSpeed;

    AnimationType mAnimationType;
    DirectionType mMovementDirection;

    std::map <std::pair <AnimationType, DirectionType>, std::vector<sf::IntRect>> animations;

    void AddAnimationSet(AnimationType type, int startRow, int column, int width, int height, int startIndex = 0);
    void AddAnimationFrame(AnimationType aType, DirectionType dType, int row, int column, int width, int height, int startIndex = 0);
    void UpdateAnimation(AnimationType type, DirectionType movementDirection);

    sf::IntRect& MovementAnimation(sf::Time& dt, bool moveState);
    void UpdateAnimationStates(AnimationType animtype, DirectionType dirtype);
    float mStopTimer;
    bool mWasMoving;
    bool mIsMoving;

};
