#include "pch.h"

#include "RecastHelpers.h"

namespace geodata {

inline int allow_direction(int area, int direction) {
  return area | 1 << (direction + 2);
}

inline int change_area(int area, int new_area) {
  return new_area | unpack_nswe(area) << 2;
}

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

    if (normal.y <= 0.0f) {
      areas[i] = RC_NULL_AREA;
    } else if (normal.y < walkable_angle_radians) {
      areas[i] = RC_STEEP_AREA;
    } else {
      areas[i] = RC_FLAT_AREA;
    }
  }
}

void calculate_nswe(const rcHeightfield &hf, int walkable_height,
                    int min_walkable_climb, int max_walkable_climb) {

  const auto max_height = 0xffff;

  for (auto y = 0; y < hf.height; ++y) {
    for (auto x = 0; x < hf.width; ++x) {
      for (auto *span = hf.spans[x + y * hf.width]; span != nullptr;
           span = span->next) {

        const auto bottom = static_cast<int>(span->smax);
        const auto top = span->next != nullptr
                             ? static_cast<int>(span->next->smin)
                             : max_height;

        for (auto direction = 0; direction < 4; ++direction) {
          const auto dx = x + rcGetDirOffsetX(direction);
          const auto dy = y + rcGetDirOffsetY(direction);

          if (dx < 0 || dy < 0 || dx >= hf.width || dy >= hf.height) {
            span->area = allow_direction(span->area, direction);
            continue;
          }

          auto direction_allowed = false;

          for (auto *neighbour = hf.spans[dx + dy * hf.width];
               neighbour != nullptr; neighbour = neighbour->next) {

            const auto neighbour_bottom = static_cast<int>(neighbour->smax);
            const auto neighbour_top =
                neighbour->next != nullptr
                    ? static_cast<int>(neighbour->next->smin)
                    : max_height;

            const auto height = std::min(top, neighbour_top) -
                                std::max(bottom, neighbour_bottom);
            const auto diff = neighbour_bottom - bottom;

            if (diff > min_walkable_climb && diff <= max_walkable_climb) {
              span->area = change_area(span->area, RC_COMPLEX_AREA);
            }

            if (height > walkable_height) {
              direction_allowed = diff <= max_walkable_climb;
              break;
            }
          }

          if (direction_allowed) {
            span->area = allow_direction(span->area, direction);
          }
        }
      }
    }
  }
}

} // namespace geodata
