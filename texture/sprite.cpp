#include "sprite.hpp"
sprite::sprite(const texture& texture) : _spr(texture.get_texture()) {

}

void sprite::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    state.transform *= getTransform();

    target.draw(_spr, state);
}

void sprite::set_texture(const sf::Texture& tex) {
    _spr.setTexture(tex, true);
}

void sprite::set_texture_rect(const sf::IntRect& rect) {
    _spr.setTextureRect(rect);
}

void sprite::set_scale(const sf::Vector2f& vec) {
    _spr.setScale(vec);
};

void sprite::set_position(const sf::Vector2f& pos) {
    _spr.setPosition(pos);
}

sf::FloatRect sprite::get_global_bounds() {
    return _spr.getGlobalBounds();
}

sf::Vector2f sprite::get_position() {
    return _spr.getPosition();
}
