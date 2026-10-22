#pragma once
#include <SFML/Graphics.hpp>
#include "animations.hpp"
#include <math.h>
#include <iostream>
#include <map>
#include <cmath>
#include "texturemanager.hpp"
#include "collider.hpp"
#include "playerState.hpp"



class Player {
public:
    Player(uint8_t id, playerState player_state, TextureManager& m_tex_mngr, AnimationHandler& m_anim_handl);
    void update(sf::Time& dt, bool hasFocus);
    void draw(sf::RenderWindow& mWindow);
    void applyState();
    sf::Vector2f getPosition();
    void initialize_sprites_textures();
    void MovementAnimation(sf::Time& dt);
    void IdleAnimation();
    void print_pos();
    void set_player_id(uint8_t player_id);
    int get_player_id() const;
    void set_player_state(playerState& player_state);
    playerState get_player_state();

    //i think members should be private and i need to get them with getter or set them with setter functions but i dont want to work on it rn and im not sure if this is the best approach to do it so.
    TextureManager& mTextureManager;
    collider<Player> mCollider;
    sf::Sprite mCharacterSprite;
    sf::Sprite mCharacterHairSprite;
    sf::Sprite mCharacterArmourSprite;

private:

    playerState localstate;

    sf::Vector2f mDefaultPosition;
    sf::Vector2f mDefaultScale;

    uint8_t my_character_id;

    AnimationHandler& mAnimationHandler;
    AnimationType& mAnimationType;
    float speed;
    sf::Vector2f movement;

    DirectionType& mMovementDirection;
    sf::FloatRect GetCharacterHitbox();
    sf::Time dt;
};