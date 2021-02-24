#pragma once

#include "filemanager.hpp"
#include "logmanager.hpp"
#include "buffermanager.hpp"
#include "transaction.hpp"
#include "metadatamanager.hpp"

namespace smartdb {
  class smartdb {
  public:
    const int mBlockSize = 400;
    const int mBufferSize = 8;
    const std::string mLogFile = "simpledb.log";
    
    smartdb(const std::string &pDirname, const int &pBlockSize, const int &pBuffSize);
    smartdb(const std::string &pDirname);

    std::shared_ptr<transaction> new_tx();
    std::shared_ptr<metadata_manager> new_mm();
    std::shared_ptr<file_manager> new_fm();
    std::shared_ptr<log_manager> new_lm();
    std::shared_ptr<buffer_manager> new_bm();
    
  private:
    std::shared_ptr<file_manager> mFM;
    std::shared_ptr<buffer_manager> mBM;
    std::shared_ptr<log_manager> mLM;
    std::shared_ptr<metadata_manager> mMM;
  };
}
