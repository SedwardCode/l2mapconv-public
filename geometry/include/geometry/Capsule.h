#pragma once

#include <glm/glm.hpp>

namespace geometry {

class Capsule {
public:
  explicit Capsule(const glm::vec3 &base, const glm::vec3 &tip, float radius);

  auto base() const -> const glm::vec3 &;
  auto tip() const -> const glm::vec3 &;
  auto radius() const -> float;

private:
  const glm::vec3 m_base;
  const glm::vec3 m_tip;
  const float m_radius;
};

} // namespace geometry
