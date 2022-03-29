#pragma once

#include "Recast.h"

namespace geodata {

void mark_walkable_triangles(float walkable_angle, const float *vertices,
                             const int *triangles, std::size_t triangle_count,
                             unsigned char *areas);

} // namespace geodata
