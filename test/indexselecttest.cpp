#include "index/planner/indexselectplan.hpp"
#include "metadata/indexmanager.hpp"
#include "metadata/metadatamanager.hpp"
#include "plan/plan.hpp"
#include "plan/tableplan.hpp"
#include "record/rid.hpp"
#include "server/simpledb.hpp"
#include "tx/transaction.hpp"
#include "gtest/gtest.h"

namespace simpledb {
void use_index_manually(const index_info &iI, const std::shared_ptr<plan> &p,
                        const constant &c) {
  std::shared_ptr<table_scan> s =
      std::static_pointer_cast<table_scan>(p->open());
  std::shared_ptr<index> idx = iI.open();

  idx->before_first(c);
  while (idx->next()) {
    rid dataRID = idx->get_data_rid();
    s->move_to_rid(dataRID);
    std::cout << s->get_string("grade") << std::endl;
  }
  idx->close();
  s->close();
}

void use_index_scan(const index_info &iI, const std::shared_ptr<plan> &p,
                    const constant &c) {
  std::shared_ptr<plan> idxPlan = std::static_pointer_cast<plan>(
      std::make_shared<index_select_plan>(p, iI, c));
  std::shared_ptr<scan> s = idxPlan->open();

  while (s->next()) {
    std::cout << s->get_string("grade") << std::endl;
  }
  s->close();
}

TEST(index_query, indexselect_test) {
  simpledb db("root:password@localhost");
  metadata_manager &mM = db.md_mgr();
  auto tx = db.new_tx();

  // Find the index on studentid.
  std::map<std::string, index_info> indexes =
      mM.get_index_info("enroll", tx.get());
  index_info sIdIdx = indexes.at("studentid");

  // get the plan for the enroll table
  std::shared_ptr<plan> enrollPlan = std::static_pointer_cast<plan>(
      std::make_shared<table_plan>(tx.get(), "enroll", &mM));

  // create the selection constant
  constant c(6);

  // two different ways to use the index in simpledb:
  use_index_manually(sIdIdx, enrollPlan, c);
  use_index_scan(sIdIdx, enrollPlan, c);

  tx->commit();
}
} // namespace simpledb
