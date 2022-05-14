#pragma once

#include "Box.h"

#include <glm/glm.hpp>

namespace geometry {

struct Triangle {
  const glm::vec3 a;
  const glm::vec3 b;
  const glm::vec3 c;

  explicit Triangle(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c);

  auto closest_point_to(const glm::vec3 &point) const -> glm::vec3;
};

} // namespace geometry
