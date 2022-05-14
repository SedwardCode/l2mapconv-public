#pragma once

#include <glm/glm.hpp>

namespace geometry {

struct Ellipsoid {
  const glm::vec3 center;
  const glm::vec3 radius;

  explicit Ellipsoid(const glm::vec3 &center, const glm::vec3 &radius);
};

} // namespace geometry
