#include "record/layout.hpp"
#include "file/page.hpp"

namespace simpledb {
  layout::layout() {}

  layout::layout(const layout &pLt):
    mSchema(pLt.mSchema), mOffsets(pLt.mOffsets), mSlotSize(pLt.mSlotSize) {}


  layout::layout(const schema &pSchema):
    mSchema(pSchema)
  {
    int pos = sizeof(int);      // space for the emtpy/inuse flag
    for (const std::string &fldName: mSchema.fields()) {
      mOffsets[fldName] = pos;
      pos += length_in_bytes(fldName);
    }
    mSlotSize = pos;
  }

  layout::layout(const schema &pSchema,
                 const std::map<std::string, int> &pOffsets,
                 int pSlotSize):
    mSchema(pSchema), mOffsets(pOffsets), mSlotSize(pSlotSize) {}

  layout &layout::operator=(const layout &pLt) {
    if (this != &pLt) {
      mSchema = pLt.mSchema;
      mOffsets = pLt.mOffsets;
      mSlotSize = pLt.mSlotSize;
    }
    return *this;
  }

  schema layout::get_schema() const {
    return mSchema;
  }

  int layout::offset(const std::string &pFldName) {
    if (mOffsets.find(pFldName) == mOffsets.end()) {
      throw std::runtime_error("field name " + pFldName + " not found in schema" );
    } else {
      return mOffsets[pFldName];
    }
  }

  int layout::slot_size() const {
    return mSlotSize;
  }

  int layout::length_in_bytes(const std::string &pFldName) {
    int fldType = mSchema.type(pFldName);
    if (fldType == schema::integer) {
      return sizeof(int);
    } else if (fldType == schema::varchar) {
      return page::max_length(mSchema.length(pFldName));
    } else {
      throw std::runtime_error("field type not defined");
    }
  }
}
