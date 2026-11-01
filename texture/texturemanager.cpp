#include "SFML/Graphics.hpp"
#include "texturemanager.hpp"
#include <iostream> 
#include <string> 

//lazy loading - load the memory when it is only requested for first time and create a counter for its users.  //               if any sprite uses the texture, delete it. can done with smart pointers probably.  
TextureManager::TextureManager() {
    initialize_textures();
}

void TextureManager::initialize_textures() {

    load_texture("body::texture::base", "assets/characters/char_a_p1_0bas_humn_v00.png");

    load_texture("armour::texture::base", "assets/characters/char_a_p1_4har_bob1_v03.png");

    load_texture("hair::texture::base", "assets/characters/char_a_p1_1out_pfpn_v05.png");

}

bool TextureManager::load_texture(const std::string& type, const std::string& path) {
    texture texture;

    if (!texture.load(path)) {
        std::cout << "texture could not be loaded!" << std::endl;
        return false;
    }
    else {
        textures.insert({ type, texture });
        return true;
    }


};

const sf::Texture& TextureManager::get_texture(const std::string& type) const {
    return textures.at(type).get_texture();
};

const std::unordered_map<std::string, texture>& TextureManager::texture_list() const {
    return textures;
}

