#pragma once

#include "MeshSurface.h"

#include <geometry/Box.h>

#include <vector>

namespace rendering {

class DrawableMesh {
public:
  virtual auto surfaces() const -> const std::vector<MeshSurface> & = 0;
  virtual auto bounding_box() const -> const geometry::Box & = 0;

  virtual void draw(const MeshSurface &surface) const = 0;
};

} // namespace rendering
