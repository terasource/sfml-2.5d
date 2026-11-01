#pragma once
#include <SFML/Graphics.hpp>
#include "player/animations.hpp"
#include <math.h>
#include <iostream>
#include <map>
#include <cmath>
#include "texture/texturemanager.hpp"
#include "collider.hpp"
#include "playerState.hpp"
#include "texture/spritemanager.hpp"


class Player {
public:
    Player(uint8_t id, playerState player_state, SpriteManager& m_spr_mngr, AnimationHandler& m_anim_handl);
    void update(sf::Time& dt);
    void draw(sf::RenderWindow& mWindow);
    void applyState();
    void set_movestate(bool moveState);
    sf::Vector2f getPosition();
    void MovementAnimation(sf::Time& dt);
    void IdleAnimation();
    void set_player_id(uint8_t player_id);
    int get_player_id() const;
    void set_player_state(playerState& player_state);
    playerState get_player_state();
    std::pair<AnimationType, DirectionType> getAnimState(const std::pair<playerState, playerState>& states);
    void setAnimState(const std::pair<AnimationType, DirectionType>& states);

private:
    SpriteManager& mSpriteManager;
    AnimationHandler& mAnimationHandler;

    sf::Vector2f mDefaultPosition;
    sf::Vector2f mDefaultScale;

    float speed;

    AnimationType& mAnimationType;
    DirectionType& mMovementDirection;

    sprite mCharacterSprite;
    sprite mCharacterHairSprite;
    sprite mCharacterArmourSprite;

    collider<Player> mCollider;
    playerState localstate;


    sf::Vector2f movement;
    uint8_t my_character_id;
    bool moveState = false;


    sf::FloatRect GetCharacterHitbox();
    sf::Time dt;
};