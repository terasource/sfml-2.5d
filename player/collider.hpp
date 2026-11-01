#pragma once
#include <SFML/Graphics.hpp>

template<typename T>
class collider : public sf::Drawable, public sf::Transformable {

private:
    sf::RectangleShape collider_bounds;
    sf::Color collider_color = sf::Color::Magenta;
    bool isDrawed = false;
    bool isFilled = false;

public:
    collider() = default;

    collider(sf::FloatRect rect) {
        collider_bounds = sf::RectangleShape({ rect.size.x, rect.size.y });
    }



    void set_drawstate(bool state);
    bool get_drawstate() const;

    void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
    void set_position(sf::Vector2f vec);
    void set_bounds(sf::FloatRect rect);
    //not in use actually i dont need rn but maybe i need in future so keep it like this and maybe think to convert this into a interface for other classes too.
    sf::Vector2f get_bounds();
};

// collider.cpp

//this wrapper takes parameteres but i dont want it to take parameters actually. 
//i just want to player.collider.draw(); and the collider will be drew at the 
//render target of player which is its parent.



template<typename T>
void collider<T>::draw(sf::RenderTarget& target, sf::RenderStates state) const {

    state.transform *= getTransform();

    //sf::RectangleShape border({collider_bounds.getGlobalBounds().size.x, collider_bounds.getGlobalBounds().size.y});

    sf::RectangleShape border = collider_bounds;

    border.setOutlineColor(collider_color);
    border.setOutlineThickness(3.f);

    if (isFilled)
        border.setFillColor(collider_color);
    else
        border.setFillColor(sf::Color::Transparent);


    if (get_drawstate())
        target.draw(border, state);

    else return;
}

template <typename T>
void collider<T>::set_drawstate(bool state) {
    isDrawed = state;
}


template <typename T>
bool collider<T>::get_drawstate() const {

    return isDrawed;
}

template <typename T>
void collider<T>::set_position(sf::Vector2f vec) {
    collider_bounds.setPosition(vec);
}

template <typename T>
void collider<T>::set_bounds(sf::FloatRect rect) {
    collider_bounds.setSize(rect.size);
}

template <typename T>
sf::Vector2f collider<T>::get_bounds() {
    return collider_bounds.getGlobalBounds().size;
}