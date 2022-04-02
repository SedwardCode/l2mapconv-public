#pragma once

#include "Entity.h"

#include <utils/NonCopyable.h>

#include <geometry/Box.h>

#include <glm/glm.hpp>

#include <cstdint>
#include <span>
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

  auto vertices() const -> const std::vector<glm::vec3> &;
  auto indices() const -> const std::vector<unsigned int> &;

  auto bounding_box() const -> const geometry::Box &;

private:
  struct EntityView {
    std::span<unsigned int> indices;
    geometry::Box bounding_box;

    explicit EntityView() : indices{}, bounding_box{} {}
  };

  const std::string m_name;
  const geometry::Box m_bounding_box;
  std::vector<glm::vec3> m_vertices;
  std::vector<unsigned int> m_indices;
  std::vector<EntityView> m_entities;
};

} // namespace geodata
