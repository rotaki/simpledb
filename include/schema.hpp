#pragma once

#include <string>
#include <memory>
#include <vector>
#include <map>

namespace smartdb {
  class schema {
  public:
    enum type {
               integer = 0,
               varchar = 1
    };
    void add_field(const std::string &pFldName, const int &pType, const int &pLength);
    void add_int_field(const std::string &pFldName);

    void add_string_field(const std::string &pFldName, const int &pLength);

    void add(const std::string &pFldName, std::shared_ptr<schema> pSchema);

    void add_all(std::shared_ptr<schema> pSchema);

    std::vector<std::string> fields();

    bool has_field(const std::string &pFldName);

    int type(const std::string &pFldName);

    int length(const std::string &pFldName);

    class field_info {
    public:
      field_info(const int &pType, const int &pLength);
      int type();
      int length();
    private:
      int mType, mLength;
    };

  private:
    std::vector<std::string> mFields;
    std::map<std::string, std::shared_ptr<field_info>> mInfo; // todo value ma?
  };
}
