#include <geometry/Sphere.h>

namespace geometry {

Sphere::Sphere(const glm::vec3 &center, float radius)
    : m_center{center}, m_radius{radius} {}

auto Sphere::bounding_box() const -> Box {
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
    bounding_box += m_center + corners[i] * m_radius;
  }

  return bounding_box;
}

auto Sphere::intersects(const Triangle &triangle,
                        Intersection &intersection) const -> bool {

  const auto closest_point = triangle.closest_point(m_center);
  const auto vector = closest_point - m_center;
  const auto length = glm::length(vector);

  intersection.normal = glm::normalize(vector);
  intersection.depth = m_radius - length;

  return length <= m_radius;
}

} // namespace geometry
