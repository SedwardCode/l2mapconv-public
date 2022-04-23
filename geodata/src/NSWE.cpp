#include "pch.h"

#include "NSWE.h"

namespace geodata {

void mark_walkable_triangles(float walkable_angle, const float *vertices,
                             const int *triangles, std::size_t triangle_count,
                             unsigned char *areas);

auto triangles_at_columns(const rcHeightfield &hf, const Map &map,
                          const std::vector<std::vector<int>> &triangle_index,
                          int x, int y, int radius = 0)
    -> std::vector<geometry::Triangle>;

inline auto allow_direction(int area, int direction) -> int {
  return area | 1 << (direction + 2);
}

inline auto forbid_direction(int area, int direction) -> int {
  return area & ~(1 << (direction + 2));
}

inline auto change_area(int area, int new_area) -> int {
  return new_area | unpack_nswe(area) << 2;
}

NSWE::NSWE(const Map &map, float cell_size, float cell_height,
           float walkable_height, float walkable_angle,
           float min_walkable_climb, float max_walkable_climb)
    : m_map{map}, m_cell_size{cell_size}, m_cell_height{cell_height},
      m_walkable_height{walkable_height}, m_walkable_angle{walkable_angle},
      m_min_walkable_climb{min_walkable_climb}, m_max_walkable_climb{
                                                    max_walkable_climb} {

  m_hf = rcAllocHeightfield();

  build_filtered_heightfield();
}

NSWE::~NSWE() { rcFreeHeightField(m_hf); }

auto NSWE::calculate_nswe() -> const rcHeightfield & {

  calculate_simple_nswe();
  calculate_complex_nswe();

  return *m_hf;
}

void NSWE::build_filtered_heightfield() {

  const auto *bb_min = glm::value_ptr(m_map.flipped_bounding_box().min());
  const auto *bb_max = glm::value_ptr(m_map.flipped_bounding_box().max());

  // Grid size
  auto width = 0;
  auto height = 0;
  rcCalcGridSize(bb_min, bb_max, m_cell_size, &width, &height);

  // Create heightfield
  rcContext context{};
  rcCreateHeightfield(&context, *m_hf, width, height, bb_min, bb_max,
                      m_cell_size, m_cell_height);

  // Prepare geometry data
  const auto *vertices = glm::value_ptr(m_map.vertices().front());
  const auto vertex_count = m_map.vertices().size();
  const auto *triangles = reinterpret_cast<const int *>(m_map.indices().data());
  const auto triangle_count = m_map.indices().size() / 3;

  // Rasterize triangles
  m_triangle_index.reserve(width * height);
  std::vector<unsigned char> areas(triangle_count);
  mark_walkable_triangles(vertices, triangles, triangle_count, &areas.front());
  rcRasterizeTriangles(&context, vertices, vertex_count, triangles,
                       &areas.front(), triangle_count, *m_hf,
                       &m_triangle_index.front());

  // Filter spans
  rcFilterWalkableLowHeightSpans(
      &context, static_cast<int>(m_walkable_height / m_cell_height), *m_hf);
}

void NSWE::calculate_simple_nswe() {
  const auto walkable_height_cells =
      static_cast<int>(m_walkable_height / m_cell_height);
  const auto min_walkable_climb_cells =
      static_cast<int>(m_min_walkable_climb / m_cell_height);
  const auto max_walkable_climb_cells =
      static_cast<int>(m_max_walkable_climb / m_cell_height);

  const auto max_height = 0xffff;

  for (auto y = 0; y < m_hf->height; ++y) {
    for (auto x = 0; x < m_hf->width; ++x) {
      for (auto *span = m_hf->spans[x + y * m_hf->width]; span != nullptr;
           span = span->next) {

        const auto area = unpack_area(span->area);

        if (area == RC_NULL_AREA) {
          continue;
        }

        const auto bottom = static_cast<int>(span->smax);
        const auto top = span->next != nullptr
                             ? static_cast<int>(span->next->smin)
                             : max_height;

        for (auto direction = 0; direction < 4; ++direction) {
          const auto side_x = x + rcGetDirOffsetX(direction);
          const auto side_y = y + rcGetDirOffsetY(direction);

          if (side_x < 0 || side_y < 0 || side_x >= m_hf->width ||
              side_y >= m_hf->height) {

            span->area = allow_direction(span->area, direction);
            continue;
          }

          auto direction_allowed = false;

          for (auto *neighbour = m_hf->spans[side_x + side_y * m_hf->width];
               neighbour != nullptr; neighbour = neighbour->next) {

            const auto neighbour_bottom = static_cast<int>(neighbour->smax);
            const auto neighbour_top =
                neighbour->next != nullptr
                    ? static_cast<int>(neighbour->next->smin)
                    : max_height;

            const auto height = std::min(top, neighbour_top) -
                                std::max(bottom, neighbour_bottom);
            const auto diff = neighbour_bottom - bottom;

            if (diff >= min_walkable_climb_cells &&
                diff <= max_walkable_climb_cells) {
              span->area = change_area(span->area, RC_COMPLEX_AREA);
            }

            if (height > walkable_height_cells) {
              direction_allowed = diff <= max_walkable_climb_cells;
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

void NSWE::calculate_complex_nswe() {
  const auto map_origin = m_map.bounding_box().min();

  for (auto y = 0; y < m_hf->height; ++y) {
    for (auto x = 0; x < m_hf->width; ++x) {
      for (auto *span = m_hf->spans[x + y * m_hf->width]; span != nullptr;
           span = span->next) {

        const auto area = unpack_area(span->area);

        if (area != RC_COMPLEX_AREA) {
          continue;
        }

        for (auto direction = 0; direction < 4; ++direction) {
          const auto dx = rcGetDirOffsetX(direction);
          const auto dy = rcGetDirOffsetY(direction);
          const auto side_x = x + dx;
          const auto side_y = y + dy;

          if (side_x < 0 || side_y < 0 || side_x >= m_hf->width ||
              side_y >= m_hf->height) {

            continue;
          }

          const auto sphere_radius = m_cell_size / 2.0f;

          const glm::vec3 sphere_center{
              map_origin.x + x * m_cell_size + m_cell_size / 2.0f,
              map_origin.z + span->smax * m_cell_height + sphere_radius,
              map_origin.y + y * m_cell_size + m_cell_size / 2.0f,
          };

          const geometry::Sphere sphere{sphere_center, sphere_radius};

          const auto triangles = triangles_at_columns(x, y, 0);

          for (const auto &triangle : triangles) {
            geometry::Intersection intersection{};

            if (sphere.intersects(triangle, intersection)) {
              if (intersection.normal.y <= 0.0f) {
                span->area = forbid_direction(span->area, direction);
                break;
              }
            }
          }
        }
      }
    }
  }
}

void NSWE::mark_walkable_triangles(const float *vertices, const int *triangles,
                                   std::size_t triangle_count,
                                   unsigned char *areas) {

  const auto walkable_angle_radians = std::cos(glm::radians(m_walkable_angle));

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

auto NSWE::triangles_at_columns(int x, int y, int radius)
    -> std::vector<geometry::Triangle> {

  std::vector<geometry::Triangle> triangles;
  std::unordered_set<int> column_indices;

  const auto &map_vertices = m_map.vertices();
  const auto &map_triangles = m_map.indices();

  for (auto dy = y - radius; dy < y + radius + 1; ++dy) {
    for (auto dx = x - radius; dx < x + radius + 1; ++dx) {
      if (dx < 0 || dy < 0 || dx >= m_hf->width || dy >= m_hf->height) {
        continue;
      }

      const auto &indices = m_triangle_index[x + y * m_hf->width];
      column_indices.insert(indices.cbegin(), indices.cend());
    }
  }

  for (const auto index : column_indices) {
    const geometry::Triangle triangle{
        map_vertices[map_triangles[index * 3 + 0]],
        map_vertices[map_triangles[index * 3 + 1]],
        map_vertices[map_triangles[index * 3 + 2]],
    };

    triangles.emplace_back(triangle);
  }

  return triangles;
}

} // namespace geodata
