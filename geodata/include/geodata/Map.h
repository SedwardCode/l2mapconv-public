#pragma once

#include "Entity.h"

#include <utils/NonCopyable.h>

#include <geometry/Box.h>
#include <geometry/Triangle.h>

#include <glm/glm.hpp>

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace geodata {

class Map : public utils::NonCopyable {
public:
  explicit Map(const std::string &name, const geometry::Box &bounding_box);
  Map(Map &&other) noexcept;

  void add(const Entity &entity);

  auto name() const -> const std::string &;
  auto bounding_box() const -> geometry::Box;

  auto flipped_bounding_box() const -> const geometry::Box &;

  auto vertices() const -> const std::vector<glm::vec3> &;
  auto indices() const -> const std::vector<unsigned int> &;

  auto triangles_that_intersects(const geometry::Box &source_bb) const
      -> const std::vector<geometry::Triangle>;

private:
  struct EntityView {
    std::size_t start_index;
    std::size_t index_count;
    geometry::Box bounding_box;

    explicit EntityView() : start_index{}, index_count{}, bounding_box{} {}
  };

  const std::string m_name;
  const geometry::Box m_bounding_box;
  std::vector<glm::vec3> m_vertices;
  std::vector<unsigned int> m_indices;
  std::vector<EntityView> m_entities;
};

} // namespace geodata
