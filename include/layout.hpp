#pragma once

#include <memory>

#include "schema.hpp"

namespace smartdb {
  class layout {
  public:
    layout(std::shared_ptr<schema> pSchema);
    layout(std::shared_ptr<schema> pSchema, std::map<std::string, int> pOffsets, int pSlotSize);
    std::shared_ptr<schema> get_schema() const;
    int offset(const std::string &pFldName);
    int slot_size() const;
      
  private:
    std::shared_ptr<schema> mSchema;
    std::map<std::string, int> mOffsets;
    int mSlotSize;

    int length_in_bytes(const std::string &pFldName);

  };
}
