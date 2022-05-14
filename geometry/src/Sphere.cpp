#include <geometry/Sphere.h>

namespace geometry {

Sphere::Sphere(const glm::vec3 &center, float radius)
    : center{center}, radius{radius} {}

auto Sphere::intersects(const Triangle &triangle,
                        Intersection &intersection) const -> bool {

  const auto closest_point = triangle.closest_point_to(center);
  const auto vector = center - closest_point;
  const auto length = glm::length(vector);

  intersection.normal = glm::normalize(vector);
  intersection.depth = radius - length;

  return length <= radius;
}

auto Sphere::intersects(const std::vector<Triangle> &triangles) const -> bool {
  Intersection intersection{};

  for (const auto &triangle : triangles) {
    if (intersects(triangle, intersection)) {
      return true;
    }
  }

  return false;
}

} // namespace geometry
