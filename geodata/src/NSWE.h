#pragma once

#include <geodata/Map.h>

#include "Recast.h"

namespace geodata {

static constexpr unsigned char RC_STEEP_AREA = 0x1;
static constexpr unsigned char RC_FLAT_AREA = 0x2;
static constexpr unsigned char RC_COMPLEX_AREA =
    0x3; // 0x3 - max possible value

inline auto unpack_area(int area) -> int { return area & 0x3; }
inline auto unpack_nswe(int area) -> int { return area >> 2; }

// Build heightfield and filter walkable low-height spans
void build_filtered_heightfield(rcHeightfield &hf, const Map &map,
                                float cell_size, float cell_height,
                                float walkable_height, float walkable_angle);

// Calculate NSWE based on the height difference of the neighboring spans and
// mark some areas as RC_COMPLEX_AREA, on which we'll use calculate_complex_nswe
void calculate_simple_nswe(const rcHeightfield &hf, float cell_height,
                           float walkable_height, float min_walkable_climb,
                           float max_walkable_climb);

// Calculate NSWE based on sphere-to-mesh collision, must be called after
// calculate_simple_nswe
void calculate_complex_nswe(const rcHeightfield &hf, const Map &map,
                            float cell_size, float cell_height);

} // namespace geodata
