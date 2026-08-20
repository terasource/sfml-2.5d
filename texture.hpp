#pragma once
#include "SFML/Graphics.hpp"
#include <string>
#include <iostream>
#include <memory>


template <typename T> 
class texture{
    public:
    sf::Texture _tex;
    sf::Sprite _spr;
    std::string _tex_path;

    texture() : _spr(_tex) {
        
    }
};