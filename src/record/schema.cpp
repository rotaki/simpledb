#include <algorithm>

#include "record/schema.hpp"

namespace simpledb {
  schema::schema() {}

  schema::schema(const schema &pSch) {
    mFields = pSch.mFields;
    mInfo = pSch.mInfo;
  }

  schema &schema::operator=(const schema &pSch) {
    if (this != &pSch) {
      mFields = pSch.mFields;
      mInfo = pSch.mInfo;
    }
    return *this;
  }
  
  void schema::add_field(const std::string &pFldName, const int &pType, const int &pLength) {
    mFields.emplace_back(pFldName);
    mInfo[pFldName] = schema::field_info(pType, pLength);
  }

  void schema::add_int_field(const std::string &pFldName) {
    add_field(pFldName, integer, 0);
  }

  void schema::add_string_field(const std::string &pFldName, const int &length) {
    add_field(pFldName, varchar, length);
  }

  void schema::add(const std::string &pFldName, const schema &pSchema) {
    int type = pSchema.type(pFldName);
    int length = pSchema.length(pFldName);
    add_field(pFldName, type, length);
  }

  void schema::add_all(const schema &pSchema) {
    for (const std::string &fldName: pSchema.fields()) {
      add(fldName, pSchema);
    }
  }

  std::vector<std::string> schema::fields() const {
    return mFields;
  }

  bool schema::has_field(const std::string &fldName) const{
    return (std::find(mFields.begin(), mFields.end(), fldName) != mFields.end());
  }

  int schema::type(const std::string &pFldName) const {
    if (mInfo.find(pFldName) == mInfo.end()) {
      throw std::runtime_error("field info (" + pFldName+ ") not found");
    } else {
      return mInfo.at(pFldName).type();
    }
  }

  int schema::length(const std::string &pFldName) const {
    if (mInfo.find(pFldName) == mInfo.end()) {
      throw std::runtime_error("field info (" + pFldName+ ") not found");
    } else {
      return mInfo.at(pFldName).length();
    }
  }

  schema::field_info::field_info(const int &pType, const int &pLength):
    mType(pType), mLength(pLength) {
  }

  int schema::field_info::type() const{
    return mType;
  }

  int schema::field_info::length() const{
    return mLength;
  }
  
};
