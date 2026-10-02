#pragma once
#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <bit>

class PacketWriter {
private:
    std::vector<std::uint8_t>& buffer;


public:
    //PacketWriter() = default;
    PacketWriter(std::vector<std::uint8_t>& data);
    void write_uint8(std::uint8_t value);
    void write_uint32(std::uint32_t value);
    void write_uint32_at(int index, std::uint32_t value);
    void write_string(const std::string& value);
    void write_float(float value);
    const std::vector<std::uint8_t>& data() const;
};