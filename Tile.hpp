#pragma once

#include "SFML/Graphics.hpp"

class Tile {
public:
    Tile(sf::Texture& mTexture);

private:
    bool mCollision;
    sf::Sprite mTileSprite;
    int mType;

};

