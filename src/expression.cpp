#include "expression.hpp"

namespace smartdb {
  expression::expression() {}
  
  expression::expression(const expression &pE):
    mVal(pE.mVal), mFldName(pE.mFldName) {}
  
  expression::expression(const constant &pVal):
    mVal(pVal) {}

  expression::expression(const std::string &pFldName):
    mFldName(pFldName) {}

  expression& expression::operator=(const expression &pE) {
    if (this != &pE) {
      mVal = pE.mVal;
      mFldName = pE.mFldName;
    }
    return *this;
  }

  bool expression::is_field_name() {
    return !mFldName.empty();
  }

  constant expression::as_constant() {
    return mVal;
  }

  std::string expression::as_field_name() {
    return mFldName;
  }

  constant expression::evaluate(std::shared_ptr<scan> pS) {
    return (!mVal.is_null() ? pS->get_val(mFldName) : mVal);
  }

  bool expression::applies_to(std::shared_ptr<schema> pSch) {
    return (!mVal.is_null() ? pSch->has_field(mFldName) : true);
  }

  std::string expression::to_string() {
    return (!mVal.is_null() ? mFldName : mVal.to_string());
  }
}
