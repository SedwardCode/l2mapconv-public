#pragma once

#include "Box.h"
#include "Intersection.h"
#include "Triangle.h"

#include <glm/glm.hpp>

#include <functional>
#include <vector>

namespace geometry {

struct Sphere {
  glm::vec3 center;
  const float radius;

  explicit Sphere(const glm::vec3 &center, float radius);

  auto intersects(const Triangle &triangle, Intersection &intersection) const
      -> bool;

  auto intersects(const std::vector<Triangle> &triangles) const -> bool;
};

} // namespace geometry
