#pragma once

#include <memory>

#include "record/schema.hpp"

namespace smartdb {
  class layout {
  public:
    layout();
    layout(const layout &pLt);
    
    layout(const schema &pSchema);
    layout(const schema &pSchema, const std::map<std::string, int> &pOffsets, int pSlotSize);

    layout& operator=(const layout &pLt);
    schema get_schema() const;
    int offset(const std::string &pFldName);
    int slot_size() const;
      
  private:
    schema mSchema;
    std::map<std::string, int> mOffsets;
    int mSlotSize;

    int length_in_bytes(const std::string &pFldName);

  };
}
