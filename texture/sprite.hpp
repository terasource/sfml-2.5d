#pragma once
#include "texture/texture.hpp"
#include "texture/texturemanager.hpp"

class sprite : public sf::Drawable, public sf::Transformable {

public:
    sprite(const texture& texture);
    void draw(sf::RenderTarget& target, sf::RenderStates state) const;
    void set_texture(const sf::Texture& tex);
    void set_texture_rect(const sf::IntRect& rect);
    void set_scale(const sf::Vector2f& vec);
    void set_position(const sf::Vector2f& vec);
    sf::FloatRect get_global_bounds();
    sf::Vector2f  get_position();

private:
    sf::Sprite _spr;
};
