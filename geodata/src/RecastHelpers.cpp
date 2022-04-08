#include "pch.h"

#include "RecastHelpers.h"

namespace geodata {

void mark_walkable_triangles(float walkable_angle, const float *vertices,
                             const int *triangles, std::size_t triangle_count,
                             unsigned char *areas);

inline auto allow_direction(int area, int direction) -> int {
  return area | 1 << (direction + 2);
}

inline auto change_area(int area, int new_area) -> int {
  return new_area | unpack_nswe(area) << 2;
}

void build_heightfield(rcHeightfield &hf, const Map &map, float cell_size,
                       float cell_height, float walkable_angle) {

  const auto *bb_min = glm::value_ptr(map.flipped_bounding_box().min());
  const auto *bb_max = glm::value_ptr(map.flipped_bounding_box().max());

  // Grid size
  auto width = 0;
  auto height = 0;
  rcCalcGridSize(bb_min, bb_max, cell_size, &width, &height);

  // Create heightfield
  rcContext context{};
  rcCreateHeightfield(&context, hf, width, height, bb_min, bb_max, cell_size,
                      cell_height);

  // Prepare geometry data
  const auto *vertices = glm::value_ptr(map.vertices().front());
  const auto vertex_count = map.vertices().size();
  const auto *triangles = reinterpret_cast<const int *>(map.indices().data());
  const auto triangle_count = map.indices().size() / 3;

  // Rasterize triangles
  std::vector<unsigned char> areas(triangle_count);
  mark_walkable_triangles(walkable_angle, vertices, triangles, triangle_count,
                          &areas.front());
  rcRasterizeTriangles(&context, vertices, vertex_count, triangles,
                       &areas.front(), triangle_count, hf, 0);
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
