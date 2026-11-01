#pragma once
#include <string>
#include "networkl/client/input_types.hpp"

enum class DirectionType {
    none = -1,
    Up = 1,
    Left = 3,
    Down = 0,
    Right = 2
};

struct position {
    float x;
    float y;
};

class playerState {

public:
    std::string name;
    uint8_t id;
    position playerpos;
    DirectionType direction;

public:
    playerState() = default;
    playerState(unsigned int player_id, float pl_pos_x, float pl_pos_y);
    position& get_player_position();
    void set_player_position(position pos);
};