#pragma once

#include "Context.h"
#include "DrawableMesh.h"
#include "GeodataCell.h"
#include "Mesh.h"
#include "MeshSurface.h"

#include <geometry/Box.h>

#include <utils/NonCopyable.h>

#include <vector>

namespace rendering {

class GeodataMesh : public utils::NonCopyable, public DrawableMesh {
public:
  explicit GeodataMesh(Context &context, const std::vector<GeodataCell> &cells,
                       const MeshSurface &surface,
                       const geometry::Box &bounding_box);

  virtual auto surfaces() const -> const std::vector<MeshSurface> & override;
  virtual auto bounding_box() const -> const geometry::Box & override;

  virtual void draw(const MeshSurface &surface) const override;

private:
  const Mesh m_mesh;
  const std::vector<MeshSurface> m_surfaces;
  const geometry::Box m_bounding_box;

  auto vertex_buffer(const std::vector<GeodataCell> &cells) -> VertexBuffer;
};

} // namespace rendering
