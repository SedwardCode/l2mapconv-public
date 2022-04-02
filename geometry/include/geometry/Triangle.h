#pragma once

#include "Box.h"

#include <glm/glm.hpp>

namespace geometry {

class Triangle {
public:
  const glm::vec3 a;
  const glm::vec3 b;
  const glm::vec3 c;

  explicit Triangle(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c);

  auto bounding_box() const -> Box;

  auto closest_point(const glm::vec3 &point) const -> glm::vec3;
};

} // namespace geometry
