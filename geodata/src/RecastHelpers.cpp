#include "pch.h"

#include "RecastHelpers.h"

namespace geodata {

void mark_walkable_triangles(float walkable_angle, const float *vertices,
                             const int *triangles, std::size_t triangle_count,
                             unsigned char *areas) {

  const auto walkable_angle_radians = std::cos(glm::radians(walkable_angle));

  for (std::size_t i = 0; i < triangle_count; ++i) {
    const auto *triangle = &triangles[i * 3];
    const auto normal =
        glm::triangleNormal(glm::make_vec3(&vertices[triangle[0] * 3]),
                            glm::make_vec3(&vertices[triangle[1] * 3]),
                            glm::make_vec3(&vertices[triangle[2] * 3]));

    if (normal.y < walkable_angle_radians) {
      areas[i] = RC_NULL_AREA;
    } else {
      areas[i] = RC_WALKABLE_AREA;
    }
  }
}

} // namespace geodata
