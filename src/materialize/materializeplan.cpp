#include <cmath>

#include "materialize/materializeplan.hpp"
#include "materialize/temptable.hpp"

namespace simpledb {
materialize_plan::materialize_plan(transaction *pTx,
                                   const std::shared_ptr<plan> &pSrcPlan)
    : mTx(pTx), mSrcPlan(pSrcPlan) {}

std::shared_ptr<scan> materialize_plan::open() {
  schema sch = mSrcPlan->get_schema();
  std::shared_ptr<scan> src = mSrcPlan->open();
  temp_table temp(mTx, sch);
  std::shared_ptr<update_scan> dest = temp.open();
  while (src->next()) {
    dest->insert();
    for (const std::string &fldName : sch.fields()) {
      dest->set_val(fldName, src->get_val(fldName));
    }
  }
  src->close();
  dest->before_first();
  return std::static_pointer_cast<scan>(dest);
}

int materialize_plan::blocks_accessed() {
  layout lt(mSrcPlan->get_schema());
  int rPB = (mTx->block_size() / lt.slot_size());
  return std::ceil(static_cast<double>(mSrcPlan->records_output()) /
                   static_cast<double>(rPB));
}

int materialize_plan::records_output() { return mSrcPlan->records_output(); }

int materialize_plan::distinct_values(const std::string &pFldName) {
  return mSrcPlan->distinct_values(pFldName);
}

schema materialize_plan::get_schema() { return mSrcPlan->get_schema(); }
} // namespace simpledb
