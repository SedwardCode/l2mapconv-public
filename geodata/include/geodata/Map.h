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
  explicit Map(const std::string &name, const geometry::Box &bounding_box)
      : m_name{name}, m_bounding_box{bounding_box} {}

  Map(Map &&other) noexcept
      : m_name{std::move(other.m_name)}, m_bounding_box{std::move(
                                             other.m_bounding_box)},
        m_vertices{std::move(other.m_vertices)}, m_indices{std::move(
                                                     other.m_indices)},
        m_entities{std::move(other.m_entities)} {}

  void add(const Entity &entity);

  auto name() const -> const std::string &;
  auto bounding_box() const -> const geometry::Box &;

  auto vertices() const -> const std::vector<glm::vec3> &;
  auto indices() const -> const std::vector<unsigned int> &;

  auto triangles_that_intersects(const geometry::Box &bounding_box) const
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
