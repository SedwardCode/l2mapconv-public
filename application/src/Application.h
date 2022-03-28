#pragma once

#include <filesystem>
#include <string>
#include <vector>

class Application {
public:
  explicit Application();

  void preview(const std::filesystem::path &client_root,
               const std::vector<std::string> &maps) const;
  void build(const std::filesystem::path &client_root,
             const std::vector<std::string> &maps) const;
};
