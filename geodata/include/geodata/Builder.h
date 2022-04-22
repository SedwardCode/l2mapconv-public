#pragma once

#include "BuilderSettings.h"
#include "ExportBuffer.h"
#include "Geodata.h"
#include "Map.h"

namespace geodata {

class Builder {
public:
  auto build(const Map &map, const BuilderSettings &settings) const
      -> const ExportBuffer &;

private:
  mutable ExportBuffer m_export_buffer;
};

} // namespace geodata
