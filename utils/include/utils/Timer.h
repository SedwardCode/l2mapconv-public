#pragma once

#include "Log.h"

#include <chrono>
#include <string>

namespace utils {

class Timer {
public:
  explicit Timer(const std::string &name)
      : m_name{name}, m_start{std::chrono::high_resolution_clock::now()} {}

  ~Timer() {
    const auto now = std::chrono::high_resolution_clock::now();

    Log(LOG_INFO) << "Timer " << m_name << ": "
                  << std::chrono::duration_cast<std::chrono::microseconds>(
                         now - m_start)
                         .count()
                  << " us" << std::endl;
  }

private:
  const std::string m_name;
  const std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};

} // namespace utils
