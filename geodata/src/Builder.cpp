#include "pch.h"

#include "RecastHelpers.h"

#include <geodata/Builder.h>

namespace geodata {

auto Builder::build(const Map &map, const BuilderSettings &settings) const
    -> Geodata {

  // Build heightfield
  auto *hf = rcAllocHeightfield();
  build_heightfield(*hf, map, settings.cell_size, settings.cell_height,
                    settings.walkable_height, settings.walkable_angle);

  // Calculate NSWE
  calculate_nswe(
      *hf, static_cast<int>(settings.walkable_height / settings.cell_height),
      static_cast<int>(settings.min_walkable_climb / settings.cell_height),
      static_cast<int>(settings.max_walkable_climb / settings.cell_height));

  // Collsisions

  const auto depth =
      static_cast<int>((map.bounding_box().max() - map.bounding_box().min()).z /
                       settings.cell_height) /
      2;

  const auto &triangles = map.triangles_that_intersects(map.bounding_box());

  //  for (auto y = 0; y < hf->height; ++y) {
  //    for (auto x = 0; x < hf->width; ++x) {
  //      for (auto *span = hf->spans[x + y * hf->width]; span != nullptr;
  //           span = span->next) {

  //        const auto area = unpack_area(span->area);

  //        if (area != RC_COMPLEX_AREA) {
  //          continue;
  //        }

  //        const auto &triangles =
  //            map.triangles_that_intersects(map.bounding_box());
  //      }
  //    }
  //  }

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

        const auto z = static_cast<int>(span->smax) - depth;

        geodata.cells.push_back({
            static_cast<std::int16_t>(x),
            static_cast<std::int16_t>(y),
            static_cast<std::int16_t>(static_cast<float>(z) *
                                      settings.cell_height),
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
