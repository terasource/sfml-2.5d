#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <cstdint>
#include "input_types.hpp"


class inputHandler{
    private:
        //std::uint8_t movementFlag = 0;

    public:
        std::uint8_t collectInput();
};