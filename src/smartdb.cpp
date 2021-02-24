#include <filesystem>
#include <iostream>

#include "smartdb.hpp"

namespace smartdb {
  smartdb::smartdb(const std::string &pDirName, const int &pBlockSize, const int &pBuffSize) {
    auto path = std::filesystem::current_path() / pDirName;
    mFM = std::shared_ptr<file_manager>(new file_manager(path, pBlockSize));
    mLM = std::shared_ptr<log_manager>(new log_manager(mFM, mLogFile));
    mBM = std::shared_ptr<buffer_manager>(new buffer_manager(mFM, mLM, pBuffSize));
  }

  smartdb::smartdb(const std::string &pDirName) {
    smartdb(pDirName, mBlockSize, mBufferSize);
    std::shared_ptr<transaction> tx = transaction::create(mFM, mLM, mBM);
    bool isNew = mFM->is_new();
    if (isNew) {
      std::cout << "creating new database" << std::endl;
    } else {
      std::cout << "recovering existing database" << std::endl;
      tx->recover();
    }
    mMM = std::shared_ptr<metadata_manager>(new metadata_manager(isNew, tx));
    tx->commit();
  }

  std::shared_ptr<transaction> smartdb::new_tx() {
    return transaction::create(mFM, mLM, mBM);
  }

  std::shared_ptr<metadata_manager> smartdb::new_mm() {
    return mMM;
  }
  
  std::shared_ptr<file_manager> smartdb::new_fm() {
    return mFM;
  }

  std::shared_ptr<log_manager> smartdb::new_lm() {
    return mLM;
  }

  std::shared_ptr<buffer_manager> smartdb::new_bm() {
    return mBM;
  }
}
