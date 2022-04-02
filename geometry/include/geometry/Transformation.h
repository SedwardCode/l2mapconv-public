#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace geometry {

auto transformation_matrix(const glm::mat4 &identity, const glm::vec3 &position,
                           const glm::vec3 &rotation, const glm::vec3 &scale)
    -> glm::mat4;

} // namespace geometry
