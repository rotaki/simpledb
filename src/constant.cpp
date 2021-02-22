#include "constant.hpp"

namespace smartdb {
  constant::constant(const constant &pVal):
    mIVal(pVal.mIVal), mSVal(pVal.mSVal) {}

  constant::constant(const int &pVal) {
    mIVal = std::shared_ptr<int>(new int(pVal));
  }

  constant::constant(const std::string &pVal) {
    mSVal = std::shared_ptr<std::string>(new std::string(pVal));
  }

  int constant::as_int() const {
    return *mIVal;
  }

  std::string constant::as_string() const {
    return *mSVal;
  }

  bool constant::operator==(const constant &pVal) const {
    return (mIVal ? *mIVal == pVal.as_int() : *mSVal == pVal.as_string());
  }

  bool constant::operator<(const constant &pVal) const {
    return (mIVal ? *mIVal < pVal.as_int() : *mSVal < pVal.as_string());
  }

  std::string constant::to_string() const {
    return (mIVal ? std::to_string(*mIVal) : *mSVal);
  }
}
