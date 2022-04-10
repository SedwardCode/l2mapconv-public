#pragma once

#include <cstdint>

namespace rendering {

struct FrameSettings {
  std::uint64_t surface_filter;
  std::uint64_t surface_textures;
  bool wireframe;
  bool culling;
};

} // namespace rendering
