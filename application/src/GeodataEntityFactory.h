#pragma once

#include "Entity.h"

#include <geometry/Box.h>

#include <geodata/Geodata.h>

#include <cstdint>
#include <vector>

class GeodataEntityFactory {
public:
  auto make_entity(const geodata::Geodata &geodata,
                   const geometry::Box &bounding_box,
                   const std::uint64_t surface_type) const
      -> Entity<GeodataMesh>;
};
