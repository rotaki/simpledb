#pragma once

#include <string>
#include <memory>
#include <vector>
#include <map>

namespace simpledb {
  class schema {
  public:
    enum type {
               integer = 0,
               varchar = 1
    };
    schema();

    schema(const schema &pSch);

    schema &operator=(const schema &pSch);
    
    void add_field(const std::string &pFldName, const int &pType, const int &pLength);
    void add_int_field(const std::string &pFldName);

    void add_string_field(const std::string &pFldName, const int &pLength);

    void add(const std::string &pFldName, const schema &pSchema);

    void add_all(const schema &pSchema);

    std::vector<std::string> fields() const ;

    bool has_field(const std::string &pFldName) const;

    int type(const std::string &pFldName) const;

    int length(const std::string &pFldName) const;

    class field_info {
    public:
      field_info() = default;
      field_info(const int &pType, const int &pLength);
      int type() const;
      int length() const;
    private:
      int mType, mLength;
    };

  private:
    std::vector<std::string> mFields;
    std::map<std::string, field_info> mInfo;
  };
}
