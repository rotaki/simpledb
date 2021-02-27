#include "querydata.hpp"

namespace smartdb {
  query_data::query_data(){}

  query_data::query_data(const query_data &pQD):
    mFields(pQD.mFields), mTables(pQD.mTables), mPred(pQD.mPred) {}
  
  query_data::query_data(const std::vector<std::string> pFields, const std::set<std::string> &pTables, const predicate &pPred):
    mFields(pFields), mTables(pTables), mPred(pPred) {}

  query_data& query_data::operator=(const query_data &pQD) {
    if (this != &pQD) {
      mFields = pQD.mFields;
      mTables = pQD.mTables;
      mPred = pQD.mPred;
    }
    return *this;
  }
  
  std::vector<std::string> query_data::fields() const {
    return mFields;
  }

  std::set<std::string> query_data::tables() const {
    return mTables;
  }

  predicate query_data::pred() const {
    return mPred;
  }

  std::string query_data::to_string() const {
    std::string result = "select ";
    for (const std::string &fldName: mFields) {
      result += fldName + ", ";
    }
    result = result.substr(0, result.size()-2); // zap final comma
    result += " from ";
    for (const std::string &tblName: mTables) {
      result += tblName + ", ";
    }
    result = result.substr(0, result.size()-2); // zap final comma
    std::string predString = mPred.to_string();
    if (!predString.empty()) {
      result += " where " + predString;
    }
    return result;
  }
}
