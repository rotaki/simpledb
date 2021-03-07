#pragma once

#include <memory>
#include <string>

namespace simpledb {
class constant {
  friend bool operator==(const constant &pLhs, const constant &pRhs);
  friend bool operator!=(const constant &pLhs, const constant &pRhs);
  friend bool operator<(const constant &pLhs, const constant &pRhs);
  friend bool operator>(const constant &pLhs, const constant &pRhs);
  friend bool operator<=(const constant &pLhs, const constant &pRhs);
  friend bool operator>=(const constant &pLhs, const constant &pRhs);

public:
  constant();
  constant(const constant &pVal);
  constant(const int &pVal);
  constant(const std::string &pVal);
  constant &operator=(const constant &pVal);
  int as_int() const;
  std::string as_string() const;
  std::string to_string() const;
  int hash_code() const;
  bool is_null() const;

private:
  std::unique_ptr<int> mIVal;
  std::unique_ptr<std::string> mSVal;
};
} // namespace simpledb
