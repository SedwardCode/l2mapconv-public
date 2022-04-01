#pragma once

#include "Box.h"

#include <glm/glm.hpp>

namespace geometry {

class Capsule {
public:
  explicit Capsule(const glm::vec3 &base, const glm::vec3 &tip, float radius);

  auto intersects(const Box &box) const -> bool;

private:
  glm::vec3 m_base;
  glm::vec3 m_tip;
  float m_radius;
};

} // namespace geometry
