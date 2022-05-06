#pragma once

#include <cstdlib>
#include <vector>

#include <geodata/Map.h>
#include <geometry/Sphere.h>
#include <geometry/Triangle.h>

#include "Recast.h"

namespace geodata {

static constexpr unsigned char RC_STEEP_AREA = 0x1;
static constexpr unsigned char RC_FLAT_AREA = 0x2;
static constexpr unsigned char RC_COMPLEX_AREA =
    0x3; // 0x3 - max possible value

inline auto unpack_area(int area) -> int { return area & 0x3; }
inline auto unpack_nswe(int area) -> int { return area >> 2; }

class NSWE {
public:
  explicit NSWE(const Map &map, float cell_size, float cell_height,
                float walkable_height, float walkable_angle,
                float min_walkable_climb, float max_walkable_climb);

  ~NSWE();

  auto calculate_nswe() -> const rcHeightfield &;

private:
  const Map &m_map;

  const float m_cell_size;
  const float m_cell_height;
  const float m_walkable_height;
  const float m_walkable_angle_radians;
  const float m_min_walkable_climb;
  const float m_max_walkable_climb;

  rcHeightfield *m_hf;
  std::vector<std::vector<int>> m_triangle_index;

  // Build heightfield and filter walkable low-height spans
  void build_filtered_heightfield();
  void mark_walkable_triangles(const float *vertices, const int *triangles,
                               std::size_t triangle_count,
                               unsigned char *areas) const;

  // Calculate NSWE based on the height difference of the neighboring spans and
  // mark some areas as RC_COMPLEX_AREA, on which we'll use
  // calculate_complex_nswe
  void calculate_simple_nswe();

  // Calculate NSWE based on sphere-to-mesh collision, must be called after
  // calculate_simple_nswe
  void calculate_complex_nswe();
  auto slide_sphere_until_collision(int x, int y, int z, int direction) const
      -> bool;
  void drop_sphere(geometry::Sphere &sphere,
                   const std::vector<geometry::Triangle> &triangles) const;
  auto triangles_at_columns(int x, int y, int radius) const
      -> std::vector<geometry::Triangle>;
};

} // namespace geodata
