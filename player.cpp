#include "player.hpp"
#include <random>

//refactor with relative positions to charactor for its childs. sceneNode
//consider the player as entity and turn into a entity class.  
Player::Player(uint8_t id, playerState player_state, TextureManager& m_tex_mngr, AnimationHandler& m_anim_handl) :
    mDefaultPosition(300.f, 300.f), mDefaultScale({ 2.5f, 2.5f }),
    speed(300.f),
    mAnimationType(m_anim_handl.mAnimationType),
    mMovementDirection(m_anim_handl.mMovementDirection),
    mTextureManager(m_tex_mngr), mAnimationHandler(m_anim_handl),
    mCharacterArmourSprite(m_anim_handl.mCharacterArmourSprite),
    mCharacterHairSprite(m_anim_handl.mCharacterHairSprite),
    mCharacterSprite(m_anim_handl.mCharacterSprite)
{
    //texture should has its own manager. implement a texturemanager to load texture sprites etc.
    /*
       if (!mCharacterTexture.loadFromFile("assets/characters/char_a_p1_0bas_humn_v00.png")) {
        std::cout << "Character texture cannot be loaded!" << std::endl;
            }
        if (!mCharacterHairTexture.loadFromFile("assets/characters/char_a_p1_4har_bob1_v03.png")) {
        std::cout << "Hair texture cannot be loaded!" << std::endl;
            }
        if (!mCharacterArmourTexture.loadFromFile("assets/characters/char_a_p1_1out_pfpn_v05.png")) {
        std::cout << "Armour texture cannot be loaded! " << std::endl;
            }
    */

    //idle animation
    mAnimationHandler.AddAnimationSet(AnimationType::Idle, 0, 1, 64, 64); // 0 is startRow for idle animation, direction is down
    //walking animation
    mAnimationHandler.AddAnimationSet(AnimationType::Walk, 4, 6, 64, 64); // 4 is startRow for walkinganimation, direction is down
    //running animation
    int startIndex = 5;
    mAnimationHandler.AddAnimationSet(AnimationType::Run, 4, 8, 64, 64, startIndex); // 4 is startRow for runninganimation, direction is down, startColumn is 5

    set_player_id(id);
    set_player_state(player_state);
};

void Player::set_player_id(uint8_t player_id) {
    my_character_id = player_id;
}

int Player::get_player_id() const {
    return my_character_id;
}

void Player::initialize_sprites_textures() {

    //positions and scales
    //character
    mCharacterSprite.setScale(mDefaultScale);
    mCharacterSprite.setPosition(mDefaultPosition);
    //hair
    mCharacterHairSprite.setScale(mDefaultScale);
    //armour    
    mCharacterArmourSprite.setScale(mDefaultScale);

    mCharacterSprite.setTextureRect(mAnimationHandler.animations[{mAnimationType, mMovementDirection}][mAnimationHandler.mCurrentFrame]);
    mCharacterHairSprite.setTextureRect(mAnimationHandler.animations[{mAnimationType, mMovementDirection}][mAnimationHandler.mCurrentFrame]);
    mCharacterArmourSprite.setTextureRect(mAnimationHandler.animations[{mAnimationType, mMovementDirection}][mAnimationHandler.mCurrentFrame]);

    mCollider.set_bounds(mCharacterSprite.getGlobalBounds());
    mCollider.set_drawstate(true);
};



sf::FloatRect Player::GetCharacterHitbox() {
    auto pos = mCharacterSprite.getPosition();

    return sf::FloatRect({ pos.x + 20, pos.y + 25 }, { 20, 25 });
};


void Player::applyState() {

    auto pos = localstate.get_player_position();

    sf::Vector2f cpos{ pos.x, pos.y };

    //std::cout << "x: " << pos.x << "y: " << pos.y << std::endl;
    mCharacterSprite.setPosition(cpos);
    mCharacterArmourSprite.setPosition(cpos);
    mCharacterHairSprite.setPosition(cpos);
}


void Player::update(sf::Time& dt, bool hasFocus) {

    this->dt = dt;
    //applyState(hasFocus);
    IdleAnimation();
    applyState();
    auto characterPosition = mCharacterSprite.getPosition();
    //mTextureManager.setposition_all_sprites(characterPosition);
    mCollider.set_position(characterPosition);
    //mTextureManager.move_all_sprites(movement);

    //mCharacterSprite.move(movementFactor);    
    //separate this animationhandler only handle the animations not the main character sprite.

    //std::cout << "movement factor = {x: " << movementFactor.x << " y: "<< movementFactor.y << " }" <<std::endl;
    //std::cout << "character position  = {x: " << characterPosition.x << " y: "<< characterPosition.y << " }" <<std::endl;


    /*
    std::cout << "current frame; " << mAnimationHandler.mCurrentFrame << std::endl;
            std::cout << "animation type; " << typeid(mAnimationType).name() << std::endl;
            std::cout << "movement direction; " << typeid(mMovementDirection).name() << std::endl;
            std::cout << "animation vector size before; " << mAnimationHandler.animations[{mAnimationType, mMovementDirection}].size() << std::endl;
    */

    mAnimationHandler.MovementAnimation(dt);
};

void Player::draw(sf::RenderWindow& mWindow) {

    mWindow.draw(mCharacterSprite);
    mWindow.draw(mCharacterHairSprite);
    mWindow.draw(mCharacterArmourSprite);

    mWindow.draw(mCollider);
};

void Player::IdleAnimation() {
    if (!mAnimationHandler.mIsMoving && mAnimationHandler.mStopTimer > 0.5)
    {
        mAnimationType = AnimationType::Idle;
        mAnimationHandler.UpdateAnimation(mAnimationType, mMovementDirection);
    }
}

sf::Vector2f Player::getPosition() {
    auto pos = mCharacterSprite.getPosition();

    return { pos.x, pos.y };
}

void Player::print_pos() {
    std::cout << "character pos; " << "{" << getPosition().x << " ," << getPosition().y << " }" << std::endl;
}

void Player::set_player_state(playerState& player_state) {
    localstate = player_state;
}

playerState Player::get_player_state() {
    return localstate;
}