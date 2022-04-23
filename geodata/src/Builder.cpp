#include "pch.h"

#include "Compressor.h"
#include "NSWE.h"

#include <geodata/Builder.h>

namespace geodata {

auto Builder::build(const Map &map, const BuilderSettings &settings) const
    -> const ExportBuffer & {

  const auto map_origin = map.bounding_box().min();

  // Build heightfield
  auto *hf = rcAllocHeightfield();
  std::vector<std::vector<int>> triangle_index;
  build_filtered_heightfield(*hf, map, triangle_index, settings.cell_size,
                             settings.cell_height, settings.walkable_height,
                             settings.walkable_angle);

  // Calculate simple NSWE
  calculate_simple_nswe(*hf, settings.cell_height, settings.walkable_height,
                        settings.min_walkable_climb,
                        settings.max_walkable_climb);

  // Calculate complex NSWE based on sphere-to-mesh collision
  calculate_complex_nswe(*hf, map, triangle_index, settings.cell_size,
                         settings.cell_height);

  // Convert heightfield to geodata
  Geodata geodata;

  std::vector<int> columns(hf->width * hf->height);
  auto black_holes = 0;

  for (auto y = 0; y < hf->height; ++y) {
    for (auto x = 0; x < hf->width; ++x) {
      for (auto *span = hf->spans[x + y * hf->width]; span != nullptr;
           span = span->next) {

        const auto area = unpack_area(span->area);
        const auto nswe = unpack_nswe(span->area);

        if (area == RC_NULL_AREA) {
          continue;
        }

        if (nswe == 0) {
          black_holes++;
        }

        geodata.cells.push_back({
            static_cast<std::int16_t>(x),
            static_cast<std::int16_t>(y),
            static_cast<std::int16_t>(map_origin.z +
                                      span->smax * settings.cell_height),
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

  // Compress export buffer and return it
  m_export_buffer.reset(geodata);

  Compressor compressor{m_export_buffer};
  compressor.compress();

  return m_export_buffer;
}

} // namespace geodata
