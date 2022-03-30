#pragma once

namespace geodata {

struct BuilderSettings {
  float cell_size;
  float cell_height;
  float walkable_height;
  float walkable_angle;
  float min_walkable_climb;
  float max_walkable_climb;
};

} // namespace geodata
