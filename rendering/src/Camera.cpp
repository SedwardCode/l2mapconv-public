#include "pch.h"

#include <rendering/Camera.h>

namespace rendering {

Camera::Camera(Context &context, float fov, float near,
               const glm::vec3 &position)
    : m_context{context}, m_fov{fov}, m_near{near}, m_position{position},
      m_orientation{glm::quatLookAt({0.0f, 1.0f, 0.0f}, m_up)} {}

void Camera::translate(const glm::vec3 &direction) { m_position += direction; }

void Camera::set_position(const glm::vec3 &position) { m_position = position; }

void Camera::rotate(float angle, const glm::vec3 &axis) {
  m_orientation = glm::normalize(glm::angleAxis(angle, axis) * m_orientation);
}

auto Camera::position() const -> const glm::vec3 & { return m_position; }

auto Camera::forward() const -> glm::vec3 {
  return m_orientation * glm::vec3{0.0f, 0.0f, -1.0f};
}

auto Camera::right() const -> glm::vec3 {
  return m_orientation * glm::vec3{1.0f, 0.0f, 0.0f};
}

auto Camera::up() const -> glm::vec3 { return m_up; }

auto Camera::view_matrix() const -> glm::mat4 {
  return glm::translate(glm::mat4_cast(glm::conjugate(m_orientation)),
                        -m_position);
}

auto Camera::projection_matrix() const -> glm::mat4 {
  const auto ratio = static_cast<float>(m_context.framebuffer.size.width) /
                     static_cast<float>(m_context.framebuffer.size.height);
  return glm::infinitePerspective(glm::radians(m_fov), ratio, m_near);
}

auto Camera::frustum() const -> geometry::Frustum {
  return geometry::Frustum{projection_matrix(), view_matrix()};
}

} // namespace rendering
