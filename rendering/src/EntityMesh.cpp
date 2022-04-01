#include "pch.h"

#include <rendering/EntityMesh.h>
#include <rendering/VertexBuffer.h>

namespace rendering {

EntityMesh::EntityMesh(Context &context, const std::vector<Vertex> &vertices,
                       const std::vector<std::uint32_t> &indices,
                       const std::vector<MeshSurface> &surfaces,
                       const std::vector<glm::mat4> &instance_matrices,
                       const geometry::Box &bounding_box)
    : m_mesh{context, vertices.size(),
             vertex_buffers(vertices, instance_matrices), indices},
      m_surfaces{surfaces}, m_instances{instance_matrices.size()},
      m_bounding_box{bounding_box} {

  ASSERT(indices.size() >= 3, "Rendering", "Mesh must have at least 3 indices");
  ASSERT(!m_surfaces.empty(), "Rendering",
         "Mesh must have at least one surface");
  ASSERT(m_instances > 0, "Rendering",
         "Number of instances must be greater than zero");

  std::size_t index_count = 0;

  for (auto &surface : m_surfaces) {
    ASSERT(index_count + surface.index_count <= m_mesh.index_count() ||
               surface.index_offset + surface.index_count <=
                   m_mesh.index_count(),
           "Rendering", "Indices out of bounds");
    ASSERT(surface.index_count >= 3, "Rendering",
           "Surface must have at least 3 indices");

    index_count += surface.index_count;
  }

  ASSERT(index_count == m_mesh.index_count(), "Rendering",
         "Surfaces must cover entire mesh");
}

auto EntityMesh::surfaces() const -> const std::vector<MeshSurface> & {
  return m_surfaces;
}

auto EntityMesh::bounding_box() const -> const geometry::Box & {
  return m_bounding_box;
}

void EntityMesh::draw(const MeshSurface &surface) const {
  m_mesh.draw(GL_TRIANGLES, m_instances, surface.index_offset,
              surface.index_count);
}

auto EntityMesh::vertex_buffers(const std::vector<Vertex> &vertices,
                                const std::vector<glm::mat4> &instance_matrices)
    -> std::vector<VertexBuffer> {

  VertexBuffer vertex_buffer{vertices};
  vertex_buffer.float_layout(0, sizeof(Vertex::position),
                             offsetof(Vertex, position));
  vertex_buffer.float_layout(1, sizeof(Vertex::normal),
                             offsetof(Vertex, normal));
  vertex_buffer.float_layout(2, sizeof(Vertex::uv), offsetof(Vertex, uv));

  VertexBuffer instance_matrix_buffer{instance_matrices};
  instance_matrix_buffer.float_layout(3, sizeof(glm::vec4), 0, 1);
  instance_matrix_buffer.float_layout(4, sizeof(glm::vec4), sizeof(glm::vec4),
                                      1);
  instance_matrix_buffer.float_layout(5, sizeof(glm::vec4),
                                      sizeof(glm::vec4) * 2, 1);
  instance_matrix_buffer.float_layout(6, sizeof(glm::vec4),
                                      sizeof(glm::vec4) * 3, 1);

  return {vertex_buffer, instance_matrix_buffer};
}

} // namespace rendering
