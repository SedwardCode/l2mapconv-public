#pragma once

#include "Entity.h"

#include <geometry/Box.h>

#include <glm/glm.hpp>

#include <string>
#include <vector>

struct Map {
  std::string name;
  std::vector<Entity<EntityMesh>> entities;
  glm::vec3 position;
  geometry::Box bounding_box;

  explicit Map() : name{}, entities{}, position{}, bounding_box{} {}
};
