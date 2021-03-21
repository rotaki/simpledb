#include "multibuffer/multibufferproductplan.hpp"
#include "materialize/materializeplan.hpp"
#include "multibuffer/multibufferproductscan.hpp"

namespace simpledb {
multibuffer_product_plan::multibuffer_product_plan(
    transaction &pTx, const std::shared_ptr<plan> &pLhs,
    const std::shared_ptr<plan> &pRhs)
    : mTx(pTx) {
  mLhs = std::make_shared<materialize_plan>(&mTx, pLhs);
  mRhs = pRhs;
  mSch.add_all(mLhs->get_schema());
  mSch.add_all(mRhs->get_schema());
}

std::shared_ptr<scan> multibuffer_product_plan::open() {
  std::shared_ptr<scan> leftScan = mLhs->open();
  std::shared_ptr<temp_table> tT = copy_records_from(*mRhs);
  return std::static_pointer_cast<scan>(
      std::make_shared<multibuffer_product_scan>(
          &mTx, leftScan, tT->table_name(), tT->get_layout()));
}

int multibuffer_product_plan::blocks_accessed() {
  int avail = mTx.available_buffers();
  int size = std::make_unique<materialize_plan>(&mTx, mRhs)->blocks_accessed();
  int numChunks = size / avail;
  return mRhs->blocks_accessed() + (mLhs->blocks_accessed() * numChunks);
}

int multibuffer_product_plan::records_output() {
  return mLhs->records_output() * mRhs->records_output();
}

int multibuffer_product_plan::distinct_values(const std::string &pFldName) {
  if (mLhs->get_schema().has_field(pFldName)) {
    return mLhs->distinct_values(pFldName);
  } else {
    return mRhs->distinct_values(pFldName);
  }
}

schema multibuffer_product_plan::get_schema() { return mSch; }

std::shared_ptr<temp_table>
multibuffer_product_plan::copy_records_from(plan &p) {
  std::shared_ptr<scan> src = p.open();
  schema sch = p.get_schema();
  auto t = std::make_shared<temp_table>(&mTx, sch);
  auto dest = std::static_pointer_cast<update_scan>(t->open());
  while (src->next()) {
    dest->insert();
    for (const std::string &fldName : sch.fields()) {
      dest->set_val(fldName, src->get_val(fldName));
    }
  }
  src->close();
  dest->close();
  return t;
}
} // namespace simpledb
