#include <geometry/Capsule.h>

namespace geometry {

Capsule::Capsule(const glm::vec3 &base, const glm::vec3 &tip, float radius)
    : m_base{base}, m_tip{tip}, m_radius{radius} {}

auto Capsule::bounding_box() const -> Box {
  static const glm::vec3 corners[8] = {
      {-1.0f, -1.0f, -1.0f}, //
      {1.0f, -1.0f, -1.0f},  //
      {-1.0f, -1.0f, 1.0f},  //
      {1.0f, -1.0f, 1.0f},   //

      {-1.0f, 1.0f, -1.0f}, //
      {1.0f, 1.0f, -1.0f},  //
      {-1.0f, 1.0f, 1.0f},  //
      {1.0f, 1.0f, 1.0f},   //
  };

  Box bounding_box{};

  for (auto i = 0; i < 8; ++i) {
    bounding_box += m_base + corners[i] * m_radius;
    bounding_box += m_tip + corners[i] * m_radius;
  }

  return bounding_box;
}

} // namespace geometry
