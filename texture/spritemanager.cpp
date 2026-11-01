#include "spritemanager.hpp"

SpriteManager::SpriteManager(TextureManager& tex_mngr) :texture_manager(tex_mngr) {

}

void SpriteManager::initialize_sprites() {
    for (auto& [type, texture] : texture_manager.texture_list()) {
        sprite new_sprite(texture);
        sprites.insert_or_assign(type, new_sprite);
    }
}

sprite& SpriteManager::get_sprite(const std::string& type) {
    return sprites.at(type);
};

void SpriteManager::move_all_sprites(sf::Vector2f movement) {
    for (auto& [type, sprite] : sprites) {
        sprite.set_position(movement);
    }
}
