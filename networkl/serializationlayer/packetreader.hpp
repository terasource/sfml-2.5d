#pragma once
#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <bit>


class PacketReader {
private:
  const std::vector<std::uint8_t>& buffer;
  size_t offset = 0;

public:
  PacketReader(const std::vector<std::uint8_t>& data);
  int get_offset();

  std::uint8_t read_uint8();
  std::uint8_t read_uint8_at(std::size_t index);
  std::uint32_t read_uint32();
  std::uint32_t read_uint32_at(std::size_t index);
  std::string read_string(std::size_t len);
  float read_float();
};
