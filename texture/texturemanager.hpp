#pragma once
#include "texture/texture.hpp"
#include <string>
#include <any>

class TextureManager {
private:

    std::unordered_map<std::string, texture> textures;

public:
    TextureManager();

    bool load_texture(const std::string& type, const std::string& path);
    const sf::Texture& get_texture(const std::string& type) const;
    const std::unordered_map<std::string, texture>& texture_list() const;
    void initialize_textures();
    void change_texture(texture texture, sf::Texture& tex) noexcept;
};