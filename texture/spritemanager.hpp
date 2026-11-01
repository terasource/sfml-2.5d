#pragma once
#include <SFML/Graphics.hpp>
#include "texture/sprite.hpp"

class SpriteManager {
private:
    std::unordered_map<std::string, sprite> sprites;
    TextureManager& texture_manager;

public:
    SpriteManager(TextureManager& tex_mngr);
    void initialize_sprites();
    sprite& get_sprite(const std::string& type);

    void move_all_sprites(sf::Vector2f movement);
};
