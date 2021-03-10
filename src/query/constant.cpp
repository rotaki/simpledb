#include <functional>

#include "query/constant.hpp"

namespace simpledb {
bool operator==(const constant &pLhs, const constant &pRhs) {
  return (pLhs.mIVal ? *(pLhs.mIVal) == *(pRhs.mIVal)
                     : *(pLhs.mSVal) == *(pRhs.mSVal));
}

bool operator!=(const constant &pLhs, const constant &pRhs) {
  if (pLhs == pRhs) {
    return false;
  } else {
    return true;
  }
}

bool operator<(const constant &pLhs, const constant &pRhs) {
  return (pLhs.mIVal ? *(pLhs.mIVal) < *(pRhs.mIVal)
                     : *(pLhs.mSVal) < *(pRhs.mSVal));
}

bool operator>(const constant &pLhs, const constant &pRhs) {
  return (pLhs.mIVal ? *(pLhs.mIVal) > *(pRhs.mIVal)
                     : *(pLhs.mSVal) > *(pRhs.mSVal));
}

bool operator<=(const constant &pLhs, const constant &pRhs) {
  if (pLhs > pRhs) {
    return false;
  } else {
    return true;
  }
}

bool operator>=(const constant &pLhs, const constant &pRhs) {
  if (pLhs < pRhs) {
    return false;
  } else {
    return true;
  }
}

constant::constant() {}

constant::constant(const constant &pVal) {
  if (pVal.mIVal) {
    mIVal = std::make_unique<int>(*(pVal.mIVal));
  }
  if (pVal.mSVal) {
    mSVal = std::make_unique<std::string>(*(pVal.mSVal));
  }
}

constant::constant(int pVal) { mIVal = std::make_unique<int>(pVal); }

constant::constant(const std::string &pVal) {
  mSVal = std::make_unique<std::string>(pVal);
}

constant &constant::operator=(const constant &pVal) {
  if (this != &pVal) {
    if (pVal.mIVal) {
      mIVal = std::make_unique<int>(*(pVal.mIVal));
    }
    if (pVal.mSVal) {
      mSVal = std::make_unique<std::string>(*(pVal.mSVal));
    }
  }
  return *this;
}

int constant::as_int() const { return *mIVal; }

std::string constant::as_string() const { return *mSVal; }

std::string constant::to_string() const {
  return (mIVal ? std::to_string(*mIVal) : *mSVal);
}

int constant::hash_code() const {
  return (mIVal ? std::hash<int>{}(*mIVal) : std::hash<std::string>{}(*mSVal));
}

bool constant::is_null() const {
  if (mIVal || mSVal) {
    return false;
  } else {
    return true;
  }
}
} // namespace simpledb
