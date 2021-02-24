#pragma once

#include <string>
#include <memory>

namespace smartdb {
  class constant {
  public:
    constant();
    constant(const constant &pVal);
    constant(const int &pVal);
    constant(const std::string &pVal);
    constant &operator=(const constant &pVal);
    int as_int() const;
    std::string as_string() const;
    bool operator==(const constant &pVal) const;
    bool operator<(const constant &pVal) const;
    std::string to_string() const;
    int hash_code() const;
  private:
    std::shared_ptr<int> mIVal = std::shared_ptr<int>(nullptr);
    std::shared_ptr<std::string> mSVal = std::shared_ptr<std::string>(nullptr);
  };
}
