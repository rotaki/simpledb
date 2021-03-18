#include <iostream>

#include "metadata/indexmanager.hpp"
#include "metadata/metadatamanager.hpp"
#include "plan/plan.hpp"
#include "plan/tableplan.hpp"
#include "record/rid.hpp"
#include "server/simpledb.hpp"
#include "tx/transaction.hpp"
#include "gtest/gtest.h"

namespace simpledb {
TEST(index, indexretrieval_test) {
  simpledb db("root:password@localhost");
  auto tx = db.new_tx();
  metadata_manager &mM = db.md_mgr();

  std::shared_ptr<plan> studentPlan = std::static_pointer_cast<plan>(
      std::make_shared<table_plan>(tx.get(), "student", &mM));
  std::shared_ptr<update_scan> studentScan =
      std::static_pointer_cast<update_scan>(studentPlan->open());

  layout lt = mM.get_layout("student", tx.get());

  std::map<std::string, index_info> indexes =
      mM.get_index_info("student", tx.get());
  index_info ii = indexes.at("majorid");
  std::shared_ptr<index> idx = ii.open();

  idx->before_first(constant(20));
  while (idx->next()) {
    rid dataRID = idx->get_data_rid();
    studentScan->move_to_rid(dataRID);
    std::cout << studentScan->get_string("sname") << std::endl;
  }

  idx->close();
  studentScan->close();
  tx->commit();
}
} // namespace simpledb
