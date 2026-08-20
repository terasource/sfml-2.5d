#pragma once
#include <SFML/Graphics.hpp>
#include "animations.hpp"
#include <math.h>
#include <iostream>
#include <map>
#include <cmath>
#include "collision.hpp"
#include "texturemanager.hpp"

class Player {
public:
    Player(TextureManager& m_tex_mngr, AnimationHandler& m_anim_handl);
    void update(sf::Time& dt, bool hasFocus);
    void draw(sf::RenderWindow& mWindow);
    void HandleInput(bool hasFocus);
    sf::Vector2f getPosition();
    void initialize_sprites_textures(); 
    //make these private back.
    sf::Vector2f movement;

private:
    void MovementAnimation(sf::Time& dt);
    void IdleAnimation();

public:

    sf::Sprite& mCharacterSprite;
    sf::Sprite& mCharacterHairSprite;
    sf::Sprite& mCharacterArmourSprite;

    TextureManager& mTextureManager;
  

    sf::Vector2f mDefaultPosition;
    sf::Vector2f mDefaultScale;

    AnimationHandler& mAnimationHandler;
    AnimationType& mAnimationType;
    float speed;
    
    
    DirectionType& mMovementDirection;
    sf::FloatRect GetCharacterHitbox();
    sf::Time dt;
};