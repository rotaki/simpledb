#include "index/planner/indexselectplan.hpp"
#include "index/query/indexselectscan.hpp"
#include "record/tablescan.hpp"

namespace simpledb {
index_select_plan::index_select_plan(std::shared_ptr<plan> pP,
                                     const index_info &pII,
                                     const constant &pVal)
    : mP(pP), mII(pII), mVal(pVal) {}

std::shared_ptr<scan> index_select_plan::open() {
  std::shared_ptr<table_scan> tS =
      std::dynamic_pointer_cast<table_scan>(mP->open());
  if (!tS) {
    throw std::runtime_error("type conversion failed");
  }
  std::shared_ptr<index> idx = mII.open();
  return std::static_pointer_cast<scan>(
      std::make_shared<index_select_scan>(tS, idx, mVal));
}

int index_select_plan::blocks_accessed() {
  return mII.blocks_accessed() + records_output();
}

int index_select_plan::records_output() { return mII.records_output(); }

int index_select_plan::distinct_values(const std::string &pFldName) {
  return mII.distinct_values(pFldName);
}

schema index_select_plan::get_schema() { return mP->get_schema(); }

} // namespace simpledb
