#include <geometry/Capsule.h>

namespace geometry {

Capsule::Capsule(const glm::vec3 &base, const glm::vec3 &tip, float radius)
    : m_base{base}, m_tip{tip}, m_radius{radius} {}

auto Capsule::intersects(const Box &box) const -> bool {
  static const glm::vec3 corners[8] = {
      glm::vec3{-1, -1, -1}, //
      glm::vec3{1, -1, -1},  //
      glm::vec3{-1, -1, 1},  //
      glm::vec3{1, -1, 1},   //

      glm::vec3{-1, 1, -1}, //
      glm::vec3{1, 1, -1},  //
      glm::vec3{-1, 1, 1},  //
      glm::vec3{1, 1, 1},   //
  };

  Box bounding_box{};

  for (auto i = 0; i < 8; ++i) {
    bounding_box += m_base + corners[i] * m_radius;
  }

  for (auto i = 0; i < 8; ++i) {
    bounding_box += m_tip + corners[i] * m_radius;
  }

  return bounding_box.intersects(box);
}

} // namespace geometry
