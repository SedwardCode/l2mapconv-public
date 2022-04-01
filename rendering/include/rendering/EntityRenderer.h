#pragma once

#include "Camera.h"
#include "Context.h"
#include "Entity.h"
#include "EntityShader.h"
#include "FrameSettings.h"
#include "Scene.h"

#include <vector>

namespace rendering {

class EntityRenderer {
public:
  explicit EntityRenderer(Context &context, const Camera &camera);

  void render(const Scene &scene, const FrameSettings &settings,
              int &draws) const;

private:
  Context &m_context;
  const Camera &m_camera;

  void unbind_current_texture() const;
};

} // namespace rendering
