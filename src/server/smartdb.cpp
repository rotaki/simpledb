#include <filesystem>
#include <memory>
#include <iostream>

#include "server/smartdb.hpp"
#include "plan/basicqueryplanner.hpp"
#include "plan/basicupdateplanner.hpp"

namespace smartdb {
  int smartdb::mBlockSize = 400;
  int smartdb::mBufferSize = 8;
  std::string smartdb::mLogFile = "simpledb.log";
  
  smartdb::smartdb(const std::string &pDirName, const int &pBlockSize, const int &pBuffSize) {
    auto path = std::filesystem::current_path() / pDirName;
    mFM = std::make_unique<file_manager>(path, pBlockSize);
    mLM = std::make_unique<log_manager>(mFM.get(), mLogFile);
    mBM = std::make_unique<buffer_manager>(mFM.get(), mLM.get(), pBuffSize);
  }

  smartdb::smartdb(const std::string &pDirName): smartdb(pDirName, mBlockSize, mBufferSize) {
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
    auto uP = std::make_unique<basic_update_planner>(mMM.get());
    mP = std::make_unique<planner>(std::move(qP), std::move(uP));
    tx->commit();
  }

  std::unique_ptr<transaction> smartdb::new_tx() {
    auto txPtr = std::make_unique<transaction>(mFM.get(), mLM.get(), mBM.get());
    return txPtr;
  }

  metadata_manager* smartdb::md_mgr() {
    return mMM.get();
  }
  
  file_manager* smartdb::file_mgr() {
    return mFM.get();
  }

  planner* smartdb::plnr() {
    return mP.get();
  }

  log_manager* smartdb::log_mgr() {
    return mLM.get();
  }

  buffer_manager* smartdb::buffer_mgr() {
    return mBM.get();
  }
}
