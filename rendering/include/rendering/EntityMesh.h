#pragma once

#include "Context.h"
#include "DrawableMesh.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshSurface.h"
#include "Vertex.h"
#include "VertexBuffer.h"

#include <utils/NonCopyable.h>

#include <geometry/Box.h>

#include <glm/glm.hpp>

#include <cstdint>
#include <vector>

namespace rendering {

class EntityMesh : public utils::NonCopyable, public DrawableMesh {
public:
  explicit EntityMesh(Context &context, const std::vector<Vertex> &vertices,
                      const std::vector<std::uint32_t> &indices,
                      const std::vector<MeshSurface> &surfaces,
                      const std::vector<glm::mat4> &instance_matrices,
                      const geometry::Box &bounding_box);

  virtual auto surfaces() const -> const std::vector<MeshSurface> & override;
  virtual auto bounding_box() const -> const geometry::Box & override;

  virtual void draw(const MeshSurface &surface) const override;

private:
  const Mesh m_mesh;
  const std::vector<MeshSurface> m_surfaces;
  const std::size_t m_instances;
  const geometry::Box m_bounding_box;

  auto vertex_buffers(const std::vector<Vertex> &vertices,
                      const std::vector<glm::mat4> &instance_matrices)
      -> std::vector<VertexBuffer>;
};

} // namespace rendering
