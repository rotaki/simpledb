#pragma once

#include <string>

#include "materialize/sortscan.hpp"
#include "query/constant.hpp"

namespace simpledb {
class merge_join_scan : public scan {
public:
  merge_join_scan(const std::shared_ptr<scan> &pS1,
                  const std::shared_ptr<sort_scan> &pS2,
                  const std::string &pFldName1, const std::string &pFldName2);

  void before_first() override;
  bool next() override;
  int get_int(const std::string &pFldName) override;
  std::string get_string(const std::string &pFldName) override;
  constant get_val(const std::string &pFldName) override;
  bool has_field(const std::string &pFldName) override;
  void close() override;

private:
  std::shared_ptr<scan> mS1;
  std::shared_ptr<sort_scan> mS2;
  std::string mFldName1, mFldName2;
  constant mJoinVal;
};
} // namespace simpledb
