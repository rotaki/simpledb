#include <filesystem>
#include <iostream>
#include <memory>

#include "index/planner/indexupdateplanner.hpp"
#include "plan/basicqueryplanner.hpp"
#include "plan/basicupdateplanner.hpp"
#include "server/simpledb.hpp"

namespace simpledb {
int simpledb::mBlockSize = 400;
int simpledb::mBufferSize = 8;
std::string simpledb::mLogFile = "simpledb.log";

simpledb::simpledb(const std::string &pDirName, int pBlockSize, int pBuffSize) {
  auto path = std::filesystem::current_path() / pDirName;
  mFM = std::make_unique<file_manager>(path, pBlockSize);
  mLM = std::make_unique<log_manager>(mFM.get(), mLogFile);
  mBM = std::make_unique<buffer_manager>(mFM.get(), mLM.get(), pBuffSize);
}

simpledb::simpledb(const std::string &pDirName)
    : simpledb(pDirName, mBlockSize, mBufferSize) {
  auto tx = std::make_unique<transaction>(mFM.get(), mLM.get(), mBM.get());
  bool isNew = mFM->is_new();
  if (isNew) {
    std::cout << "creating new database" << std::endl;
  } else {
    std::cout << "recovering existing database" << std::endl;
    tx->recover();
  }
  mMM = std::make_unique<metadata_manager>(isNew, tx.get());
  auto qP = std::make_unique<basic_query_planner>(mMM.get());
  // auto uP = std::make_unique<basic_update_planner>(mMM.get());
  auto uP = std::make_unique<index_update_planner>(mMM.get());
  mP = std::make_unique<planner>(std::move(qP), std::move(uP));
  tx->commit();
}

std::unique_ptr<transaction> simpledb::new_tx() {
  auto txPtr = std::make_unique<transaction>(mFM.get(), mLM.get(), mBM.get());
  return txPtr;
}

metadata_manager &simpledb::md_mgr() { return *mMM; }

file_manager &simpledb::file_mgr() { return *mFM; }

planner &simpledb::plnr() { return *mP; }

log_manager &simpledb::log_mgr() { return *mLM; }

buffer_manager &simpledb::buffer_mgr() { return *mBM; }
} // namespace simpledb
