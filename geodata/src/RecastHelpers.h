#pragma once

#include "Recast.h"

namespace geodata {

static const unsigned char RC_STEEP_AREA = 0x1;
static const unsigned char RC_FLAT_AREA = 0x2;
static const unsigned char RC_COMPLEX_AREA = 0x3; // 0x3 - max possible value!

inline int unpack_area(int area) { return area & 0x3; }
inline int unpack_nswe(int area) { return area >> 2; }

void mark_walkable_triangles(float walkable_angle, const float *vertices,
                             const int *triangles, std::size_t triangle_count,
                             unsigned char *areas);

void calculate_nswe(const rcHeightfield &hf, int walkable_height,
                    int min_walkable_climb, int max_walkable_climb);

} // namespace geodata
