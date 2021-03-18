#include "index/planner/indexjoinplan.hpp"
#include "metadata/indexmanager.hpp"
#include "metadata/metadatamanager.hpp"
#include "plan/plan.hpp"
#include "plan/tableplan.hpp"
#include "record/rid.hpp"
#include "server/simpledb.hpp"
#include "tx/transaction.hpp"
#include "gtest/gtest.h"

namespace simpledb {
void use_index_manually(const std::shared_ptr<plan> &p1,
                        const std::shared_ptr<plan> &p2, const index_info &iI,
                        const std::string &joinField) {
  std::shared_ptr<scan> s1 = p1->open();
  std::shared_ptr<table_scan> s2 =
      std::static_pointer_cast<table_scan>(p2->open());
  std::shared_ptr<index> idx = iI.open();

  while (s1->next()) {
    constant c = s1->get_val(joinField);
    idx->before_first(c);
    while (idx->next()) {
      rid dataRID = idx->get_data_rid();
      s2->move_to_rid(dataRID);
      std::cout << s2->get_string("grade") << std::endl;
    }
  }

  idx->close();
  s1->close();
  s2->close();
}

void use_index_scan(const std::shared_ptr<plan> &p1,
                    const std::shared_ptr<plan> &p2, const index_info &iI,
                    const std::string &joinField) {
  std::shared_ptr<plan> idxPlan = std::static_pointer_cast<plan>(
      std::make_shared<index_join_plan>(p1, p2, iI, joinField));
  std::shared_ptr<scan> s = idxPlan->open();
  while (s->next()) {
    std::cout << s->get_string("grade") << std::endl;
  }
  s->close();
}

TEST(index_query, indexjoin_test) {
  simpledb db("root:password@localhost");
  metadata_manager &mM = db.md_mgr();
  auto tx = db.new_tx();

  // find the index on studentid
  std::map<std::string, index_info> indexes =
      mM.get_index_info("enroll", tx.get());
  index_info sIdIdx = indexes.at("studentid");

  // get plans for the student and enroll tables
  std::shared_ptr<plan> studentPlan = std::static_pointer_cast<plan>(
      std::make_shared<table_plan>(tx.get(), "student", &mM));
  std::shared_ptr<plan> enrollPlan = std::static_pointer_cast<plan>(
      std::make_shared<table_plan>(tx.get(), "enroll", &mM));

  // two different ways to use the index in simpledb:
  use_index_manually(studentPlan, enrollPlan, sIdIdx, "sid");
  use_index_scan(studentPlan, enrollPlan, sIdIdx, "sid");

  tx->commit();
}
} // namespace simpledb
