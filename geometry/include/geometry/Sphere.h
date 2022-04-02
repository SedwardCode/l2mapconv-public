#pragma once

#include "Box.h"
#include "Intersection.h"
#include "Triangle.h"

#include <glm/glm.hpp>

namespace geometry {

class Sphere {
public:
  explicit Sphere(const glm::vec3 &center, float radius);

  auto bounding_box() const -> Box;

  auto intersects(const Triangle &triangle, Intersection &intersection) const
      -> bool;

private:
  const glm::vec3 m_center;
  const float m_radius;
};

} // namespace geometry
