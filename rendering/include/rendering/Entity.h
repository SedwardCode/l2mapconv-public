#pragma once

#include "DrawableMesh.h"
#include "EntityMesh.h"
#include "EntityShader.h"

#include <geometry/Box.h>

#include <glm/glm.hpp>

#include <memory>

namespace rendering {

class Entity {
public:
  explicit Entity(std::shared_ptr<DrawableMesh> mesh,
                  std::shared_ptr<EntityShader> shader,
                  const glm::mat4 &model_matrix, bool wireframe);

  auto mesh() const -> const std::shared_ptr<DrawableMesh> &;
  auto shader() const -> const std::shared_ptr<EntityShader> &;
  auto model_matrix() const -> const glm::mat4 &;
  auto bounding_box() const -> const geometry::Box &;
  auto wireframe() const -> bool;

private:
  std::shared_ptr<DrawableMesh> m_mesh;
  std::shared_ptr<EntityShader> m_shader;
  glm::mat4 m_model_matrix;
  geometry::Box m_bounding_box;
  bool m_wireframe;
};

} // namespace rendering
