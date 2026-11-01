#include "player.hpp"
#include <random>

//refactor with relative positions to charactor for its childs. sceneNode
//consider the player as entity and turn into a entity class.  
Player::Player(uint8_t id, playerState player_state, SpriteManager& m_spr_mngr, AnimationHandler& m_anim_handl) :
    mSpriteManager(m_spr_mngr), mAnimationHandler(m_anim_handl),
    mDefaultPosition(300.f, 300.f), mDefaultScale({ 2.5f, 2.5f }),
    speed(300.f),
    mAnimationType(m_anim_handl.mAnimationType),
    mMovementDirection(m_anim_handl.mMovementDirection),
    mCharacterSprite(mSpriteManager.get_sprite("body::texture::base")),
    mCharacterHairSprite(mSpriteManager.get_sprite("hair::texture::base")),
    mCharacterArmourSprite(mSpriteManager.get_sprite("armour::texture::base"))
{
    mCharacterSprite.set_texture_rect(mAnimationHandler.mCurrentFrame);
    mCharacterHairSprite.set_texture_rect(mAnimationHandler.mCurrentFrame);
    mCharacterArmourSprite.set_texture_rect(mAnimationHandler.mCurrentFrame);

    mCharacterSprite.set_scale({ 1.5,1.5 });
    mCharacterHairSprite.set_scale({ 1.5,1.5 });
    mCharacterArmourSprite.set_scale({ 1.5,1.5 });

    mCollider.set_bounds(sf::FloatRect({ 0,0 }, { 96.f,96.f }));
    mCollider.set_drawstate(true);

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


sf::FloatRect Player::GetCharacterHitbox() {
    auto pos = mCharacterSprite.get_position();

    return sf::FloatRect({ pos.x + 20, pos.y + 25 }, { 20, 25 });
};


void Player::applyState() {

    auto pos = localstate.get_player_position();

    sf::Vector2f cpos{ pos.x, pos.y };

    //std::cout << "x: " << pos.x << "y: " << pos.y << std::endl;
    mCharacterSprite.set_position(cpos);
    mCharacterArmourSprite.set_position(cpos);
    mCharacterHairSprite.set_position(cpos);
}

void Player::set_movestate(bool moveState) {
    this->moveState = moveState;
}


void Player::update(sf::Time& dt) {

    this->dt = dt;
    //applyState(hasFocus);
    //IdleAnimation();
    applyState();
    auto characterPosition = mCharacterSprite.get_position();
    mCollider.set_position(characterPosition);


    auto mCurrentFrame = mAnimationHandler.MovementAnimation(dt, moveState);

    mCharacterSprite.set_texture_rect(mCurrentFrame);
    mCharacterArmourSprite.set_texture_rect(mCurrentFrame);
    mCharacterHairSprite.set_texture_rect(mCurrentFrame);
};

void Player::draw(sf::RenderWindow& mWindow) {

    mWindow.draw(mCharacterSprite);
    mWindow.draw(mCharacterHairSprite);
    mWindow.draw(mCharacterArmourSprite);

    mWindow.draw(mCollider);
};


std::pair<AnimationType, DirectionType> Player::getAnimState(const std::pair<playerState, playerState>& states) {
    float delta_x = states.second.playerpos.x - states.first.playerpos.x;
    float delta_y = states.second.playerpos.y - states.first.playerpos.y;

    if (delta_x > 0)
        std::cout << "states second = " << states.second.playerpos.x << "states first = " << states.first.playerpos.x << std::endl;

    //delta possibilites. 0idle 10walk 20run.
    //std::cout << "delta x =" << delta_x << std::endl;
    //std::cout << "delta y =" << delta_y << std::endl;

    DirectionType dirtype = DirectionType::Left;
    AnimationType animtype = AnimationType::Idle;

    //directiontype:

    if (abs(delta_x) >= abs(delta_y)) {
        dirtype = ((delta_x > 0.0) ? DirectionType::Right : DirectionType::Left);
    }
    else {

        dirtype = ((delta_y > 0.0) ? DirectionType::Down : DirectionType::Up);
    }

    //animationtype:

    if (abs(delta_x) > 10.0 || abs(delta_y) > 10.0) {
        //std::cout << "running anim state " << std::endl;
        animtype = AnimationType::Run;
    }
    else if (delta_x == 0 && delta_y == 0) {
        //std::cout << "idle anim state" << std::endl;
        animtype = AnimationType::Idle;
    }
    else if (abs(delta_x) == 10.0 || abs(delta_y) == 10.0) {
        //std::cout << "walking anim state " << std::endl;
        animtype = AnimationType::Walk;
    }


    return { animtype, dirtype };
}

void Player::setAnimState(const std::pair<AnimationType, DirectionType>& states) {
    mAnimationType = states.first;
    mMovementDirection = states.second;
    mAnimationHandler.UpdateAnimationStates(states.first, states.second);
}

void Player::IdleAnimation() {
    if (!mAnimationHandler.mIsMoving && mAnimationHandler.mStopTimer > 0.5)
    {
        mAnimationType = AnimationType::Idle;
        mAnimationHandler.UpdateAnimation(mAnimationType, mMovementDirection);
    }
}

sf::Vector2f Player::getPosition() {
    auto pos = mCharacterSprite.get_position();

    return { pos.x, pos.y };
}

void Player::set_player_state(playerState& player_state) {
    localstate = player_state;
}

playerState Player::get_player_state() {
    return localstate;
}