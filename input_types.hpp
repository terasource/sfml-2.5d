#pragma once
#include <cstdint>

enum class inputFlag : std::uint8_t {
    shift = 1 << 4,
    up = 1 << 3,
    left = 1 << 2,
    down = 1 << 1,
    right = 1 << 0
};

struct inputState {
    std::uint8_t movementFlag = 0;
    std::uint32_t sequence = 0;
};