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
  float radius;

  explicit Sphere(const glm::vec3 &center, float radius);

  auto bounding_box() const -> Box;

  auto intersects(const Triangle &triangle, Intersection &intersection) const
      -> bool;

  auto intersects(const std::vector<Triangle> &triangles,
                  std::function<bool(float, float)> slope_predicate,
                  float slope_treshold) const -> bool;
};

} // namespace geometry
