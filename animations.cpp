#include "animations.hpp"
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>


AnimationHandler::AnimationHandler(TextureManager& m_tex_mngr) :
    mCurrentFrame(0), mCurrentTime(0.f), mAnimationTime(0.5),
    mAnimationType(AnimationType::Idle), mMovementDirection(DirectionType::Right),
    mTextureManager(m_tex_mngr),
    mCharacterSprite(mTextureManager.get_texture<_tex_type::body>()._spr), 
    mCharacterHairSprite(mTextureManager.get_texture<_tex_type::hair>()._spr), 
    mCharacterArmourSprite(mTextureManager.get_texture<_tex_type::armour>()._spr)
    {

  
    };

void AnimationHandler::load_sprites_textures(){
    
    auto& body_texture = mTextureManager.get_texture<_tex_type::body>();
    auto& hair_texture = mTextureManager.get_texture<_tex_type::hair>();
    auto& armour_texture = mTextureManager.get_texture<_tex_type::armour>();

    
    mTextureManager.assign_sprite_texture(body_texture);
        //std::cout << "tex_type; " << body_texture._tex_path << std::endl;
    mTextureManager.assign_sprite_texture(hair_texture);
        //std::cout << "tex_type; " << hair_texture._tex_path << std::endl;
    mTextureManager.assign_sprite_texture(armour_texture);
        //std::cout << "tex_type; " << armour_texture._tex_path << std::endl;

    mCharacterSprite = mTextureManager.get_sprite(body_texture);
    mCharacterHairSprite = mTextureManager.get_sprite(hair_texture);
    mCharacterArmourSprite = mTextureManager.get_sprite(armour_texture);

}   

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
    // mTextureManager.mCharacterSprite.setTextureRect(currentRectangle);
    // mTextureManager.mCharacterHairSprite.setTextureRect(currentRectangle);
    // mTextureManager.mCharacterArmourSprite.setTextureRect(currentRectangle);
    
    mTextureManager.set_texture_rect(mTextureManager.get_texture<_tex_type::body>(), currentRectangle); //but which sprite?
    mTextureManager.set_texture_rect(mTextureManager.get_texture<_tex_type::hair>(), currentRectangle);
    mTextureManager.set_texture_rect(mTextureManager.get_texture<_tex_type::armour>(), currentRectangle);
};


void AnimationHandler::MovementAnimation(sf::Time& dt) {
//this function should only be responsible for movementanimation but now it loads the sprite too.
    if (mIsMoving) {
        mStopTimer = 0;
        // mAnimationHandler.mAnimationSpeed = 40.f / speed;
        if (!mWasMoving) {
            mAnimationTime = 0;
            if (mCurrentFrame < animations[{mAnimationType, mMovementDirection}].size()) {
                mCurrentFrame++;
            }
        }
        else {

            mAnimationTime += dt.asSeconds();
            std::cout << "animation speed; " << mAnimationSpeed << " animation time; " << mAnimationTime << std::endl;

            if (mAnimationTime >= mAnimationSpeed) {
            //std::cout << "animation timer = " << mAnimationTime << std::endl;
                mAnimationTime = 0;
                mCurrentFrame++;
            }  

            /*
            std::cout << "current frame; " << mCurrentFrame << std::endl;
            std::cout << "animation type; " << typeid(mAnimationType).name() << std::endl;
            std::cout << "movement direction; " << typeid(mMovementDirection).name() << std::endl;
            std::cout << "animation vector size before; " << animations[{mAnimationType, mMovementDirection}].size() << std::endl;

            these are empty here because the animation sets are loaded in player constructor and player constructor is called after
            this file so this are now empty at the run time. 
            */
            
            if (mCurrentFrame >= animations[{mAnimationType, mMovementDirection}].size())
                mCurrentFrame = 0;

            if (mCurrentFrame < animations[{mAnimationType, mMovementDirection}].size()) {
                mCharacterSprite.setTextureRect(animations[{mAnimationType, mMovementDirection}][mCurrentFrame]);
                mTextureManager.set_texture_rect(mTextureManager.get_texture<_tex_type::body>(), animations[{mAnimationType, mMovementDirection}][mCurrentFrame]);
                //hair animation cycle
                //mCharacterHairSprite.setTextureRect(animations[{mAnimationType, mMovementDirection}][mCurrentFrame]);
                mTextureManager.set_texture_rect(mTextureManager.get_texture<_tex_type::hair>(), animations[{mAnimationType, mMovementDirection}][mCurrentFrame]);

                //armour animation cycle
                //mCharacterArmourSprite.setTextureRect(animations[{mAnimationType, mMovementDirection}][mCurrentFrame]);
                mTextureManager.set_texture_rect(mTextureManager.get_texture<_tex_type::armour>(), animations[{mAnimationType, mMovementDirection}][mCurrentFrame]);

            }
            
        }

    }
    else {

        mStopTimer += dt.asSeconds();
        if (mStopTimer >= 0.15)
            mCurrentFrame = 0;
        //thats why, these parts should not be there i guess. no actullay this are for the animation but i just need to predefine them one time before here. this is the first define thats why.

        if (mCurrentFrame < animations[{mAnimationType, mMovementDirection}].size()) {
            mTextureManager.set_texture_rect(mTextureManager.get_texture<_tex_type::body>(), animations[{mAnimationType, mMovementDirection}][mCurrentFrame]);
            mTextureManager.set_texture_rect(mTextureManager.get_texture<_tex_type::hair>(), animations[{mAnimationType, mMovementDirection}][mCurrentFrame]);
            mTextureManager.set_texture_rect(mTextureManager.get_texture<_tex_type::armour>(), animations[{mAnimationType, mMovementDirection}][mCurrentFrame]);
        }

    }

    mWasMoving = mIsMoving;
};
