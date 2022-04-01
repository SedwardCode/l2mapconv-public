#include <geometry/Capsule.h>

namespace geometry {

Capsule::Capsule(const glm::vec3 &base, const glm::vec3 &tip, float radius)
    : m_base{base}, m_tip{tip}, m_radius{radius} {}

auto Capsule::base() const -> const glm::vec3 & { return m_base; }
auto Capsule::tip() const -> const glm::vec3 & { return m_tip; }
auto Capsule::radius() const -> float { return m_radius; }

} // namespace geometry
