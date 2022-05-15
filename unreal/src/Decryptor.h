#pragma once

#include <filesystem>
#include <iostream>
#include <optional>

namespace unreal {

class Decryptor {
public:
  void decrypt(const std::filesystem::path &path, std::ostream &output) const;

private:
  auto extract_version(std::istream &input) const -> std::optional<int>;
  void decrypt_xor(std::istream &input, std::ostream &output, int key) const;
  void decrypt_v111(std::istream &input, std::ostream &output) const;
  void decrypt_v121(std::istream &input, std::ostream &output,
                    const std::filesystem::path &path) const;
};

} // namespace unreal
