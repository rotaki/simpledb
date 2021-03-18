#include "metadata/indexmanager.hpp"
#include "metadata/metadatamanager.hpp"
#include "plan/plan.hpp"
#include "plan/tableplan.hpp"
#include "record/rid.hpp"
#include "server/simpledb.hpp"
#include "tx/transaction.hpp"
#include "gtest/gtest.h"

namespace simpledb {
TEST(index, indexupdate_test) {
  simpledb db("root:password@localhost");
  auto tx = db.new_tx();
  metadata_manager &mM = db.md_mgr();
  std::shared_ptr<plan> studentPlan = std::static_pointer_cast<plan>(
      std::make_shared<table_plan>(tx.get(), "student", &mM));
  std::shared_ptr<update_scan> studentScan =
      std::static_pointer_cast<update_scan>(studentPlan->open());

  // create a map containing all indexes for STUDENT
  std::map<std::string, std::shared_ptr<index>> indexes;
  std::map<std::string, index_info> idxInfo =
      mM.get_index_info("student", tx.get());
  for (const auto &[fldName, iI] : idxInfo) {
    indexes[fldName] = idxInfo.at(fldName).open();
  }

  // Task1: Insert a new STUDENT record for Sam.
  // First, insert the record into STUDENT
  studentScan->insert();
  studentScan->set_int("sid", 1);
  studentScan->set_string("sname", "sam");
  studentScan->set_int("gradyear", 2023);
  studentScan->set_int("majorid", 30);

  rid dataRID = studentScan->get_rid();
  for (const auto &[fldName, idx] : indexes) {
    constant dataVal = studentScan->get_val(fldName);
    idx->insert(dataVal, dataRID);
  }

  // Task 2: Find and delete Joe's record
  studentScan->before_first();
  while (studentScan->next()) {
    if (studentScan->get_string("sname") == "joe") {
      // First, delete the index records for Joe.
      rid joeRID = studentScan->get_rid();
      for (const auto &[fldName, idx] : indexes) {
        constant dataVal = studentScan->get_val(fldName);
        idx->remove(dataVal, joeRID);
      }
      // Then delete Joe's record in student
      studentScan->remove();
      break;
    }
  }

  // print the records to verify the updates
  studentScan->before_first();
  while (studentScan->next()) {
    std::cout << studentScan->get_string("sname") << " "
              << studentScan->get_int("sid") << std::endl;
  }
  studentScan->close();
  for (const auto &[fldName, idx] : indexes) {
    idx->close();
  }
  tx->commit();
}
} // namespace simpledb
