#include "layout.hpp"
#include "page.hpp"

namespace smartdb {
  layout::layout(std::shared_ptr<schema> pSchema):
    mSchema(pSchema)
  {
    int pos = sizeof(int);      // space for the emtpy/inuse flag
    for (const std::string &fldName: mSchema->fields()) {
      mOffsets[fldName] = pos;
      pos += length_in_bytes(fldName);
    }
    mSlotSize = pos;
  }

  layout::layout(std::shared_ptr<schema> pSchema,
                 std::map<std::string, int> pOffsets,
                 int pSlotSize):
    mSchema(pSchema), mOffsets(pOffsets), mSlotSize(pSlotSize) {}

  std::shared_ptr<schema> layout::get_schema() {
    return mSchema;
  }

  int layout::offset(const std::string &pFldName) {
    if (mOffsets.find(pFldName) == mOffsets.end()) {
      return -1;               // not found
    } else {
      return mOffsets[pFldName];
    }
  }

  int layout::slot_size() {
    return mSlotSize;
  }

  int layout::length_in_bytes(const std::string &pFldName) {
    int fldType = mSchema->type(pFldName);
    if (fldType == schema::integer) {
      return sizeof(int);
    } else if (fldType == schema::varchar) {
      return page::max_length(mSchema->length(pFldName));
    } else {
      return -1;
    }
  }
}
