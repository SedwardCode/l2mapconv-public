#pragma once

#include <geodata/ExportBuffer.h>

#include <utility>

namespace geodata {

class Compressor {
public:
  explicit Compressor(ExportBuffer &buffer);

  void compress();

private:
  ExportBuffer &m_buffer;

  auto is_multilayer_block(int x, int y) const -> bool;
  auto is_simple_block(int x, int y, std::int16_t &new_z) const -> bool;
};

} // namespace geodata
