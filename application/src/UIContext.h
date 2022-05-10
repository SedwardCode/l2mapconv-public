#pragma once

#include <functional>

struct UIContext {
  struct {
    int draws;
    bool textures;
    bool culling;
    bool wireframe;
    bool passable;
    bool terrain;
    bool static_meshes;
    bool csg;
    bool bounding_boxes;
    bool imported_geodata;
    bool generated_geodata;

    void set_defaults() {
      culling = true;
      terrain = true;
      static_meshes = true;
      csg = true;
      generated_geodata = true;
    }
  } rendering;

  struct {
    float actor_height;
    float actor_radius;
    float max_walkable_angle;
    float min_walkable_climb;
    float max_walkable_climb;
    float cell_size;
    float cell_height;

    std::function<void()> build_handler;
    bool should_export;

    void set_defaults() {
      actor_height = 48.0f;
      actor_radius = 16.0f;
      max_walkable_angle = 46.0f;
      min_walkable_climb = 8.0f;
      max_walkable_climb = 16.0f;
      cell_size = 16.0f;
      cell_height = 1.0f;
    }
  } geodata;
};
