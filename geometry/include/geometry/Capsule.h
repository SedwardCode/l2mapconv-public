#pragma once

#include <glm/glm.hpp>

namespace geometry {

class Capsule {
public:
  explicit Capsule(const glm::vec3 &base, const glm::vec3 &tip, float radius)
      : m_base{base}, m_tip{tip}, m_radius{radius} {}

private:
  glm::vec3 m_base;
  glm::vec3 m_tip;
  float m_radius;
};

} // namespace geometry
