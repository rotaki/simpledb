#pragma once

#include "file/filemanager.hpp"
#include "log/logmanager.hpp"
#include "buffer/buffermanager.hpp"
#include "tx/transaction.hpp"
#include "metadata/metadatamanager.hpp"
#include "plan/planner.hpp"

namespace smartdb {
  class smartdb {
  public:
    static int mBlockSize;
    static int mBufferSize;
    static std::string mLogFile;
    
    smartdb(const std::string &pDirname, const int &pBlockSize, const int &pBuffSize);
    smartdb(const std::string &pDirname);

    std::shared_ptr<transaction> new_tx();
    std::shared_ptr<metadata_manager> new_mm();
    std::shared_ptr<planner> new_planner();
    std::shared_ptr<file_manager> new_fm();
    std::shared_ptr<log_manager> new_lm();
    std::shared_ptr<buffer_manager> new_bm();
    
  private:
    std::shared_ptr<file_manager> mFM;
    std::shared_ptr<buffer_manager> mBM;
    std::shared_ptr<planner> mP;
    std::shared_ptr<log_manager> mLM;
    std::shared_ptr<metadata_manager> mMM;
  };
}
