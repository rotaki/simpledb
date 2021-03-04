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
    mFM = std::make_unique<file_manager>(path, pBlockSize); // todo unique
    mLM = std::make_unique<log_manager>(mFM.get(), mLogFile);
    mBM = std::make_unique<buffer_manager>(mFM.get(), mLM.get(), pBuffSize);
  }

  smartdb::smartdb(const std::string &pDirName): smartdb(pDirName, mBlockSize, mBufferSize) {
    //std::unique_ptr<transaction> tx = transaction::create(mFM.get(), mLM.get(), mBM.get());
    auto tx = std::make_shared<transaction>(mFM.get(), mLM.get(), mBM.get());
    //auto tx = std::make_unique<transaction>(mFM.get(), mLM.get(), mBM.get());
    bool isNew = mFM->is_new();
    if (isNew) {
      std::cout << "creating new database" << std::endl;
    } else {
      std::cout << "recovering existing database" << std::endl;
      tx->recover();
    }
    mMM = std::shared_ptr<metadata_manager>(new metadata_manager(isNew, tx));
    std::shared_ptr<query_planner> qP(new basic_query_planner(mMM));
    std::shared_ptr<update_planner> uP(new basic_update_planner(mMM));
    mP = std::shared_ptr<planner>(new planner(qP, uP));
    tx->commit();
  }

  std::unique_ptr<transaction> smartdb::new_tx() {
    auto txPtr = std::make_unique<transaction>(mFM.get(), mLM.get(), mBM.get());
    return txPtr;
  }

  std::shared_ptr<metadata_manager> smartdb::new_mm() {
    return mMM;
  }
  
  file_manager* smartdb::new_fm() {
    return mFM.get();
  }

  std::shared_ptr<planner> smartdb::new_planner() {
    return mP;
  }

  log_manager* smartdb::new_lm() {
    return mLM.get();
  }

  buffer_manager* smartdb::new_bm() {
    return mBM.get();
  }
}
