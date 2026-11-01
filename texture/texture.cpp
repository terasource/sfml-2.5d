#include "texture.hpp"

texture::texture() {

};

bool texture::load(const std::string& path) {

    if (!_tex.loadFromFile(path)) {
        return false;
    }
    else {
        _tex_path = path;
        return true;
    }

}

const sf::Texture& texture::get_texture() const {
    return _tex;
}
