#include "pch.h"

#include <rendering/EntityShader.h>

namespace rendering {

EntityShader::EntityShader(Shader &&shader) : m_shader{std::move(shader)} {}

void EntityShader::load_projection_matrix(const glm::mat4 &projection) const {
  m_shader.load("u_projection", projection);
}

void EntityShader::load_view_matrix(const glm::mat4 &view) const {
  m_shader.load("u_view", view);
}

void EntityShader::load_model_matrix(const glm::mat4 &model) const {
  m_shader.load("u_model", model);
}

void EntityShader::load_texture_unit(int unit) const {
  m_shader.load("u_texture", unit, false);
}

void EntityShader::load_color(const glm::vec3 &color) const {
  m_shader.load("u_color", color, false);
}

void EntityShader::load_camera(const glm::vec3 &camera) const {
  m_shader.load("u_camera", camera, false);
}

void EntityShader::bind() const { m_shader.bind(); }
void EntityShader::unbind() const { m_shader.unbind(); }

} // namespace rendering
