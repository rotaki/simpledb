#pragma once

#include <memory>
#include <string>
#include <vector>

#include "materialize/aggregationfn.hpp"
#include "materialize/groupvalue.hpp"
#include "query/constant.hpp"
#include "query/scan.hpp"

namespace simpledb {
class group_by_scan : public scan {
public:
  group_by_scan(const std::shared_ptr<scan> &pS,
                const std::vector<std::string> &pGroupFields,
                const std::vector<std::shared_ptr<aggregation_fn>> &pAggFns);
  void before_first() override;
  bool next() override;
  int get_int(const std::string &pFldName) override;
  std::string get_string(const std::string &pFldName) override;
  constant get_val(const std::string &pFldName) override;
  bool has_field(const std::string &pFldName) override;
  void close() override;

private:
  std::shared_ptr<scan> mS;
  std::vector<std::string> mGroupFields;
  std::vector<std::shared_ptr<aggregation_fn>> mAggFns;
  std::unique_ptr<group_value> mGroupVal;
  bool mMoreGroups;
};
} // namespace simpledb
