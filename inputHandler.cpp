#include "inputHandler.hpp"


// movement flag = 
std::uint8_t inputHandler::collectInput(){
    
    std::uint8_t movementFlag = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {   
        movementFlag |= static_cast<std::uint8_t>(inputFlag::shift);
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        movementFlag |= static_cast<std::uint8_t>(inputFlag::up);
    } 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        movementFlag |= static_cast<std::uint8_t>(inputFlag::left);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
         movementFlag |= static_cast<std::uint8_t>(inputFlag::down);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        movementFlag |= static_cast<std::uint8_t>(inputFlag::right);
    }

    return movementFlag;
}