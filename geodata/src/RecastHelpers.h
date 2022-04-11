#pragma once

#include <geodata/Map.h>

#include "Recast.h"

namespace geodata {

static const unsigned char RC_STEEP_AREA = 0x1;
static const unsigned char RC_FLAT_AREA = 0x2;
static const unsigned char RC_COMPLEX_AREA = 0x3; // 0x3 - max possible value

inline auto unpack_area(int area) -> int { return area & 0x3; }
inline auto unpack_nswe(int area) -> int { return area >> 2; }

void build_heightfield(rcHeightfield &hf, const Map &map, float cell_size,
                       float cell_height, float walkable_height,
                       float walkable_angle);

void calculate_nswe(const rcHeightfield &hf, int walkable_height,
                    int min_walkable_climb, int max_walkable_climb);

} // namespace geodata
