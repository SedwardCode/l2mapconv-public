#pragma once

namespace geodata {

struct BuilderSettings {
  float actor_height;
  float actor_radius;
  float max_walkable_angle;
  float min_walkable_climb;
  float max_walkable_climb;
  float cell_size;
  float cell_height;

  explicit BuilderSettings(float actor_height, float actor_radius,
                           float max_walkable_angle, float min_walkable_climb,
                           float max_walkable_climb, float cell_size,
                           float cell_height)
      : actor_height{actor_height}, actor_radius{actor_radius},
        max_walkable_angle{max_walkable_angle},
        min_walkable_climb{min_walkable_climb},
        max_walkable_climb{max_walkable_climb}, cell_size{cell_size},
        cell_height{cell_height} {}
};

} // namespace geodata
