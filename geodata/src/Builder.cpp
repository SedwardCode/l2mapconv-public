#include "pch.h"

#include "Preprocessing.h"

#include <geodata/Builder.h>

namespace geodata {

static constexpr auto cell_size = 16.0f;
static constexpr auto cell_height = 1.0f;
static constexpr auto walkable_height = static_cast<int>(48.0f / cell_height);
static constexpr auto walkable_angle = 45.0f;

auto Builder::build(const Map &map, const BuilderSettings &settings) const
    -> Geodata {

  // Flip bounding box for Recast (Y <-> Z)
  const auto *source_bb_min = glm::value_ptr(map.bounding_box().min());
  const auto *source_bb_max = glm::value_ptr(map.bounding_box().max());
  float bb_min[3] = {source_bb_min[0], source_bb_min[2], source_bb_min[1]};
  float bb_max[3] = {source_bb_max[0], source_bb_max[2], source_bb_max[1]};

  // Grid size
  auto width = 0;
  auto height = 0;
  rcCalcGridSize(static_cast<float *>(bb_min), static_cast<float *>(bb_max),
                 cell_size, &width, &height);

  rcContext context{};

  // Create heightfield
  auto *hf = rcAllocHeightfield();
  rcCreateHeightfield(&context, *hf, width, height,
                      static_cast<float *>(bb_min),
                      static_cast<float *>(bb_max), cell_size, cell_height);

  // Prepare geometry data
  const auto *vertices = glm::value_ptr(map.vertices().front());
  const auto vertex_count = map.vertices().size();
  const auto *triangles = reinterpret_cast<const int *>(map.indices().data());
  const auto triangle_count = map.indices().size() / 3;

  // Rasterize triangles
  std::vector<unsigned char> areas(triangle_count);
  mark_triangles(walkable_angle, vertices, triangles, triangle_count,
                 &areas.front());
  rcRasterizeTriangles(&context, vertices, vertex_count, triangles,
                       &areas.front(), triangle_count, *hf, 0);

  // Filter low height spans
  rcFilterWalkableLowHeightSpans(&context, walkable_height, *hf);

  // Calculate NSWE
  calculate_nswe(*destination_hf, walkable_height, min_walkable_climb,
                 max_walkable_climb);

  // Convert heightfield to geodata
  Geodata geodata;

  const auto depth = static_cast<int>((bb_max[2] - bb_min[2]) / cell_height);

  std::vector<int> columns(hf->width * hf->height);
  auto black_holes = 0;

  for (auto y = 0; y < hf->height; ++y) {
    for (auto x = 0; x < hf->width; ++x) {
      for (auto *span = hf->spans[x + y * hf->width]; span != nullptr;
           span = span->next) {

        const auto area = static_cast<int>(span->area) & 0xf;
        const auto nswe = static_cast<int>(span->area) >> 4;

        if (area == RC_NULL_AREA) {
          continue;
        }

        if (nswe == 0) {
          black_holes++;
        }

        const auto z = static_cast<int>(span->smax) - depth / 2 + 1;

        geodata.cells.push_back({
            static_cast<std::int16_t>(x),
            static_cast<std::int16_t>(y),
            static_cast<std::int16_t>(static_cast<float>(z) * cell_height +
                                      28), // TODO: Fucking magic
            BLOCK_MULTILAYER,
            (nswe & DIRECTION_N) != 0,
            (nswe & DIRECTION_W) != 0,
            (nswe & DIRECTION_E) != 0,
            (nswe & DIRECTION_S) != 0,
        });

        columns[y * hf->width + x]++;
      }
    }
  }

  // Add fake cells to columns with no layers
  for (auto y = 0; y < hf->height; ++y) {
    for (auto x = 0; x < hf->width; ++x) {
      if (columns[y * hf->width + x] > 0) {
        continue;
      }

      geodata.cells.push_back({
          static_cast<std::int16_t>(x),
          static_cast<std::int16_t>(y),
          -0x4000,
          BLOCK_COMPLEX,
          false,
          false,
          false,
          false,
      });
    }
  }

  if (black_holes > 0) {
    utils::Log(utils::LOG_WARN, "Geodata")
        << "Black holes (points of no return): " << black_holes << std::endl;
  }

  rcFreeHeightField(hf);

  return geodata;
}

} // namespace geodata
