#pragma once

#include <glm/glm.hpp>

namespace geometry {

struct Intersection {
  glm::vec3 normal;
  float depth;
};

} // namespace geometry
