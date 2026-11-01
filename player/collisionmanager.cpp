#include "collisionmanager.hpp"

void CollisionManager::draw_screen_collision_borders(sf::RenderTarget& target){

    sf::RectangleShape borders({screen_width, screen_height});

    borders.setFillColor(sf::Color::Transparent);
    borders.setOutlineColor(sf::Color::Cyan);
    borders.setOutlineThickness(3.f);
    borders.setPosition({0,0});

    target.draw(borders); 
    //look about this how to draw something that is not affected by the camera view and stays the same place.
}
