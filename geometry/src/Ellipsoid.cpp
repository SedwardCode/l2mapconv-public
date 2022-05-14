#include <geometry/Ellipsoid.h>

namespace geometry {

Ellipsoid::Ellipsoid(const glm::vec3 &center, const glm::vec3 &radius)
    : center{center}, radius{radius} {}

} // namespace geometry
