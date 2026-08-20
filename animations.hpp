#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include "texturemanager.hpp"

// turn into a template class for every animations.

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
    AnimationHandler(TextureManager& m_tex_mngr);
    
    AnimationType mAnimationType;
    DirectionType mMovementDirection;

    TextureManager& mTextureManager;

    sf::Sprite& mCharacterSprite;
    sf::Sprite& mCharacterHairSprite;
    sf::Sprite& mCharacterArmourSprite;

    int mCurrentFrame;
    int mTotalFrames;
    float mCurrentTime;
    float mAnimationTime;
    float mAnimationSpeed;

    std::map <std::pair <AnimationType, DirectionType>, std::vector<sf::IntRect>> animations;

    void AddAnimationSet(AnimationType type, int startRow, int column, int width, int height, int startIndex = 0);
    void AddAnimationFrame(AnimationType aType, DirectionType dType, int row, int column, int width, int height, int startIndex = 0);
    void UpdateAnimation(AnimationType type, DirectionType movementDirection);

    void MovementAnimation(sf::Time& dt);

    void load_sprites_textures();
    
    float mStopTimer;
    bool mWasMoving;
    bool mIsMoving;
    
};
