#include <geometry/Sphere.h>

namespace geometry {

Sphere::Sphere(const glm::vec3 &center, float radius)
    : center{center}, radius{radius} {}

auto Sphere::bounding_box() const -> Box {
  static constexpr glm::vec3 corners[8] = {
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
    bounding_box += center + corners[i] * radius;
  }

  return bounding_box;
}

auto Sphere::intersects(const Triangle &triangle,
                        Intersection &intersection) const -> bool {

  const auto closest_point = triangle.closest_point_to(center);
  const auto vector = center - closest_point;
  const auto length = glm::length(vector);

  intersection.normal = glm::normalize(vector);
  intersection.depth = radius - length;

  return length <= radius;
}

auto Sphere::intersects(const std::vector<Triangle> &triangles,
                        std::function<bool(float, float)> normal_predicate,
                        float normal_treshold) const -> bool {

  Intersection intersection{};

  for (const auto &triangle : triangles) {
    if (intersects(triangle, intersection) &&
        normal_predicate(intersection.normal.y, normal_treshold)) {

      return true;
    }
  }

  return false;
}

} // namespace geometry
