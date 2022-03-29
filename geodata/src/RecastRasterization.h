#pragma once

#include "Recast.h"

namespace geodata {

bool rasterize_triangles_with_sphere_collision(
    rcContext *ctx, const float *verts, const int /*nv*/, const int *tris,
    const unsigned char *areas, const int nt, rcHeightfield &solid,
    const int flagMergeThr);

} // namespace geodata
