#pragma once
#include "player.hpp"
#include "collider.hpp"
#include <iostream>
#include <algorithm>

class CollisionManager {

private:
    float screen_width = 960;
    float screen_height = 640;

    //later convert this into a tupple with reference_wrapper for different types.

    std::tuple<
        std::vector<std::reference_wrapper<collider<Player>>>

    > _colliderslist;

public:
    CollisionManager() = default;
    void draw_screen_collision_borders(sf::RenderTarget& target);

    template <typename T>
    void screen_collision(T& obj) {

        auto spr_bounds = obj.mCharacterSprite.getGlobalBounds();

        auto pos_x = spr_bounds.position.x;
        auto pos_y = spr_bounds.position.y;

        float threshold = 60.f;

        float vertical_threshold = spr_bounds.size.x;
        float horizontal_threshold = spr_bounds.size.y;

        // map x: 925; starts at -60 ends at 865 and y: 590; starts at -60 ends at 530;
        // idk why ill inspect it later. placeholder solution for now.

        //pos_x = std::clamp(pos_x, 0.f - threshold, screen_width - vertical_threshold);
        //pos_y = std::clamp(pos_y, 0.f - threshold, screen_height - horizontal_threshold);

        pos_x = std::clamp(pos_x, 0.f, screen_width - vertical_threshold);
        pos_y = std::clamp(pos_y, 0.f, screen_height - horizontal_threshold);



        //apply collision
        obj.mTextureManager.setposition_all_sprites(sf::Vector2f(pos_x, pos_y));
        obj.mCollider.set_position(sf::Vector2f(pos_x, pos_y));
    }

    template <typename T>
    void add_collider(collider<T>& _collider) {
        auto col_vec = std::get<std::vector<std::reference_wrapper<collider<T>>>>(_colliderslist);

        col_vec.push_back(_collider);
    }

};


