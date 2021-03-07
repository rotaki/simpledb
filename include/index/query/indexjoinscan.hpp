#pragma once

#include <memory>

#include "index/index.hpp"
#include "record/tablescan.hpp"

namespace simpledb {
class index_join_scan : public scan {
public:
  index_join_scan(const std::shared_ptr<scan> &pLhs,
                  const std::shared_ptr<index> &pIdx,
                  const std::string &pJoinFld,
                  const std::shared_ptr<table_scan> &pRhs);
  void before_first();
  bool next();
  int get_int(const std::string &pFldName);
  std::string get_string(const std::string &pFldName);
  constant get_val(const std::string &pFldName);
  bool has_field(const std::string &pFldName);
  void close();
  void reset_index();

private:
  std::shared_ptr<scan> mLhs;
  std::shared_ptr<index> mIdx;
  std::string mJoinField;
  std::shared_ptr<table_scan> mRhs;
};
} // namespace simpledb
