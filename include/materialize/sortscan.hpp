#pragma once

#include "materialize/materializeplan.hpp"
#include "materialize/recordcomparator.hpp"
#include "materialize/temptable.hpp"
#include "plan/plan.hpp"
#include "query/updatescan.hpp"
#include "record/layout.hpp"
#include "record/tablescan.hpp"
#include "tx/transaction.hpp"

namespace simpledb {
class sort_scan : public scan {
public:
  sort_scan(const std::vector<std::shared_ptr<temp_table>> &pRuns,
            record_comparator &pComp); // todo record comparator is ref ok??
  void before_first() override;
  bool next() override;
  int get_int(const std::string &pFldName) override;
  std::string get_string(const std::string &pFldName) override;
  constant get_val(const std::string &pFldName) override;
  bool has_field(const std::string &pFldName) override;
  void close() override;

  void save_position();
  void restore_position();

private:
  std::shared_ptr<update_scan> mS1, mS2, mCurrentScan;
  record_comparator &mComp;
  bool mHasMore1, mHasMore2;
  std::vector<rid> mSavedPosition;
};
} // namespace simpledb
