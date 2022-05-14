#include <geometry/Triangle.h>

namespace geometry {

Triangle::Triangle(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c)
    : a{a}, b{b}, c{c} {}

// Copy-pasted from the Real-Time Collision Detection book
auto Triangle::closest_point_to(const glm::vec3 &point) const -> glm::vec3 {
  // Check if P in vertex region outside A
  const auto ab = b - a;
  const auto ac = c - a;
  const auto ap = point - a;
  const auto d1 = glm::dot(ab, ap);
  const auto d2 = glm::dot(ac, ap);
  if (d1 <= 0.0f && d2 <= 0.0f) {
    return a;
  }

  // Check if P in vertex region outside B
  const auto bp = point - b;
  const auto d3 = glm::dot(ab, bp);
  const auto d4 = glm::dot(ac, bp);
  if (d3 >= 0.0f && d4 <= d3) {
    return b; // barycentric coordinates (0, 1, 0)
  }

  // Check if P in edge region of AB, if so return projection of P onto AB
  const auto vc = d1 * d4 - d3 * d2;
  if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
    const auto v = d1 / (d1 - d3);
    return a + v * ab; // barycentric coordinates (1-v, v, 0)
  }

  // Check if P in vertex region outside C
  const auto cp = point - c;
  const auto d5 = glm::dot(ab, cp);
  const auto d6 = glm::dot(ac, cp);
  if (d6 >= 0.0f && d5 <= d6) {
    return c; // barycentric coordinates (0, 0, 1)
  }

  // Check if P in edge region of AC, if so return projection of P onto AC
  const auto vb = d5 * d2 - d1 * d6;
  if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
    const auto w = d2 / (d2 - d6);
    return a + w * ac; // barycentric coordinates (1-w, 0, w)
  }

  // Check if P in edge region of BC, if so return projection of P onto BC
  const auto va = d3 * d6 - d5 * d4;
  if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
    const auto w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
    return b + w * (c - b); // barycentric coordinates (0, 1-w, w)
  }

  // P inside face region. Compute Q through its barycentric coordinates (u, v,
  // w)
  const auto denom = 1.0f / (va + vb + vc);
  const auto v = vb * denom;
  const auto w = vc * denom;
  return a + ab * v + ac * w; // = u*a + v*b + w*c, u = va*denom = 1.0f-v-w
}

} // namespace geometry
