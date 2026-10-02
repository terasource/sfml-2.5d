#pragma once
#include <string>
#include "input_types.hpp"

// wasd in the bitmask 0001111 will be ordered as up left down right
enum class DirectionType {
    none = -1,
    Up = 3,
    Left = 2,
    Down = 1,
    Right = 0
};

struct position{
    float x;
    float y;
};

class playerState{

    public:
        std::string name;
        unsigned int id;
        position playerpos;
        DirectionType direction;
    
    public:
    playerState() = default;
    playerState(unsigned int player_id, float pl_pos_x, float pl_pos_y);
    position& get_player_position();
    void set_player_position(position pos);
};