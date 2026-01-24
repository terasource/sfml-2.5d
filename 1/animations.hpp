#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <vector>

enum class DirectionType {
    Up = 1,
    Left = 3,
    Down = 0,
    Right = 2
};

enum class AnimationType {
    Idle,
    Walk,
    Run,
};

class AnimationHandler {
public:

    AnimationHandler();

    AnimationType mAnimationType;
    sf::Sprite mCharacterSprite;
    sf::Texture mCharacterTexture;
    sf::Sprite mCharacterHairSprite;
    sf::Texture mCharacterHairTexture;
    sf::Sprite mCharacterArmourSprite;
    sf::Texture mCharacterArmourTexture;

    int mCurrentFrame;
    int mTotalFrames;
    float mCurrentTime;
    float mAnimationTime;
    float mAnimationSpeed;

    std::map <std::pair <AnimationType, DirectionType>, std::vector<sf::IntRect>> animations;

    void AddAnimationSet(AnimationType type, int startRow, int column, int width, int height, int startIndex = 0);
    void AddAnimationFrame(AnimationType aType, DirectionType dType, int row, int column, int width, int height, int startIndex = 0);
    void UpdateAnimation(AnimationType type, DirectionType movementDirection);
};
